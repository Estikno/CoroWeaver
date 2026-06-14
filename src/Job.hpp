#pragma once

#include <limits>
#include <memory>
#include <functional>
#include <coroutine>
#include <utility>

#include "Types.hpp"
#include "RingBuffer.hpp"

namespace cw {
    // Useful defines
    inline static constexpr u32 BufferCapacity = 64;

    using ThreadAffinity = u8;
    enum class JobPriority { Low = 0, Medium, High };
    template <typename T>
    using JobBufferPtr = std::unique_ptr<RingBuffer<T, BufferCapacity>>;
    using Tag = u8;

    constexpr ThreadAffinity InvalidThreadIndex = std::numeric_limits<ThreadAffinity>::max();
    constexpr ThreadAffinity MaxThreads = 64;
    constexpr Tag InvalidTag = std::numeric_limits<Tag>::max();

    // Forward declarations
    struct Job;
    template <typename T>
    class JobCoroutine;
    template <typename T>
    class JobPromise;
    template <typename T>
    class JobPromiseBase;
    struct JobFunction;
    template <typename T>
    struct FinalAwaiter;
    template <typename T, typename... Us>
    struct JobAwaiterMultiple;
    template <typename T>
    struct ThreadAwaiter;

    /**
     * This is the base Job sruct. Alls jobs derive from this.
     * */
    struct Job {
        JobPriority m_Priority{JobPriority::Medium};
        ThreadAffinity m_ThreadIndex{InvalidThreadIndex};
        bool m_IsFunction{true};
        Tag m_Tag{InvalidTag};

        // Who is waiting for me (only on coroutines)
        Job* m_Parent{nullptr};
        // How many children I'm waiting for (only on coroutines)
        std::atomic<u32> m_Children{0};

        Job() = default;
        Job(JobPriority priority, ThreadAffinity threadIndex, bool isFunction, Tag tag)
            : m_Priority(priority),
              m_ThreadIndex(threadIndex),
              m_IsFunction(isFunction),
              m_Tag(tag) {}
        virtual ~Job() = default;

        virtual void Resume() = 0;
        virtual void Destroy() = 0;
    };

    /**
     * Represents a simple function job.
     * */
    struct JobFunction : public Job {
        std::function<void()> m_Function;

        JobFunction() = default;
        JobFunction(std::function<void()> func,
                    JobPriority priority = JobPriority::Medium,
                    ThreadAffinity threadIndex = InvalidThreadIndex,
                    Tag tag = InvalidTag)
            : m_Function(func),
              Job(priority, threadIndex, true, tag) {}

        void Resume() override {
            m_Function();
        }

        void Destroy() override {}
    };

    /**
     * Creates a job coroutine
     * */
    template <typename T>
    class JobCoroutine {
    public:
        using promise_type = JobPromise<T>;

        JobCoroutine() = default;
        explicit JobCoroutine(std::coroutine_handle<JobPromise<T>> h)
            : m_Handle(h) {}

        JobCoroutine(JobCoroutine&& other) noexcept
            : m_Handle(std::exchange(other.m_Handle, {})),
              m_Scheduled(std::exchange(other.m_Scheduled, false)) {}
        JobCoroutine& operator=(JobCoroutine&& other) noexcept {
            if (this != &other) {
                // Destroy our current handle if we own one
                if (m_Handle)
                    m_Handle.destroy();
                m_Handle = std::exchange(other.m_Handle, {});
                m_Scheduled = std::exchange(other.m_Scheduled, false);
            }
            return *this;
        }
        JobCoroutine(const JobCoroutine&) = delete;
        ~JobCoroutine() {
            if (!m_Handle)
                return;

            if (!m_Scheduled) {
                m_Handle.destroy();
                return;
            }

            // If scheduled as a child (has parent) and is done,
            // we are responsible for cleanup since FinalAwaiter returned true
            if (m_Handle.done() && m_Handle.promise().m_Parent != nullptr) {
                m_Handle.destroy();
                return;
            }

            // If scheduled without parent, FinalAwaiter self-destructed, don't touch it
        }

        void Schedule() {
            m_Scheduled = true;
        }

        void ReleaseHandle() {
            m_Handle = {};
        }

        std::coroutine_handle<JobPromise<T>> GetHandle() {
            return m_Handle;
        }

        template <typename U = T>
            requires(!std::is_void_v<U>)
        bool Get(U& value) noexcept {
            if (m_Handle && m_Handle.done()) {
                value = m_Handle.promise().Get();
                return true;
            }
            return false;
        }

    private:
        std::coroutine_handle<JobPromise<T>> m_Handle{};
        bool m_Scheduled{false};
    };

    template <typename... Us>
    struct WhenAllTag {
        std::tuple<JobCoroutine<Us>...> coros;
        ThreadAffinity m_ThreadIndex{InvalidThreadIndex};
    };

    template <typename... Us>
    WhenAllTag<Us...> WhenAll(ThreadAffinity affinity, JobCoroutine<Us>&&... coros) {
        return {std::tuple<JobCoroutine<Us>...>(std::move(coros)...), affinity};
    }

    template <typename... Us>
    WhenAllTag<Us...> WhenAll(JobCoroutine<Us>&&... coros) {
        return {std::tuple<JobCoroutine<Us>...>(std::move(coros)...), InvalidThreadIndex};
    }

    /**
     * Base class for all coroutine promises. It's needed to distinguish between
     * void and any other return type.
     * */
    template <typename T>
    class JobPromiseBase : public Job {
    public:
        JobPromiseBase(std::coroutine_handle<> handle)
            : m_Handle(handle),
              Job(JobPriority::Medium, InvalidThreadIndex, false, InvalidTag) {}

        std::suspend_always initial_suspend() noexcept {
            return {};
        }
        FinalAwaiter<T> final_suspend() noexcept {
            return {};
        }

        void unhandled_exception() {
            CW_PANIC("Coroutines in the JobSystem panicked");
            // std::terminate();
        }

        template <typename... Us>
        JobAwaiterMultiple<T, Us...> await_transform(WhenAllTag<Us...>&& tag) {
            ThreadAffinity affinity = tag.m_ThreadIndex;
            return std::apply(
                [affinity](auto&&... coros) { return JobAwaiterMultiple<T, Us...>(affinity, std::move(coros)...); },
                std::move(tag.coros));
        }

        ThreadAwaiter<T> await_transform(ThreadAffinity thread) {
            return ThreadAwaiter<T>(thread);
        }

        virtual void Resume() override {
            if (m_Handle && !m_Handle.done())
                m_Handle.resume();
        }
        virtual void Destroy() override {
            m_Handle.destroy();
        }

    private:
        std::coroutine_handle<> m_Handle;
    };

    template <typename T>
    class JobPromise : public JobPromiseBase<T> {
    public:
        JobPromise() noexcept
            : JobPromiseBase<T>(std::coroutine_handle<JobPromise<T>>::from_promise(*this)) {}

        JobCoroutine<T> get_return_object() {
            return JobCoroutine<T>(std::coroutine_handle<JobPromise<T>>::from_promise(*this));
        }

        void return_value(T value) noexcept {
            m_Value = value;
        }

        T Get() {
            return m_Value;
        }

    private:
        T m_Value{};
    };

    // Void specialization of the JobPromise
    template <>
    class JobPromise<void> : public JobPromiseBase<void> {
    public:
        JobPromise() noexcept
            : cw::JobPromiseBase<void>(std::coroutine_handle<JobPromise<void>>::from_promise(*this)) {}

        JobCoroutine<void> get_return_object() {
            return JobCoroutine<void>(std::coroutine_handle<JobPromise<void>>::from_promise(*this));
        }

        void return_void() {}
    };
} // namespace cw
