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

#include "Types.hpp"
#include "RingBuffer.hpp"
#include "Job.hpp"

namespace cw {
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
            if (s_JobSystem != nullptr) {
                return;
            }

            ThreadAffinity totalThreads = std::thread::hardware_concurrency();
            // FIX: This error message is temporal, in such cases simply prevent the
            // initialization or something
            CW_ENSURE(totalThreads > 0, "There are not sufficient threads to initialize the job system");
            CW_ENSURE(threadCount <= MaxThreads, "Can't spawn more than {0} worker threads", MaxThreads);

            s_JobSystem = std::make_unique<JobSystem>();
            JobSystem& js = *s_JobSystem;

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
            if (s_JobSystem == nullptr) {
                return;
            }

            // Signal all threads to execute everything that remains and exit
            s_JobSystem->m_Running.store(false, std::memory_order_seq_cst);
            for (ThreadAffinity i = 0; i < s_JobSystem->m_LargestAvailableIndex.load(); ++i) {
                std::unique_lock lock(*(s_JobSystem->m_CVsMutex[i]));
                s_JobSystem->m_CVs[i]->notify_all();
            }

            // Just join the threads the job system owns (it may not be as many as the
            // m_NumThreads variable's value)
            for (std::thread& thread : s_JobSystem->m_Threads) {
                if (thread.joinable())
                    thread.join();
            }

            // Wait until all non-owned workers have been unregistered
            ThreadAffinity active = s_JobSystem->m_NumThreads.load(std::memory_order_acquire);
            while (active != 0) {
                s_JobSystem->m_NumThreads.wait(active, std::memory_order_acquire);
                active = s_JobSystem->m_NumThreads.load(std::memory_order_acquire);
            }

            s_JobSystem.reset();
        }

        /**
         * Simply gets the singleton instance. Call after initialization.
         *
         * Thread Safe
         * */
        inline static JobSystem& GetInstance() noexcept {
            return *s_JobSystem;
        }

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
        ThreadAffinity ConvertToWorkerThread() {
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

        /**
         * Assumes the calling thread has already been converted to a worker, if not
         * it will panic. Deregisters the calling worker thread from being a worker.
         * This action is necessary to be performed by all external workers so the
         * system can shutdown correctly.
         *
         * Thread Safe
         * */
        void DeregisterWorkerThread() {
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
        void Schedule(std::function<void()> job,
                      JobPriority priority = JobPriority::Medium,
                      ThreadAffinity threadId = InvalidThreadIndex,
                      Tag tag = InvalidTag) {
            // Functions shall never have a parent
            Schedule(new JobFunction(job, priority, threadId, tag), nullptr);
        }

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
        void Schedule(JobCoroutine<T>& job,
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

        /**
         * Schedules all the jobs assigned to the specified tag. The jobs are scheduled according to the parameters
         * passed when scheduling them (thread affinity, priority, etc).
         *
         * @param tag The tag to schedule
         *
         * Thread safe
         * */
        void ScheduleTag(Tag tag) {
            if (tag == InvalidTag)
                return;

            std::vector<Job*> toSchedule;

            {
                std::shared_lock lock(m_TagBuffersMutex);

                // The tag doesn't exist
                if (!m_TagBuffers.contains(tag))
                    return;

                Job* job;
                while (m_TagBuffers.at(tag)->Pop(job))
                    toSchedule.push_back(job);
            }

            for (Job* job : toSchedule)
                Schedule(job, nullptr);
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
        void RunWorkerUntil(std::stop_token stopToken) {
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
        void RunWorkerFor(std::chrono::milliseconds time) {
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
         * Retrieves how many worker thread are there.
         *
         * @returns The number of worker threads currently working.
         *
         * Thread Safe
         * */
        ThreadAffinity GetNumThreads() const {
            return m_NumThreads.load(std::memory_order_acquire);
        }

        /**
         * Simple method for retreaving the index/id of the calling thread. If the calling thread is not a worker then
         * it's undifined behavior.
         *
         * @returns The thread index/id of the calling thread
         * */
        ThreadAffinity GetThreadIndex() const {
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
        friend struct ThreadAwaiter;

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
                        job->m_Tag = InvalidTag;
                        CW_ENSURE(m_TagBuffers.at(tag)->Push(job), "...");
                        return;
                    }
                }

                // Slow path: need to create it
                {
                    std::unique_lock lock(m_TagBuffersMutex);

                    // We double check because some thread might have already crated the tag buffer
                    if (!m_TagBuffers.contains(tag)) {
                        m_TagBuffers[tag] = std::make_unique<RingBuffer<Job*, TagBufferCapacity>>();
                    }

                    job->m_Tag = InvalidTag;
                    CW_ENSURE(m_TagBuffers.at(tag)->Push(job), "...");
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

        /**
         * Called within a thread to setup the worker's thread related config.
         *
         * @param threadId Which id will the thread have?
         * */
        void SetupWorkerThread(ThreadAffinity threadId) {
            m_Index = threadId;
        }

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
            // virtual call
            job->Resume();

            // Delete the function job since it can't possibly be rescheduled
            if (isFunction) {
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
        inline static std::unique_ptr<JobSystem> s_JobSystem = nullptr;

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

        std::unordered_map<Tag, TagBufferPtr<Job*>> m_TagBuffers;
        std::shared_mutex m_TagBuffersMutex;

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
            Job* parent = handle.promise().m_Parent;

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

    /**
     * Allows a coroutine to decide on which thread to continue executing by being
     * rescheduled again
     * */
    template <typename T>
    struct ThreadAwaiter {
        ThreadAffinity m_Thread;

        ThreadAwaiter<T>(ThreadAffinity thread)
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
