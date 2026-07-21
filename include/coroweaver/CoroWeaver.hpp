// ════════════════════════════════════════════════════════════
// AUTO-GENERATED — do not edit directly.
// Generated from CoroWeaver.hpp via scripts/amalgamate.py
// ════════════════════════════════════════════════════════════

// ────────────────────────────────────────────────────────────
// LICENSE
// ────────────────────────────────────────────────────────────
//                                  Apache License
//                            Version 2.0, January 2004
//                         http://www.apache.org/licenses/
//
//    TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION
//
//    1. Definitions.
//
//       "License" shall mean the terms and conditions for use, reproduction,
//       and distribution as defined by Sections 1 through 9 of this document.
//
//       "Licensor" shall mean the copyright owner or entity authorized by
//       the copyright owner that is granting the License.
//
//       "Legal Entity" shall mean the union of the acting entity and all
//       other entities that control, are controlled by, or are under common
//       control with that entity. For the purposes of this definition,
//       "control" means (i) the power, direct or indirect, to cause the
//       direction or management of such entity, whether by contract or
//       otherwise, or (ii) ownership of fifty percent (50%) or more of the
//       outstanding shares, or (iii) beneficial ownership of such entity.
//
//       "You" (or "Your") shall mean an individual or Legal Entity
//       exercising permissions granted by this License.
//
//       "Source" form shall mean the preferred form for making modifications,
//       including but not limited to software source code, documentation
//       source, and configuration files.
//
//       "Object" form shall mean any form resulting from mechanical
//       transformation or translation of a Source form, including but
//       not limited to compiled object code, generated documentation,
//       and conversions to other media types.
//
//       "Work" shall mean the work of authorship, whether in Source or
//       Object form, made available under the License, as indicated by a
//       copyright notice that is included in or attached to the work
//       (an example is provided in the Appendix below).
//
//       "Derivative Works" shall mean any work, whether in Source or Object
//       form, that is based on (or derived from) the Work and for which the
//       editorial revisions, annotations, elaborations, or other modifications
//       represent, as a whole, an original work of authorship. For the purposes
//       of this License, Derivative Works shall not include works that remain
//       separable from, or merely link (or bind by name) to the interfaces of,
//       the Work and Derivative Works thereof.
//
//       "Contribution" shall mean any work of authorship, including
//       the original version of the Work and any modifications or additions
//       to that Work or Derivative Works thereof, that is intentionally
//       submitted to Licensor for inclusion in the Work by the copyright owner
//       or by an individual or Legal Entity authorized to submit on behalf of
//       the copyright owner. For the purposes of this definition, "submitted"
//       means any form of electronic, verbal, or written communication sent
//       to the Licensor or its representatives, including but not limited to
//       communication on electronic mailing lists, source code control systems,
//       and issue tracking systems that are managed by, or on behalf of, the
//       Licensor for the purpose of discussing and improving the Work, but
//       excluding communication that is conspicuously marked or otherwise
//       designated in writing by the copyright owner as "Not a Contribution."
//
//       "Contributor" shall mean Licensor and any individual or Legal Entity
//       on behalf of whom a Contribution has been received by Licensor and
//       subsequently incorporated within the Work.
//
//    2. Grant of Copyright License. Subject to the terms and conditions of
//       this License, each Contributor hereby grants to You a perpetual,
//       worldwide, non-exclusive, no-charge, royalty-free, irrevocable
//       copyright license to reproduce, prepare Derivative Works of,
//       publicly display, publicly perform, sublicense, and distribute the
//       Work and such Derivative Works in Source or Object form.
//
//    3. Grant of Patent License. Subject to the terms and conditions of
//       this License, each Contributor hereby grants to You a perpetual,
//       worldwide, non-exclusive, no-charge, royalty-free, irrevocable
//       (except as stated in this section) patent license to make, have made,
//       use, offer to sell, sell, import, and otherwise transfer the Work,
//       where such license applies only to those patent claims licensable
//       by such Contributor that are necessarily infringed by their
//       Contribution(s) alone or by combination of their Contribution(s)
//       with the Work to which such Contribution(s) was submitted. If You
//       institute patent litigation against any entity (including a
//       cross-claim or counterclaim in a lawsuit) alleging that the Work
//       or a Contribution incorporated within the Work constitutes direct
//       or contributory patent infringement, then any patent licenses
//       granted to You under this License for that Work shall terminate
//       as of the date such litigation is filed.
//
//    4. Redistribution. You may reproduce and distribute copies of the
//       Work or Derivative Works thereof in any medium, with or without
//       modifications, and in Source or Object form, provided that You
//       meet the following conditions:
//
//       (a) You must give any other recipients of the Work or
//           Derivative Works a copy of this License; and
//
//       (b) You must cause any modified files to carry prominent notices
//           stating that You changed the files; and
//
//       (c) You must retain, in the Source form of any Derivative Works
//           that You distribute, all copyright, patent, trademark, and
//           attribution notices from the Source form of the Work,
//           excluding those notices that do not pertain to any part of
//           the Derivative Works; and
//
//       (d) If the Work includes a "NOTICE" text file as part of its
//           distribution, then any Derivative Works that You distribute must
//           include a readable copy of the attribution notices contained
//           within such NOTICE file, excluding those notices that do not
//           pertain to any part of the Derivative Works, in at least one
//           of the following places: within a NOTICE text file distributed
//           as part of the Derivative Works; within the Source form or
//           documentation, if provided along with the Derivative Works; or,
//           within a display generated by the Derivative Works, if and
//           wherever such third-party notices normally appear. The contents
//           of the NOTICE file are for informational purposes only and
//           do not modify the License. You may add Your own attribution
//           notices within Derivative Works that You distribute, alongside
//           or as an addendum to the NOTICE text from the Work, provided
//           that such additional attribution notices cannot be construed
//           as modifying the License.
//
//       You may add Your own copyright statement to Your modifications and
//       may provide additional or different license terms and conditions
//       for use, reproduction, or distribution of Your modifications, or
//       for any such Derivative Works as a whole, provided Your use,
//       reproduction, and distribution of the Work otherwise complies with
//       the conditions stated in this License.
//
//    5. Submission of Contributions. Unless You explicitly state otherwise,
//       any Contribution intentionally submitted for inclusion in the Work
//       by You to the Licensor shall be under the terms and conditions of
//       this License, without any additional terms or conditions.
//       Notwithstanding the above, nothing herein shall supersede or modify
//       the terms of any separate license agreement you may have executed
//       with Licensor regarding such Contributions.
//
//    6. Trademarks. This License does not grant permission to use the trade
//       names, trademarks, service marks, or product names of the Licensor,
//       except as required for reasonable and customary use in describing the
//       origin of the Work and reproducing the content of the NOTICE file.
//
//    7. Disclaimer of Warranty. Unless required by applicable law or
//       agreed to in writing, Licensor provides the Work (and each
//       Contributor provides its Contributions) on an "AS IS" BASIS,
//       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
//       implied, including, without limitation, any warranties or conditions
//       of TITLE, NON-INFRINGEMENT, MERCHANTABILITY, or FITNESS FOR A
//       PARTICULAR PURPOSE. You are solely responsible for determining the
//       appropriateness of using or redistributing the Work and assume any
//       risks associated with Your exercise of permissions under this License.
//
//    8. Limitation of Liability. In no event and under no legal theory,
//       whether in tort (including negligence), contract, or otherwise,
//       unless required by applicable law (such as deliberate and grossly
//       negligent acts) or agreed to in writing, shall any Contributor be
//       liable to You for damages, including any direct, indirect, special,
//       incidental, or consequential damages of any character arising as a
//       result of this License or out of the use or inability to use the
//       Work (including but not limited to damages for loss of goodwill,
//       work stoppage, computer failure or malfunction, or any and all
//       other commercial damages or losses), even if such Contributor
//       has been advised of the possibility of such damages.
//
//    9. Accepting Warranty or Additional Liability. While redistributing
//       the Work or Derivative Works thereof, You may choose to offer,
//       and charge a fee for, acceptance of support, warranty, indemnity,
//       or other liability obligations and/or rights consistent with this
//       License. However, in accepting such obligations, You may act only
//       on Your own behalf and on Your sole responsibility, not on behalf
//       of any other Contributor, and only if You agree to indemnify,
//       defend, and hold each Contributor harmless for any liability
//       incurred by, or claims asserted against, such Contributor by reason
//       of your accepting any such warranty or additional liability.
//
//    END OF TERMS AND CONDITIONS
//
//    APPENDIX: How to apply the Apache License to your work.
//
//       To apply the Apache License to your work, attach the following
//       boilerplate notice, with the fields enclosed by brackets "[]"
//       replaced with your own identifying information. (Don't include
//       the brackets!)  The text should be enclosed in the appropriate
//       comment syntax for the file format. We also recommend that a
//       file or class name and description of purpose be included on the
//       same "printed page" as the copyright notice for easier
//       identification within third-party archives.
//
//    Copyright [2026] [David]
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
// ────────────────────────────────────────────────────────────

#pragma once

#include <condition_variable>
#include <memory>
#include <functional>
#include <shared_mutex>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <queue>
#include <vector>
#include <array>
#include <type_traits>
#include <variant>
#include <tuple>
#include <bit>
#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <coroutine>
#include <stop_token>
#include <concurrentqueue.hpp>
#include <cstdint>
#include <semaphore>
#include <limits>

// ─────────────────────────────────────────────
// CoroWeaver.hpp
// ─────────────────────────────────────────────









// ─────────────────────────────────────────────
// Types.hpp
// ─────────────────────────────────────────────



namespace cw {
    // Unsigned integers
    using u8 = std::uint8_t;
    using u16 = std::uint16_t;
    using u32 = std::uint32_t;
    using u64 = std::uint64_t;

    // Signed integers
    using i8 = std::int8_t;
    using i16 = std::int16_t;
    using i32 = std::int32_t;
    using i64 = std::int64_t;

    // Floating point
    using f32 = float;
    using f64 = double;

    // Compile-time size checks
    static_assert(sizeof(u8) == 1, "u8 is not 8 bits!");
    static_assert(sizeof(u16) == 2, "u16 is not 16 bits!");
    static_assert(sizeof(u32) == 4, "u32 is not 32 bits!");
    static_assert(sizeof(u64) == 8, "u64 is not 64 bits!");

    static_assert(sizeof(i8) == 1, "i8 is not 8 bits!");
    static_assert(sizeof(i16) == 2, "i16 is not 16 bits!");
    static_assert(sizeof(i32) == 4, "i32 is not 32 bits!");
    static_assert(sizeof(i64) == 8, "i64 is not 64 bits!");

    static_assert(sizeof(f32) == 4, "f32 is not 32 bits!");
    static_assert(sizeof(f64) == 8, "f64 is not 64 bits!");


// Macros for easier error handling
#define CW_PANIC(...)     \
    do {                  \
        std::terminate(); \
    } while (0)

#define CW_ENSURE(condition, ...)  \
    do {                           \
        if (!(condition)) {        \
            CW_PANIC(__VA_ARGS__); \
        }                          \
    } while (0)

} // namespace cw

// ─────────────────────────────────────────────
// RingBuffer.hpp
// ─────────────────────────────────────────────




namespace cw {
    /**
     * Fairly basic RingBuffer inspired on David Álvarez Rosa implementation:
     * https://david.alvarezrosa.com/posts/optimizing-a-lock-free-ring-buffer/
     *
     * The size must be a power of 2.
     *
     * We leave one slot permanently unused to distinguish “full” from “empty".
     * Important: This class is not thread safe if used incorrectly so read carefuly the methods comments
     * */
    template <typename T, std::size_t N>
    class RingBuffer {
    public:
        RingBuffer()
            : m_Head(0),
              m_Tail(0),
              m_TailCached(0),
              m_HeadCached(0),
              m_PushSemaphore(1) {}

        /**
         * Tries to push an element to the list. Returns without blocking if fails.
         *
         * @param elem The element to be pushed
         *
         * @returns true if it was able to push the element, false otherwise
         * */
        bool TryPush(T elem);

        /**
         * Tries to push an element to the list. Blocks if it needs to.
         *
         * @param elem The element to be pushed
         *
         * @returns true if it was able to push the element, false otherwise
         * */
        bool Push(T elem);

        /**
         * Pops a Job from the list
         *
         * Important: Only the "owner" thread of the buffer may pop elements.
         *
         * @param value A reference to where to store the result of the operation if successuful
         *
         * @returns true if it could pop a correct value, false if it couldn't
         * */
        bool Pop(T& value);

        bool Empty() const {
            return m_Head.load(std::memory_order_acquire) == m_Tail.load(std::memory_order_acquire);
        }

    private:
        /**
         * Assumes that the semaphore has been acquired and proceeds to push if it can.
         * This is obiosly thread unsafe if the semaphore hasn't been acquired.
         *
         * @returns true if it was able to push the element, false otherwise
         * */
        bool PushUnsafe(T elem);

        alignas(std::hardware_destructive_interference_size) std::atomic<u32> m_Head{0};
        alignas(std::hardware_destructive_interference_size) u32 m_HeadCached{0};

        alignas(std::hardware_destructive_interference_size) std::atomic<u32> m_Tail{0};
        alignas(std::hardware_destructive_interference_size) u32 m_TailCached{0};


        std::array<T, N> m_Buffer;
        static_assert((N & (N - 1)) == 0, "Size of the RingBuffer must be a power of 2");

        // Semaphore that ensures only one thread pushes at a time.
        std::binary_semaphore m_PushSemaphore;
    };


    template <typename T, std::size_t N>
    bool RingBuffer<T, N>::TryPush(T elem) {
        // There is already a thread trying to push, return
        if (!m_PushSemaphore.try_acquire())
            return false;

        return PushUnsafe(elem);
    }

    template <typename T, std::size_t N>
    bool RingBuffer<T, N>::Push(T elem) {
        // Blocks until acquires the semaphore
        m_PushSemaphore.acquire();

        return PushUnsafe(elem);
    }

    template <typename T, std::size_t N>
    bool RingBuffer<T, N>::PushUnsafe(T elem) {
        const u32 head = m_Head.load(std::memory_order_relaxed);
        u32 nextHead = (head + 1) & (N - 1);

        // The RingBuffer is full
        if (nextHead == m_TailCached) [[unlikely]] {
            m_TailCached = m_Tail.load(std::memory_order_acquire);
            if (nextHead == m_TailCached) {
                m_PushSemaphore.release();
                return false;
            }
        }

        m_Buffer[head] = std::move(elem);
        m_Head.store(nextHead, std::memory_order_release);

        m_PushSemaphore.release();
        return true;
    }

    template <typename T, std::size_t N>
    bool RingBuffer<T, N>::Pop(T& value) {
        const u32 tail = m_Tail.load(std::memory_order_relaxed);

        // The RingBuffer is empty
        if (tail == m_HeadCached) [[unlikely]] {
            m_HeadCached = m_Head.load(std::memory_order_acquire);
            if (tail == m_HeadCached)
                return false;
        }

        value = std::move(m_Buffer[tail]);
        u32 nextTail = (tail + 1) & (N - 1);
        m_Tail.store(nextTail, std::memory_order_release);
        return true;
    }
} // namespace cw

// ─────────────────────────────────────────────
// Job.hpp
// ─────────────────────────────────────────────




namespace cw {
    // Useful defines
    // TODO: Make this static values defined by the user
    inline static constexpr u32 BufferCapacity = 64;
    inline static constexpr u32 TagBufferCapacity = 256;

    using ThreadAffinity = u8;
    using Tag = u16;

    enum class JobPriority { Low = 0, Medium, High };
    template <typename T>
    using JobBufferPtr = std::unique_ptr<RingBuffer<T, BufferCapacity>>;
    template <typename T>
    using TagBufferPtr = std::unique_ptr<RingBuffer<T, TagBufferCapacity>>;

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
    struct MoveToThreadAwaiter;
    template <typename T>
    struct MoveToTagAwaiter;
    template <typename T>
    struct WaitOnTagAwaiter;
    struct TagWaitState;

    /**
     * This is the base Job sruct. Alls jobs derive from this.
     * */
    struct Job {
        JobPriority m_Priority{JobPriority::Medium};
        ThreadAffinity m_ThreadIndex{InvalidThreadIndex};
        bool m_IsFunction{true};

        Tag m_Tag{InvalidTag};
        TagWaitState* m_TagWaitState{nullptr};

        // Who is waiting for me (only on coroutines)
        Job* m_Parent{nullptr};
        // How many children I'm waiting for (only on coroutines)
        std::atomic<u32> m_Children{0};

#ifdef TRACY_ENABLE
        const char* m_DebugName{nullptr};
#endif // TRACY_ENABEL

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
#ifndef TRACY_ENABLE
        JobFunction(std::function<void()> func,
                    JobPriority priority = JobPriority::Medium,
                    ThreadAffinity threadIndex = InvalidThreadIndex,
                    Tag tag = InvalidTag)
            : m_Function(func),
              Job(priority, threadIndex, true, tag) {}
#else
        JobFunction(std::function<void()> func,
                    JobPriority priority = JobPriority::Medium,
                    ThreadAffinity threadIndex = InvalidThreadIndex,
                    Tag tag = InvalidTag,
                    const char* name = nullptr)
            : m_Function(func),
              Job(priority, threadIndex, true, tag) {
            m_DebugName = name;
        }
#endif // !TRACY_ENABLE

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
    inline WhenAllTag<Us...> WhenAll(ThreadAffinity affinity, JobCoroutine<Us>&&... coros) {
        return {std::tuple<JobCoroutine<Us>...>(std::move(coros)...), affinity};
    }

    template <typename... Us>
    inline WhenAllTag<Us...> WhenAll(JobCoroutine<Us>&&... coros) {
        return {std::tuple<JobCoroutine<Us>...>(std::move(coros)...), InvalidThreadIndex};
    }

    struct MoveToThreadTag {
        ThreadAffinity m_Thread;
    };

    inline MoveToThreadTag MoveToThread(ThreadAffinity thread) {
        return MoveToThreadTag{thread};
    }

    struct MoveToTagTag {
        Tag m_Tag;
    };

    inline MoveToTagTag MoveToTag(Tag tag) {
        return MoveToTagTag{tag};
    }

    struct WaitOnTagTag {
        Tag m_Tag;
    };

    inline WaitOnTagTag WaitOnTag(Tag tag) {
        return WaitOnTagTag{tag};
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

        MoveToThreadAwaiter<T> await_transform(MoveToThreadTag&& tag) {
            return MoveToThreadAwaiter<T>(tag.m_Thread);
        }

        MoveToTagAwaiter<T> await_transform(MoveToTagTag&& tag) {
            return MoveToTagAwaiter<T>(tag.m_Tag);
        }

        WaitOnTagAwaiter<T> await_transform(WaitOnTagTag&& tag) {
            return WaitOnTagAwaiter<T>(tag.m_Tag);
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

    struct TagWaitState {
        // jobs still in-flight under this tag
        std::atomic<u32> m_PendingJobs{0};
        std::vector<Job*> m_Waiters;
        std::mutex m_WaitersMutex;
    };
} // namespace cw

#ifdef TRACY_ENABLE
#    include <tracy/Tracy.hpp>
#endif // TRACY_ENABLE

namespace cw {
    struct TagAux {
        TagBufferPtr<Job*> m_Jobs;
        std::unique_ptr<TagWaitState> m_Await;
    };


    class JobSystem {
    public:
        JobSystem(const JobSystem& other) = delete;
        JobSystem& operator=(const JobSystem&) = delete;

        // Constructors do nothing because the initialization/destruction is manual
        // with Init/ShutDown
        JobSystem() {}
        ~JobSystem() {}

        /**
         * Simple method for initialising the system. Shall not be called more than
         * once.
         *
         * @param threadCount How many working threads you need? The threads you
         * specify will be fully managed by the system. You can later convert already
         * running threads to worker ones but those won't automatically be managed by
         * the system.
         *
         * This function is NOT thread safe so it must be called from only one thread
         * and only once to be safe.
         * */
        static void Init(ThreadAffinity threadCount) {
            if (s_Instance != nullptr) {
                return;
            }

            ThreadAffinity totalThreads = std::thread::hardware_concurrency();
            // FIX: This error message is temporal, in such cases simply prevent the
            // initialization or something
            CW_ENSURE(totalThreads > 0, "There are not sufficient threads to initialize the job system");
            CW_ENSURE(threadCount <= MaxThreads, "Can't spawn more than {0} worker threads", MaxThreads);

            s_Instance = std::make_unique<JobSystem>();
            JobSystem& js = *s_Instance;

            js.m_NumThreads.store(std::min(totalThreads, threadCount));
            js.m_LargestAvailableIndex.store(js.m_NumThreads.load());
            js.m_Running.store(true, std::memory_order_seq_cst);

            // Create local buffers
            for (ThreadAffinity i = 0; i < js.m_NumThreads; ++i) {
                js.m_JobLocalBuffers[i] = std::make_unique<RingBuffer<Job*, BufferCapacity>>();
            }

            // Allocate all mutexes and condition variables
            for (ThreadAffinity i = 0; i < js.m_NumThreads.load(); ++i) {
                js.m_CVsMutex[i] = std::make_unique<std::mutex>();
                js.m_CVs[i] = std::make_unique<std::condition_variable>();
            }

            // Spawn worker threads
            for (ThreadAffinity i = 0; i < js.m_NumThreads.load(); ++i) {
                js.m_Threads.emplace_back([i, &js]() {
                    js.SetupWorkerThread(i);
                    js.WorkerLoop();
                });
            }
        }

        /**
         * Same as Init but for destroying and cleaning everything.
         * This function is NOT thread safe so it must be called from only one thread
         * and only once to be safe.
         * */
        static void Shutdown() {
            if (s_Instance == nullptr) {
                return;
            }

            // Signal all threads to execute everything that remains and exit
            s_Instance->m_Running.store(false, std::memory_order_seq_cst);
            for (ThreadAffinity i = 0; i < s_Instance->m_LargestAvailableIndex.load(); ++i) {
                std::unique_lock lock(*(s_Instance->m_CVsMutex[i]));
                s_Instance->m_CVs[i]->notify_all();
            }

            // Just join the threads the job system owns (it may not be as many as the
            // m_NumThreads variable's value)
            for (std::thread& thread : s_Instance->m_Threads) {
                if (thread.joinable())
                    thread.join();
            }

            // Wait until all non-owned workers have been unregistered
            ThreadAffinity active = s_Instance->m_NumThreads.load(std::memory_order_acquire);
            while (active != 0) {
                s_Instance->m_NumThreads.wait(active, std::memory_order_acquire);
                active = s_Instance->m_NumThreads.load(std::memory_order_acquire);
            }

            s_Instance.reset();
        }

#ifndef TRACY_ENABLE
        /**
         * Converts the calling thread into a worker one. If the calling thread is
         * already a worker panics.
         *
         * The converted thread will have to manually call the corresponding methods
         * to work as because the system itslef doesn't own the thread it can't manage
         * it.
         *
         * @returns The thread ID of the newnly created worker
         *
         * Thread Safe
         * */
        static ThreadAffinity ConvertToWorkerThread() {
            return s_Instance->ConvertToWorkerThreadImpl();
        }
#else
        /**
         * Converts the calling thread into a worker one. If the calling thread is
         * already a worker panics.
         *
         * The converted thread will have to manually call the corresponding methods
         * to work as because the system itslef doesn't own the thread it can't manage
         * it.
         *
         * @param name Debug name of the thread
         *
         * @returns The thread ID of the newnly created worker
         *
         * Thread Safe
         * */
        static ThreadAffinity ConvertToWorkerThread(const char* name) {
            return s_Instance->ConvertToWorkerThreadImpl(name);
        }
#endif // !TARCY_ENABLE

        /**
         * Assumes the calling thread has already been converted to a worker, if not
         * it will panic. Deregisters the calling worker thread from being a worker.
         * This action is necessary to be performed by all external workers so the
         * system can shutdown correctly.
         *
         * Thread Safe
         * */
        static void DeregisterWorkerThread() {
            s_Instance->DeregisterWorkerThreadImpl();
        }

#ifndef TRACY_ENABLE
        /**
         * Schedules a simple function
         *
         * @param job The function/lamda/... to schedule
         * @param priority The priority the job will have (medium by default)
         * @param threadId The thread you want this job to be executed on. If assigned
         * then priotity is ignored.
         *
         * Thread Safe
         * */
        static void Schedule(std::function<void()> job,
                             JobPriority priority = JobPriority::Medium,
                             ThreadAffinity threadId = InvalidThreadIndex,
                             Tag tag = InvalidTag) {
            s_Instance->ScheduleImpl(job, priority, threadId, tag);
        }
#else
        /**
         * Schedules a simple function
         *
         * @param job The function/lamda/... to schedule
         * @param priority The priority the job will have (medium by default)
         * @param threadId The thread you want this job to be executed on. If assigned
         * then priotity is ignored.
         * @param name Debug for the job
         *
         * Thread Safe
         * */
        static void Schedule(std::function<void()> job,
                             JobPriority priority = JobPriority::Medium,
                             ThreadAffinity threadId = InvalidThreadIndex,
                             Tag tag = InvalidTag,
                             const char* name = nullptr) {
            s_Instance->ScheduleImpl(job, priority, threadId, tag, name);
        }
#endif // !TRACY_ENABLE

#ifndef TRACY_ENABLE
        /**
         * Schedules the given job coroutine
         *
         * Important: Jobs scheduled with the same priority in a specify order are not
         * necessarly going to mantain that order when executed. If
         * order/synchronization is important then you can add coroutines as
         * dependencies of a job by waiting on them with the co_await operator.
         *
         * @param job A reference to the job coroutine
         * @param threadId The thread you want to job to run on. If set then priority
         * is ignored (it doesn't matter by default)
         * @param priority The priority of the given job (medium by default)
         *
         * Thread Safe
         * */
        template <typename T>
        static void Schedule(JobCoroutine<T>& job,
                             ThreadAffinity threadId = InvalidThreadIndex,
                             JobPriority priority = JobPriority::Medium,
                             Tag tag = InvalidTag) {
            s_Instance->ScheduleImpl(job, threadId, priority, tag);
        }
#else
        /**
         * Schedules the given job coroutine
         *
         * Important: Jobs scheduled with the same priority in a specify order are not
         * necessarly going to mantain that order when executed. If
         * order/synchronization is important then you can add coroutines as
         * dependencies of a job by waiting on them with the co_await operator.
         *
         * @param job A reference to the job coroutine
         * @param threadId The thread you want to job to run on. If set then priority
         * is ignored (it doesn't matter by default)
         * @param priority The priority of the given job (medium by default)
         * @param name Debug name for the job
         *
         * Thread Safe
         * */
        template <typename T>
        static void Schedule(JobCoroutine<T>& job,
                             ThreadAffinity threadId = InvalidThreadIndex,
                             JobPriority priority = JobPriority::Medium,
                             Tag tag = InvalidTag,
                             const char* name = nullptr) {
            s_Instance->ScheduleImpl(job, threadId, priority, tag, name);
        }
#endif // !TRACY_ENABLE

        /**
         * Schedules all the jobs assigned to the specified tag. The jobs are scheduled according to the parameters
         * passed when scheduling them (thread affinity, priority, etc).
         *
         * @param tag The tag to schedule
         *
         * Thread safe
         * */
        static void ScheduleTag(Tag tag) {
            s_Instance->ScheduleTagImpl(tag);
        }

        /**
         * Assumes the calling thread has already been converted to a worker, if not
         * it will panic. The caller thread will work until the the stop_source
         * requests a stop. Stopping may take some time as it may happen that the
         * thread is currently executing a big job when the condition was set to
         * false.
         *
         * This method doesn't deregister the worker thread automatically, this means
         * that you can call it as many times as you want but it also means that you
         * will have to call DeregisterWorkerThread manually before shutting down the
         * system.
         *
         * @param stopToken The stop_token the thread will check in order to stop
         * excecution
         *
         * Thread Safe
         * */
        static void RunWorkerUntil(std::stop_token stopToken) {
            s_Instance->RunWorkerUntilImpl(stopToken);
        }

        /**
         * Assumes the calling thread has already been converted to a worker, if not
         * it will panic. The caller thread will work for the given time. It's not
         * guaranted to return exactly after the specified amount of time becase it
         * may happen that the thread is currently executing a big job when the
         * condition was set to false.
         *
         * This method doesn't deregister the worker thread automatically, this means
         * that you can call it as many times as you want but it also means that you
         * will have to call DeregisterWorkerThread manually before shutting down the
         * system.
         *
         * @param time The amount of time the thread will work for, in milliseconds
         *
         * Thread Safe
         * */
        static void RunWorkerFor(std::chrono::milliseconds time) {
            s_Instance->RunWorkerForImpl(time);
        }

        /**
         * Retrieves how many worker thread are there.
         *
         * @returns The number of worker threads currently working.
         *
         * Thread Safe
         * */
        static ThreadAffinity GetNumThreads() {
            return s_Instance->m_NumThreads.load(std::memory_order_acquire);
        }

        /**
         * Simple method for retreaving the index/id of the calling thread. If the calling thread is not a worker then
         * it's undifined behavior.
         *
         * @returns The thread index/id of the calling thread
         * */
        static ThreadAffinity GetThreadIndex() {
            return m_Index;
        }

#ifdef CW_TESTING
        const std::vector<std::thread>& GetThreadsDEBUG() const {
            return m_Threads;
        }
        u64 GetLocalBufferNumDEBUG() const {
            return m_LargestAvailableIndex.load(std::memory_order_acquire);
        }

#endif // CW_TESTING

    private:
        template <typename T>
        friend struct FinalAwaiter;

        template <typename T, typename... Us>
        friend struct JobAwaiterMultiple;

        template <typename T>
        friend struct MoveToThreadAwaiter;

        template <typename T>
        friend struct MoveToTagAwaiter;

        template <typename T>
        friend struct WaitOnTagAwaiter;

        // Static methods implementations

#ifndef TRACY_ENABLE
        ThreadAffinity ConvertToWorkerThreadImpl() {
            CW_ENSURE(m_Index == InvalidThreadIndex, "Trying to convert an already worker thread");

            ThreadAffinity newIndex;
            {
                std::scoped_lock lock(m_ExternalWorkerMutex);

                // Update thread count
                ThreadAffinity threads = m_NumThreads.fetch_add(1, std::memory_order_acq_rel);
                CW_ENSURE(threads <= MaxThreads, "Can't spawn more than {0} worker threads", MaxThreads);

                // Get new thread ID
                if (m_AvailableIndexes.empty()) {
                    newIndex = m_LargestAvailableIndex.fetch_add(1, std::memory_order_acq_rel);
                    // This check is technically not necessary because we alread have the
                    // one above
                    CW_ENSURE(newIndex < MaxThreads, "Can't spawn more than {0} worker threads", MaxThreads);

                    // Create local buffer
                    m_JobLocalBuffers[newIndex] = std::make_unique<RingBuffer<Job*, BufferCapacity>>();

                    // Allocate mutex and condition
                    m_CVsMutex[newIndex] = std::make_unique<std::mutex>();
                    m_CVs[newIndex] = std::make_unique<std::condition_variable>();
                } else {
                    newIndex = m_AvailableIndexes.top();
                    m_AvailableIndexes.pop();

                    // Here the thread will use another's thread mutex, CV and local buffer
                    // (it's guaranted that a deregistered thread will clean everything
                    // before leaving)
                }
            }

            SetupWorkerThread(newIndex);
            return newIndex;
        }
#else
        ThreadAffinity ConvertToWorkerThreadImpl(const char* name) {
            CW_ENSURE(m_Index == InvalidThreadIndex, "Trying to convert an already worker thread");

            ThreadAffinity newIndex;
            {
                std::scoped_lock lock(m_ExternalWorkerMutex);

                // Update thread count
                ThreadAffinity threads = m_NumThreads.fetch_add(1, std::memory_order_acq_rel);
                CW_ENSURE(threads <= MaxThreads, "Can't spawn more than {0} worker threads", MaxThreads);

                // Get new thread ID
                if (m_AvailableIndexes.empty()) {
                    newIndex = m_LargestAvailableIndex.fetch_add(1, std::memory_order_acq_rel);
                    // This check is technically not necessary because we alread have the
                    // one above
                    CW_ENSURE(newIndex < MaxThreads, "Can't spawn more than {0} worker threads", MaxThreads);

                    // Create local buffer
                    m_JobLocalBuffers[newIndex] = std::make_unique<RingBuffer<Job*, BufferCapacity>>();

                    // Allocate mutex and condition
                    m_CVsMutex[newIndex] = std::make_unique<std::mutex>();
                    m_CVs[newIndex] = std::make_unique<std::condition_variable>();
                } else {
                    newIndex = m_AvailableIndexes.top();
                    m_AvailableIndexes.pop();

                    // Here the thread will use another's thread mutex, CV and local buffer
                    // (it's guaranted that a deregistered thread will clean everything
                    // before leaving)
                }
            }

            SetupWorkerThread(newIndex, name);
            return newIndex;
        }
#endif // !TARCY_ENABLE

        void DeregisterWorkerThreadImpl() {
            CW_ENSURE(m_Index != InvalidThreadIndex, "The calling thread is not a worker");

            // Finish everything that remains
            EmptyLocalBuffer();

            // Mark the thread index as busy so no job is assigned to it
            m_IdleThreads.fetch_and(~(1ULL << m_Index), std::memory_order_release);

            {
                std::scoped_lock lock(m_ExternalWorkerMutex);

                // This thread index is now free
                m_AvailableIndexes.push(m_Index);
                m_Index = InvalidThreadIndex;
            }

            m_NumThreads.fetch_sub(1, std::memory_order_release);
            // If shutdown is currently being called then notify that this thread has
            // been deregistered succesfully
            m_NumThreads.notify_all();
        }

#ifndef TRACY_ENABLE
        void ScheduleImpl(std::function<void()> job,
                          JobPriority priority = JobPriority::Medium,
                          ThreadAffinity threadId = InvalidThreadIndex,
                          Tag tag = InvalidTag) {
            // Functions shall never have a parent
            Schedule(new JobFunction(job, priority, threadId, tag), nullptr);
        }
#else
        void ScheduleImpl(std::function<void()> job,
                          JobPriority priority = JobPriority::Medium,
                          ThreadAffinity threadId = InvalidThreadIndex,
                          Tag tag = InvalidTag,
                          const char* name = nullptr) {
            // Functions shall never have a parent
            Schedule(new JobFunction(job, priority, threadId, tag, name), nullptr);
        }
#endif // !TRACY_ENABLE

#ifndef TRACY_ENABLE
        template <typename T>
        void ScheduleImpl(JobCoroutine<T>& job,
                          ThreadAffinity threadId = InvalidThreadIndex,
                          JobPriority priority = JobPriority::Medium,
                          Tag tag = InvalidTag) {
            // The underlying job promise
            JobPromise<T>* jobProm = &job.GetHandle().promise();

            // Assign given values to the promise
            jobProm->m_Priority = priority;
            jobProm->m_ThreadIndex = threadId;
            jobProm->m_Tag = tag;

            // Notify the coroutine object that the job has been scheduled
            job.Schedule();
            // No parent will be set (top-level schedule), null the handle
            // so the destructor can't touch the freed frame after FinalAwaiter runs
            job.ReleaseHandle();

            // Top level coroutines can't have a parent
            Schedule(jobProm, nullptr);
        }
#else
        template <typename T>
        void ScheduleImpl(JobCoroutine<T>& job,
                          ThreadAffinity threadId = InvalidThreadIndex,
                          JobPriority priority = JobPriority::Medium,
                          Tag tag = InvalidTag,
                          const char* name = nullptr) {
            // The underlying job promise
            JobPromise<T>* jobProm = &job.GetHandle().promise();

            // Assign given values to the promise
            jobProm->m_Priority = priority;
            jobProm->m_ThreadIndex = threadId;
            jobProm->m_Tag = tag;
            jobProm->m_DebugName = name;

            // Notify the coroutine object that the job has been scheduled
            job.Schedule();
            // No parent will be set (top-level schedule), null the handle
            // so the destructor can't touch the freed frame after FinalAwaiter runs
            job.ReleaseHandle();

            // Top level coroutines can't have a parent
            Schedule(jobProm, nullptr);
        }
#endif // !TRACY_ENABLE

        void ScheduleTagImpl(Tag tag) {
            if (tag == InvalidTag)
                return;

            std::vector<Job*> toSchedule;

            {
                std::shared_lock lock(m_TagBuffersMutex);

                // The tag doesn't exist
                if (!m_TagBuffers.contains(tag))
                    return;

                Job* job;
                while (m_TagBuffers.at(tag).m_Jobs->Pop(job))
                    toSchedule.push_back(job);
            }

            for (Job* job : toSchedule)
                Schedule(job, job->m_Parent);
        }

        void RunWorkerUntilImpl(std::stop_token stopToken) {
            CW_ENSURE(m_Index != InvalidThreadIndex, "The calling thread is not a worker");

            // We lock here because:
            // https://stackoverflow.com/questions/38147825/shared-atomic-variable-is-not-properly-published-if-it-is-not-modified-under-mut
            std::unique_lock lock(*m_CVsMutex[m_Index]);

            // The caller of the callback has to wake up the current worker thread, so
            // we need to capture its index
            ThreadAffinity index = m_Index;
            std::stop_callback callback(stopToken, [this, index] { m_CVs[index]->notify_all(); });

            while (m_Running.load(std::memory_order_acquire) && !stopToken.stop_requested()) {
                // Update idle bit mask
                m_IdleThreads.fetch_or(1ULL << m_Index, std::memory_order_release);
                m_CVs[m_Index]->wait(lock, [&] {
                    return !m_Running.load(std::memory_order_acquire) ||
                           stopToken.stop_requested() // Wake up to shutdown job system
                                                      // Check local buffers
                           || !m_JobLocalBuffers[m_Index]->Empty() ||
                           // Check priority buffers
                           m_JobBuffers[2].size_approx() > 0 || m_JobBuffers[1].size_approx() > 0 ||
                           m_JobBuffers[0].size_approx() > 0;
                });
                // Update idle bit mask
                m_IdleThreads.fetch_and(~(1ULL << m_Index), std::memory_order_release);

                if (!m_Running.load(std::memory_order_acquire) || stopToken.stop_requested())
                    break;

                lock.unlock();
                // Run job
                RunPendingJob();
                lock.lock();
            }

            // Finish everything that remains if the system is shutting down
            if (!m_Running.load(std::memory_order_acquire))
                EmptyLocalBuffer();

            // Mark the thread index as busy so no job is assigned to it
            m_IdleThreads.fetch_and(~(1ULL << m_Index), std::memory_order_release);
        }

        void RunWorkerForImpl(std::chrono::milliseconds time) {
            CW_ENSURE(m_Index != InvalidThreadIndex, "The calling thread is not a worker");

            auto start = std::chrono::steady_clock::now();
            auto spent = std::chrono::steady_clock::now() - start;

            // We lock here because:
            // https://stackoverflow.com/questions/38147825/shared-atomic-variable-is-not-properly-published-if-it-is-not-modified-under-mut
            std::unique_lock lock(*m_CVsMutex[m_Index]);

            while (m_Running.load(std::memory_order_acquire) && spent < time) {
                // Update idle bit mask
                m_IdleThreads.fetch_or(1ULL << m_Index, std::memory_order_release);
                m_CVs[m_Index]->wait_for(lock, time - spent, [&] {
                    return !m_Running.load(std::memory_order_acquire) ||
                           // system Check local buffers
                           !m_JobLocalBuffers[m_Index]->Empty() ||
                           // Check priority buffers
                           m_JobBuffers[2].size_approx() > 0 || m_JobBuffers[1].size_approx() > 0 ||
                           m_JobBuffers[0].size_approx() > 0;
                });
                // Update idle bit mask
                m_IdleThreads.fetch_and(~(1ULL << m_Index), std::memory_order_release);

                if (!m_Running.load(std::memory_order_acquire) || !(spent < time))
                    break;

                lock.unlock();
                // Run job
                RunPendingJob();
                lock.lock();

                spent = std::chrono::steady_clock::now() - start;
            }

            // Finish everything that remains if the system is shutting down
            if (!m_Running.load(std::memory_order_acquire))
                EmptyLocalBuffer();

            // Mark the thread index as busy so no job is assigned to it
            m_IdleThreads.fetch_and(~(1ULL << m_Index), std::memory_order_release);
        }

        /**
         * Simply gets the singleton instance. Call after initialization.
         *
         * Thread Safe
         * */
        inline static JobSystem& GetInstance() noexcept {
            return *s_Instance;
        }

        /**
         * Returns how many jobs does the tag have. Assumes the tag is already created, if not it's undifined behavior.
         *
         * @param tag The tag to check
         *
         * @returns The numbers of jobs assigned to the given tag
         *
         * Thread safe
         * */
        u32 TagPendingCount(Tag tag) {
            std::shared_lock lock(m_TagBuffersMutex);

            if (!m_TagBuffers.contains(tag))
                return 0;

            return m_TagBuffers.at(tag).m_Await->m_PendingJobs.load(std::memory_order_acquire);
        }

        void NotifyTagWaiters(Job* job) {
            if (job->m_TagWaitState != nullptr) {
                u32 remaining = job->m_TagWaitState->m_PendingJobs.fetch_sub(1, std::memory_order_acq_rel);

                if (remaining == 1) {
                    // last job in the tag batch finished — wake all waiters
                    std::vector<Job*> waiters;
                    {
                        std::scoped_lock lock(job->m_TagWaitState->m_WaitersMutex);
                        waiters = std::move(job->m_TagWaitState->m_Waiters);
                    }

                    for (Job* waiter : waiters)
                        JobSystem::GetInstance().Schedule(waiter, waiter->m_Parent);
                }
            }
        }

        /**
         * Internal schedule method called by all the public ones
         * It's assumed that all job parameters are already set inside the job struct
         *
         * @param job The job to schedule (either a function or a coroutine)
         * @param parent Which job the scheduled one as a dependency. Functions cannot
         * have a parent
         * */
        void Schedule(Job* job, Job* parent) {
            CW_ENSURE(parent == nullptr || !parent->m_IsFunction, "Functions are not allowed to be parents.");

            // Set parent
            job->m_Parent = parent;

            Tag tag = job->m_Tag;
            // Schedule the job to a specific tag if it has one
            if (tag != InvalidTag) {
                // Fast path: tag already exists
                {
                    std::shared_lock lock(m_TagBuffersMutex);

                    if (m_TagBuffers.contains(tag)) {
                        // Invalidate tag so that next time it's scheduled excecutes
                        job->m_Tag = InvalidTag;

                        job->m_TagWaitState = m_TagBuffers.at(tag).m_Await.get();
                        job->m_TagWaitState->m_PendingJobs.fetch_add(1, std::memory_order_release);

                        CW_ENSURE(m_TagBuffers.at(tag).m_Jobs->Push(job), "...");
                        return;
                    }
                }

                // Slow path: need to create it
                {
                    std::unique_lock lock(m_TagBuffersMutex);

                    // We double check because some thread might have already crated the tag buffer
                    if (!m_TagBuffers.contains(tag)) {
                        m_TagBuffers[tag] = {std::make_unique<RingBuffer<Job*, TagBufferCapacity>>(),
                                             std::make_unique<TagWaitState>()};
                    }

                    // Invalidate tag so that next time it's scheduled excecutes
                    job->m_Tag = InvalidTag;

                    job->m_TagWaitState = m_TagBuffers.at(tag).m_Await.get();
                    job->m_TagWaitState->m_PendingJobs.fetch_add(1, std::memory_order_release);

                    CW_ENSURE(m_TagBuffers.at(tag).m_Jobs->Push(job), "...");
                    return;
                }
            }

            // The thread matters
            if (job->m_ThreadIndex != InvalidThreadIndex) {
                CW_ENSURE(job->m_ThreadIndex < m_LargestAvailableIndex.load(std::memory_order_acquire),
                          "Can't assign a job to a non existing thread");
                CW_ENSURE(m_JobLocalBuffers[job->m_ThreadIndex]->Push(job),
                          "The local RingBuffer of thread {0} is full",
                          job->m_ThreadIndex);

                // Notify the wanted thread
                std::scoped_lock lock(*m_CVsMutex[job->m_ThreadIndex]);
                m_CVs[job->m_ThreadIndex]->notify_all();
            }
            // Thread is irrelevant
            else {
                m_JobBuffers[static_cast<u8>(job->m_Priority)].enqueue(job);

                // Find idle thread via bitmask
                u64 idle = m_IdleThreads.load(std::memory_order_acquire);
                if (idle == 0)
                    return; // all busy, they'll pick it up when done

                int index = std::countr_zero(idle);
                {
                    std::scoped_lock lock(*m_CVsMutex[index]);
                    m_CVs[index]->notify_all();
                }
            }
        }

#ifndef TRACY_ENABLE
        /**
         * Called within a thread to setup the worker's thread related config.
         *
         * @param threadId Which id will the thread have?
         * */
        void SetupWorkerThread(ThreadAffinity threadId) {
            m_Index = threadId;
        }
#else
        /**
         * Called within a thread to setup the worker's thread related config.
         *
         * @param threadId Which id will the thread have?
         * @param name Debug name for the thread
         * */
        void SetupWorkerThread(ThreadAffinity threadId, const char* name = nullptr) {
            m_Index = threadId;

            if (name) {
                tracy::SetThreadName(name);
            } else {
                char buf[32];
                std::snprintf(buf, sizeof(buf), "CW Worker %u", threadId);
                tracy::SetThreadName(buf);
            }
        }
#endif // !TRACY_ENABLE

        /**
         * Function that contains the loop that every worker thread has to run
         * continuously.
         * */
        void WorkerLoop() {
            // We lock here because:
            // https://stackoverflow.com/questions/38147825/shared-atomic-variable-is-not-properly-published-if-it-is-not-modified-under-mut
            std::unique_lock lock(*m_CVsMutex[m_Index]);

            while (m_Running.load(std::memory_order_acquire)) {
                // Update idle bit mask
                m_IdleThreads.fetch_or(1ULL << m_Index, std::memory_order_release);
                m_CVs[m_Index]->wait(lock, [this] {
                    return !m_Running.load(std::memory_order_acquire) // Wake up to shutdown job system
                                                                      // Check local buffers
                           || !m_JobLocalBuffers[m_Index]->Empty() ||
                           // Check priority buffers
                           m_JobBuffers[2].size_approx() > 0 || m_JobBuffers[1].size_approx() > 0 ||
                           m_JobBuffers[0].size_approx() > 0;
                });
                // Update idle bit mask
                m_IdleThreads.fetch_and(~(1ULL << m_Index), std::memory_order_release);

                if (!m_Running.load(std::memory_order_acquire))
                    break;

                lock.unlock();
                // Run job
                RunPendingJob();
                lock.lock();
            }

            // Finish everything that remains so the thread can join
            EmptyAllBuffers();
            m_NumThreads.fetch_sub(1, std::memory_order_release);
        }

        /**
         * Attemps to execute a job if possible.
         * */
        void RunPendingJob() {
            Job* job;

            // First check local buffers
            bool jobLocalExp = m_JobLocalBuffers[m_Index]->Pop(job);
            if (jobLocalExp) {
                RunJob(job);
                return;
            }

            // Check global priority buffers in order of priority
            for (int i = 2; i >= 0; --i) {
                if (m_JobBuffers[i].try_dequeue(job)) {
                    RunJob(job);
                    return;
                }
            }
        }

        /**
         * Executes the given job on the current thread.
         *
         * @param job The job to execute
         * */
        void RunJob(Job* job) {
            bool isFunction = job->m_IsFunction;

#ifdef TRACY_ENABLE
            if (!isFunction && job->m_DebugName) {
                TracyFiberEnter(job->m_DebugName);
                job->Resume();
                TracyFiberLeave;
            } else if (isFunction) {
                ZoneScoped;
                job->Resume();
            } else {
                job->Resume();
            }
#else
            job->Resume();
#endif // TRACY_ENABLE

            // Delete the function job since it can't possibly be rescheduled
            if (isFunction) {
                // Wait tag logic (the coroutines one already happens in the final awaiter)
                NotifyTagWaiters(job);

                delete job;
            }
        }

        /**
         * The calling worker thread will empty all the jobs from its local buffer and
         * global ones
         * */
        void EmptyAllBuffers() {
            // Empty local buffer
            EmptyLocalBuffer();

            // Empty global queues
            Job* job;
            for (int i = 2; i >= 0; --i) {
                // Loop until all jobs of the queue are completed
                while (m_JobBuffers[i].size_approx() != 0) {
                    if (m_JobBuffers[i].try_dequeue(job)) {
                        RunJob(job);
                    }
                }
            }
        }

        /**
         * The calling worker thread will empty all the jobs from its local buffer
         * */
        void EmptyLocalBuffer() {
            Job* job;
            // Empty local buffer
            while (!m_JobLocalBuffers[m_Index]->Empty()) {
                bool jobLocalExp = m_JobLocalBuffers[m_Index]->Pop(job);
                if (jobLocalExp) {
                    RunJob(job);
                }
            }
        }

        /// The JobSystem singleton
        inline static std::unique_ptr<JobSystem> s_Instance = nullptr;

        /// Indicates if the system is currently running or not
        std::atomic_bool m_Running{false};
        std::atomic<ThreadAffinity> m_NumThreads;

        // Allows adding new mutexes and CVs so that new worker threads can be
        // registeted
        std::mutex m_ExternalWorkerMutex;
        /// A counter that stores the largest available index
        std::atomic<ThreadAffinity> m_LargestAvailableIndex{0};
        /// Contains the deleted indexes that are available once again to be used
        std::priority_queue<ThreadAffinity, std::vector<ThreadAffinity>, std::greater<ThreadAffinity>>
            m_AvailableIndexes;

        // Buffers
        std::array<moodycamel::ConcurrentQueue<Job*>, 3> m_JobBuffers;
        std::array<JobBufferPtr<Job*>, MaxThreads> m_JobLocalBuffers{};

        std::unordered_map<Tag, TagAux> m_TagBuffers;
        std::shared_mutex m_TagBuffersMutex;

        // The owned threads of the system
        std::vector<std::thread> m_Threads;

        // Synchronization types
        std::array<std::unique_ptr<std::condition_variable>, MaxThreads> m_CVs;
        std::array<std::unique_ptr<std::mutex>, MaxThreads> m_CVsMutex;

        /// Indicates which index does the current thread have
        inline static thread_local ThreadAffinity m_Index = InvalidThreadIndex;

        // Idle thread bitmask: 0 on an index if thread currently busy, 1 if thread
        // idle
        std::atomic<u64> m_IdleThreads{0};
    };

    /**
     * Checks if the coroutine has a parent and if that's the case notify it of the
     * completed job. Because the parent might need the return value don't destroy
     * the coroutine yet.
     *
     * If there is no parent then the coro destroys itself.
     * */
    template <typename T>
    struct FinalAwaiter : public std::suspend_always {
        bool await_suspend(std::coroutine_handle<JobPromise<T>> handle) noexcept {
            Job* self = &handle.promise();
            Job* parent = self->m_Parent;

            // Wait tag logic
            JobSystem::GetInstance().NotifyTagWaiters(self);

            // Parent dependency logic
            if (parent != nullptr) {
                u32 remaining = parent->m_Children.fetch_sub(1, std::memory_order_acq_rel);
                if (remaining == 1) {
                    // We were the last child, wake parent up
                    JobSystem::GetInstance().Schedule(parent, parent->m_Parent);
                }
                return true; // stay alive, parent will destroy us after reading value
            }
            return false; // no parent, self-destruct
        }
    };

    template <typename T>
    struct WaitOnTagAwaiter {
        Tag m_Tag;

        WaitOnTagAwaiter<T>(Tag tag)
            : m_Tag(tag) {}

        bool await_ready() noexcept {
            return false; // always go through await_suspend to avoid a race condition
        }

        bool await_suspend(std::coroutine_handle<JobPromise<T>> h) noexcept {
            Job* job = &h.promise();

            std::shared_lock sharedLock(JobSystem::GetInstance().m_TagBuffersMutex);

            // The tag doesn't exist, we dont have to wait
            if (!JobSystem::GetInstance().m_TagBuffers.contains(m_Tag))
                return false;

            TagWaitState* state = JobSystem::GetInstance().m_TagBuffers.at(m_Tag).m_Await.get();

            std::scoped_lock lock(state->m_WaitersMutex);

            if (state->m_PendingJobs.load(std::memory_order_acquire) == 0) {
                return false; // already drained, resume immediately
            }

            state->m_Waiters.push_back(job);
            return true; // genuinely suspend
        }

        void await_resume() noexcept {}
    };

    /**
     * Allows a coroutine to decide on which thread to continue executing by being
     * rescheduled again
     * */
    template <typename T>
    struct MoveToThreadAwaiter {
        ThreadAffinity m_Thread;

        MoveToThreadAwaiter<T>(ThreadAffinity thread)
            : m_Thread(thread) {}

        bool await_ready() noexcept {
            // Don't suspend if we are already on the wanted thread
            return JobSystem::GetInstance().m_Index == m_Thread;
        }

        void await_suspend(std::coroutine_handle<JobPromise<T>> h) noexcept {
            Job* job = &h.promise();
            job->m_ThreadIndex = m_Thread;

            JobSystem::GetInstance().Schedule(job, job->m_Parent);
        }

        void await_resume() noexcept {}
    };

    /**
     * Allows a coroutine to decide on which tag to continue executing by being
     * rescheduled again
     * */
    template <typename T>
    struct MoveToTagAwaiter {
        Tag m_Tag;

        MoveToTagAwaiter<T>(Tag tag)
            : m_Tag(tag) {}

        // Alaways suspend
        bool await_ready() noexcept {
            return false;
        }

        void await_suspend(std::coroutine_handle<JobPromise<T>> h) noexcept {
            Job* job = &h.promise();
            job->m_Tag = m_Tag;

            JobSystem::GetInstance().Schedule(job, job->m_Parent);
        }

        void await_resume() noexcept {}
    };

    /**
     * Allows a coroutine to wait on multiple coroutines
     * */
    template <typename T, typename... Us>
    struct JobAwaiterMultiple {
        std::tuple<JobCoroutine<Us>...> m_Coros;
        ThreadAffinity m_ThreadIndex;

        // Only accept rvalues for better convenience
        JobAwaiterMultiple(ThreadAffinity affinity, JobCoroutine<Us>&&... coros)
            : m_ThreadIndex(affinity),
              m_Coros(std::move(coros)...) {}

        // Delete move/copy for same reason as JobAwaiter
        JobAwaiterMultiple(const JobAwaiterMultiple&) = delete;
        JobAwaiterMultiple& operator=(const JobAwaiterMultiple&) = delete;
        JobAwaiterMultiple(JobAwaiterMultiple&&) = delete;
        JobAwaiterMultiple& operator=(JobAwaiterMultiple&&) = delete;

        // Alaways suspend
        bool await_ready() noexcept {
            return false;
        }

        void await_suspend(std::coroutine_handle<JobPromise<T>> h) noexcept {
            Job* parent = &h.promise();

            // Increment children
            parent->m_Children.fetch_add(sizeof...(Us), std::memory_order_release);

            // Schedule all children
            std::apply(
                [&](auto&... coros) {
                    // Set the same priority as the father
                    ((coros.GetHandle().promise().m_Priority = parent->m_Priority), ...);

                    // Set the custom affinity
                    ((coros.GetHandle().promise().m_ThreadIndex = m_ThreadIndex), ...);

                    (JobSystem::GetInstance().Schedule(&coros.GetHandle().promise(), parent), ...);
                },
                m_Coros);
        }

        auto await_resume() noexcept {
            return std::apply([&](auto&... coros) { return std::make_tuple(GetValue(coros)...); }, m_Coros);
        }

        // Helper to get value or void_t placeholder
        template <typename U>
        auto GetValue(JobCoroutine<U>& coro) {
            if constexpr (std::is_void_v<U>)
                return std::monostate{};
            else
                return coro.GetHandle().promise().Get();
        }
    };
} // namespace cw
