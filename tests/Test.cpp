#include <doctest.h>

// #include "CoroWeaver.hpp"
#include "coroweaver/CoroWeaver.hpp" // Single include version
#include <atomic>
#include <chrono>
#include <stop_token>
#include <thread>

using namespace cw;

// ─────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────

static void InitJS() {
    JobSystem::Init(2);
}

static void ShutdownJS() {
    JobSystem::Shutdown();
}

TEST_CASE("JobSystem - Created correctly") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    // Check dimensions
    REQUIRE(js.GetLocalBufferNumDEBUG() == 2);
    CHECK(JobSystem::GetInstance().GetThreadsDEBUG().size() == 2);
    CHECK(JobSystem::GetInstance().GetNumThreads() == 2);

    ShutdownJS();
}

JobCoroutine<void> SimpleCoroutine(std::atomic<bool>* executed) {
    *executed = true;
    co_return;
}

JobCoroutine<int> SimpleCoroutineInt(std::atomic<bool>* executed) {
    executed->store(true);
    co_return 10;
}

JobCoroutine<int> SimpleCoroutineInt2() {
    co_return 10;
}

JobCoroutine<void> SimpleCoroutineVoid(std::atomic<bool>* excecuted, std::atomic<int>* val) {
    auto [a] = co_await WhenAll(SimpleCoroutineInt2());
    val->store(a);
    excecuted->store(true);
    co_return;
}

JobCoroutine<void> SimpleCoroutineChangeThread(std::atomic<bool>* correct, ThreadAffinity thread) {
    co_await WaitThread(thread);
    correct->store(JobSystem::GetInstance().GetThreadIndex() == thread);
    co_return;
}

JobCoroutine<int> SimpleCoroutineWaitForMany2(std::atomic<int>* n) {
    n->fetch_add(1);
    co_return 12;
}

JobCoroutine<void> SimpleCoroutineWaitForMany1(std::atomic<int>* n) {
    n->fetch_add(1);
    co_return;
}

JobCoroutine<void> SimpleCoroutineWaitForMany(std::atomic<int>* n, std::atomic<int>* ret1) {
    auto [a, b] = co_await WhenAll(SimpleCoroutineWaitForMany1(n), SimpleCoroutineWaitForMany2(n));
    ret1->store(b);
    co_return;
}

JobCoroutine<void> ChildrenExcecuteOnOtherThread2(ThreadAffinity affinity) {
    CHECK(affinity == JobSystem::GetInstance().GetThreadIndex());
    co_return;
}

JobCoroutine<void> ChildrenExcecuteOnOtherThread1(std::atomic<bool>& done, ThreadAffinity affinity) {
    CHECK(1 == JobSystem::GetInstance().GetThreadIndex());
    co_await WhenAll(0, ChildrenExcecuteOnOtherThread2(affinity));
    done.store(true);
    co_return;
}

// ─────────────────────────────────────────────
// Basic submission
// ─────────────────────────────────────────────

TEST_CASE("JobSystem - fire and forget job executes") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    std::atomic<bool> executed{false};

    CHECK_NOTHROW(js.Schedule([&executed]() { executed = true; }));

    auto start = std::chrono::steady_clock::now();
    while (!executed) {
        std::this_thread::yield();
        auto elapsed = std::chrono::steady_clock::now() - start;
        REQUIRE(elapsed < std::chrono::seconds(5)); // fail if takes too long
    }

    CHECK(executed);
    ShutdownJS();
}

TEST_CASE("JobSystem - fire and forget coroutine executes") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    std::atomic<bool> executed{false};
    JobCoroutine<void> job = SimpleCoroutine(&executed);

    CHECK_NOTHROW(js.Schedule(job));

    auto start = std::chrono::steady_clock::now();
    while (!executed) {
        std::this_thread::yield();
        auto elapsed = std::chrono::steady_clock::now() - start;
        REQUIRE(elapsed < std::chrono::seconds(5)); // fail if takes too long
    }

    CHECK(executed);
    ShutdownJS();
}

TEST_CASE("JobSystem - coroutine that returns cannot be excecuted from a non-coroutine") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    std::atomic<bool> executed{false};
    JobCoroutine<int> job = SimpleCoroutineInt(&executed);
    int res = 0;

    CHECK_NOTHROW(js.Schedule(job));

    auto start = std::chrono::steady_clock::now();
    while (!executed) {
        std::this_thread::yield();
        auto elapsed = std::chrono::steady_clock::now() - start;
        REQUIRE(elapsed < std::chrono::seconds(5)); // fail if takes too long
    }

    CHECK(executed);
    CHECK(!job.Get(res));

    ShutdownJS();
}

TEST_CASE("JobSystem - coroutine that returns excecutes correctly") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    std::atomic<bool> executed{false};
    std::atomic<int> val{0};
    JobCoroutine<void> job = SimpleCoroutineVoid(&executed, &val);

    CHECK_NOTHROW(js.Schedule(job));

    auto start = std::chrono::steady_clock::now();
    while (!executed) {
        std::this_thread::yield();
        auto elapsed = std::chrono::steady_clock::now() - start;
        REQUIRE(elapsed < std::chrono::seconds(5)); // fail if takes too long
    }

    CHECK(executed);
    CHECK_EQ(val.load(), 10);

    ShutdownJS();
}

TEST_CASE("JobSystem - coroutine that changes thread excecutes correctly") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    std::atomic<bool> correct{false};
    JobCoroutine<void> job = SimpleCoroutineChangeThread(&correct, 0);

    CHECK_NOTHROW(js.Schedule(job, 1));

    auto start = std::chrono::steady_clock::now();
    while (!correct) {
        std::this_thread::yield();
        auto elapsed = std::chrono::steady_clock::now() - start;
        REQUIRE(elapsed < std::chrono::seconds(5)); // fail if takes too long
    }

    CHECK(correct);

    ShutdownJS();
}

TEST_CASE("JobSystem - coroutine that waits multiple jobs simulatenously excecutes correctly") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    std::atomic<int> n{0};
    std::atomic<int> ret1{0};
    JobCoroutine<void> job = SimpleCoroutineWaitForMany(&n, &ret1);

    CHECK_NOTHROW(js.Schedule(job));

    auto start = std::chrono::steady_clock::now();
    while (ret1.load() == 0) {
        std::this_thread::yield();
        auto elapsed = std::chrono::steady_clock::now() - start;
        REQUIRE(elapsed < std::chrono::seconds(5)); // fail if takes too long
    }

    CHECK_EQ(n.load(), 2);
    CHECK_EQ(ret1.load(), 12);

    ShutdownJS();
}

TEST_CASE("JobSystem - coroutine that waits multiple jobs on multiple threads excecutes correctly") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    std::atomic<bool> done{false};
    JobCoroutine<void> job = ChildrenExcecuteOnOtherThread1(done, 0);

    CHECK_NOTHROW(js.Schedule(job, 1));

    auto start = std::chrono::steady_clock::now();
    while (!done.load()) {
        std::this_thread::yield();
        auto elapsed = std::chrono::steady_clock::now() - start;
        REQUIRE(elapsed < std::chrono::seconds(5)); // fail if takes too long
    }

    CHECK(done.load());

    ShutdownJS();
}

// ─────────────────────────────────────────────
// Priority and ordering
// ─────────────────────────────────────────────

// TEST_CASE("JobSystem - high priority job executes before low priority") {
//     InitJS();
//     JobSystem& js = JobSystem::GetInstance();
//
//     // Fill threads with blocking jobs so we can queue up prioritized work
//     std::atomic<bool> gate{false};
//     std::atomic<int> executionOrder{0};
//     std::atomic<int> lowOrder{-1};
//     std::atomic<int> highOrder{-1};
//
//     // Block all threads first
//     for (int i = 0; i < 2; ++i) {
//         js.Schedule([&gate]() {
//             while (!gate.load(std::memory_order_acquire))
//                 std::this_thread::yield();
//         });
//     }
//
//     // Small sleep to let blocking jobs occupy threads
//     std::this_thread::sleep_for(std::chrono::milliseconds(10));
//
//     // Queue low then high
//     js.Schedule([&]() { lowOrder.store(executionOrder.fetch_add(1)); }, JobPriority::Low);
//     js.Schedule([&]() { highOrder.store(executionOrder.fetch_add(1)); }, JobPriority::High);
//
//     // Release the gate
//     gate.store(true, std::memory_order_release);
//
//     auto start = std::chrono::steady_clock::now();
//     while (lowOrder.load() == -1 || highOrder.load() == -1) {
//         std::this_thread::yield();
//         REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::seconds(5));
//     }
//
//     CHECK(highOrder.load() < lowOrder.load());
//
//     ShutdownJS();
// }

TEST_CASE("JobSystem - high priority batch finishes before low priority batch") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    constexpr int N = 50;

    std::atomic<bool> gate{false};
    std::atomic<int> executionOrder{0};

    // -1 = not yet run
    std::atomic<int> highOrders[N];
    std::atomic<int> lowOrders[N];
    for (int i = 0; i < N; ++i) {
        highOrders[i].store(-1);
        lowOrders[i].store(-1);
    }

    // Block all threads so we can fill the queues before anything runs
    for (int i = 0; i < 2; ++i) {
        js.Schedule([&gate]() {
            while (!gate.load(std::memory_order_acquire))
                std::this_thread::yield();
        });
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // Queue all low-priority first, then all high-priority
    // This makes the test maximally adversarial: if priority is ignored,
    // low jobs would tend to run first since they were enqueued first
    for (int i = 0; i < N; ++i)
        js.Schedule([&, i]() { lowOrders[i].store(executionOrder.fetch_add(1)); }, JobPriority::Low);
    for (int i = 0; i < N; ++i)
        js.Schedule([&, i]() { highOrders[i].store(executionOrder.fetch_add(1)); }, JobPriority::High);

    gate.store(true, std::memory_order_release);

    auto start = std::chrono::steady_clock::now();
    while (true) {
        std::this_thread::yield();
        REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::seconds(5));

        bool allDone = true;
        for (int i = 0; i < N; ++i) {
            if (highOrders[i].load() == -1 || lowOrders[i].load() == -1) {
                allDone = false;
                break;
            }
        }
        if (allDone)
            break;
    }

    // Find the last high-priority execution order and the first low-priority one
    int lastHigh = -1;
    for (int i = 0; i < N; ++i)
        lastHigh = std::max(lastHigh, highOrders[i].load());

    int firstLow = std::numeric_limits<int>::max();
    for (int i = 0; i < N; ++i)
        firstLow = std::min(firstLow, lowOrders[i].load());

    CHECK(lastHigh < firstLow);

    ShutdownJS();
}

// ─────────────────────────────────────────────
// Thread affinity
// ─────────────────────────────────────────────

TEST_CASE("JobSystem - function job runs on correct thread") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    std::atomic<bool> done{false};
    std::atomic<ThreadAffinity> ranOnThread{InvalidThreadIndex};

    js.Schedule(
        [&]() {
            ranOnThread.store(js.GetThreadIndex());
            done.store(true);
        },
        JobPriority::Medium,
        1);

    auto start = std::chrono::steady_clock::now();
    while (!done.load()) {
        std::this_thread::yield();
        REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::seconds(5));
    }

    CHECK_EQ(ranOnThread.load(), 1);

    ShutdownJS();
}

// ─────────────────────────────────────────────
// Stress tests
// ─────────────────────────────────────────────

TEST_CASE("JobSystem - many function jobs all execute") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    constexpr int JobCount = 1000;
    std::atomic<int> counter{0};

    for (int i = 0; i < JobCount; ++i)
        js.Schedule([&counter]() { counter.fetch_add(1); });

    auto start = std::chrono::steady_clock::now();
    while (counter.load() < JobCount) {
        std::this_thread::yield();
        REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::seconds(10));
    }

    CHECK_EQ(counter.load(), JobCount);

    ShutdownJS();
}

TEST_CASE("JobSystem - many coroutine jobs all execute") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    constexpr int JobCount = 100;
    std::atomic<int> counter{0};

    // Need to keep coroutines alive until scheduled
    std::vector<JobCoroutine<void>> jobs;
    jobs.reserve(JobCount);

    for (int i = 0; i < JobCount; ++i) {
        jobs.push_back([](std::atomic<int>* c) -> JobCoroutine<void> {
            c->fetch_add(1);
            co_return;
        }(&counter));
    }

    for (auto& job : jobs)
        js.Schedule(job);

    auto start = std::chrono::steady_clock::now();
    while (counter.load() < JobCount) {
        std::this_thread::yield();
        REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::seconds(10));
    }

    CHECK_EQ(counter.load(), JobCount);

    ShutdownJS();
}

// ─────────────────────────────────────────────
// Chaining and nesting
// ─────────────────────────────────────────────

JobCoroutine<int> ChainedLevel2() {
    co_return 42;
}

JobCoroutine<int> ChainedLevel1() {
    auto [val] = co_await WhenAll(ChainedLevel2());
    co_return val + 1;
}

JobCoroutine<void> ChainedLevel0(std::atomic<int>* result, std::atomic<bool>* done) {
    auto [val] = co_await WhenAll(ChainedLevel1());
    result->store(val);
    done->store(true);
    co_return;
}

TEST_CASE("JobSystem - deeply chained coroutines resolve correctly") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    std::atomic<int> result{0};
    std::atomic<bool> done{false};
    JobCoroutine<void> job = ChainedLevel0(&result, &done);

    js.Schedule(job);

    auto start = std::chrono::steady_clock::now();
    while (!done.load()) {
        std::this_thread::yield();
        REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::seconds(5));
    }

    CHECK_EQ(result.load(), 43);

    ShutdownJS();
}

JobCoroutine<void> WhenAllNested(std::atomic<int>* sum, std::atomic<bool>* done) {
    auto [a, b, c] = co_await WhenAll(ChainedLevel2(), ChainedLevel1(), ChainedLevel2());
    sum->store(a + b + c);
    done->store(true);
    co_return;
}

TEST_CASE("JobSystem - WhenAll with three jobs resolves correctly") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    std::atomic<int> sum{0};
    std::atomic<bool> done{false};
    JobCoroutine<void> job = WhenAllNested(&sum, &done);

    js.Schedule(job);

    auto start = std::chrono::steady_clock::now();
    while (!done.load()) {
        std::this_thread::yield();
        REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::seconds(5));
    }

    // ChainedLevel2 = 42, ChainedLevel1 = 43, ChainedLevel2 = 42 → 127
    CHECK_EQ(sum.load(), 127);

    ShutdownJS();
}

// ─────────────────────────────────────────────
// Edge cases
// ─────────────────────────────────────────────

TEST_CASE("JobSystem - coroutine scheduled but never awaited doesn't leak") {
    // This test just checks it doesn't crash or leak - ASAN will catch leaks
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    {
        // Created but never scheduled - destructor should clean up
        JobCoroutine<int> unscheduled = ChainedLevel2();
        (void) unscheduled;
    }

    ShutdownJS();
}

TEST_CASE("JobSystem - multiple independent coroutines run concurrently") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    constexpr int N = 4;
    std::atomic<int> counter{0};
    std::atomic<bool> done[N];
    for (auto& d : done)
        d.store(false);

    auto makeJob = [](std::atomic<int>* c, std::atomic<bool>* d) -> JobCoroutine<void> {
        c->fetch_add(1);
        d->store(true);
        co_return;
    };

    std::vector<JobCoroutine<void>> jobs;
    for (int i = 0; i < N; ++i)
        jobs.push_back(makeJob(&counter, &done[i]));

    for (auto& job : jobs)
        js.Schedule(job);

    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < N; ++i) {
        while (!done[i].load()) {
            std::this_thread::yield();
            REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::seconds(5));
        }
    }

    CHECK_EQ(counter.load(), N);

    ShutdownJS();
}


TEST_CASE("JobSystem - convert main thread to worker and run jobs") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    js.ConvertToWorkerThread();
    ThreadAffinity myIndex = js.GetThreadIndex();
    CHECK(myIndex != InvalidThreadIndex);
    CHECK(myIndex == 2); // initial thread count was 2, so new index should be 2

    std::atomic<bool> jobDone{false};
    js.Schedule([&]() { jobDone.store(true); }, JobPriority::Medium, myIndex);

    std::stop_source source;
    // Run until job completes, then stop ourselves
    js.Schedule([&]() { source.request_stop(); }, JobPriority::Medium, myIndex);

    js.RunWorkerUntil(source.get_token());

    CHECK(jobDone.load());

    js.DeregisterWorkerThread();
    CHECK(js.GetThreadIndex() == InvalidThreadIndex);

    ShutdownJS();
}

TEST_CASE("JobSystem - RunWorkerFor returns after timeout with no jobs") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    js.ConvertToWorkerThread();

    auto start = std::chrono::steady_clock::now();
    js.RunWorkerFor(std::chrono::milliseconds(50));
    auto elapsed = std::chrono::steady_clock::now() - start;

    // Should return reasonably close to 50ms, not hang
    CHECK(elapsed >= std::chrono::milliseconds(40));
    CHECK(elapsed < std::chrono::seconds(2));

    js.DeregisterWorkerThread();
    ShutdownJS();
}

TEST_CASE("JobSystem - RunWorkerFor processes jobs scheduled to its index") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    js.ConvertToWorkerThread();
    ThreadAffinity myIndex = js.GetThreadIndex();

    std::atomic<int> counter{0};
    for (int i = 0; i < 5; ++i)
        js.Schedule([&counter]() { counter.fetch_add(1); }, JobPriority::Medium, myIndex);

    js.RunWorkerFor(std::chrono::milliseconds(100));

    CHECK_EQ(counter.load(), 5);

    js.DeregisterWorkerThread();
    ShutdownJS();
}

TEST_CASE("JobSystem - deregistered index gets reused") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    js.ConvertToWorkerThread();
    ThreadAffinity firstIndex = js.GetThreadIndex();
    js.DeregisterWorkerThread();

    js.ConvertToWorkerThread();
    ThreadAffinity secondIndex = js.GetThreadIndex();

    CHECK_EQ(firstIndex, secondIndex); // index should be reused

    js.DeregisterWorkerThread();
    ShutdownJS();
}

TEST_CASE("JobSystem - converted thread participates in global job processing") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    js.ConvertToWorkerThread();

    constexpr int JobCount = 50;
    std::atomic<int> counter{0};

    for (int i = 0; i < JobCount; ++i)
        js.Schedule([&counter]() { counter.fetch_add(1); }); // global queue, no affinity

    std::stop_source source;

    // Stop ourselves once all jobs are done
    std::thread stopper([&]() {
        while (counter.load() < JobCount)
            std::this_thread::yield();
        source.request_stop();
        // Wake the worker so it re-checks the condition
    });

    js.RunWorkerUntil(source.get_token());
    stopper.join();

    CHECK_EQ(counter.load(), JobCount);

    js.DeregisterWorkerThread();
    ShutdownJS();
}

TEST_CASE("JobSystem - shutdown waits for external worker to deregister") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    ThreadAffinity mainIndex = js.ConvertToWorkerThread();

    std::stop_source source;
    std::atomic<bool> deregistered{false};

    // std::thread externalWorker([&]() {
    //     // already converted on this "thread" conceptually -
    //     // NOTE: in practice ConvertToWorkerThread must be called
    //     // from the thread that will use it (m_Index is thread_local)
    // });

    // Since m_Index is thread_local, do the conversion + work on a real separate thread:
    std::thread realExternal([&]() {
        ThreadAffinity otherIndex = js.ConvertToWorkerThread();

        CHECK_NE(mainIndex, otherIndex);

        js.RunWorkerUntil(source.get_token()); // will exit when m_Running becomes false during Shutdown
        js.DeregisterWorkerThread();
        deregistered.store(true);
    });

    // Give it time to register and start waiting
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    // Deregister the main thread's converted worker too
    js.DeregisterWorkerThread();

    // Shutdown should block until realExternal calls DeregisterWorkerThread
    ShutdownJS();

    CHECK(deregistered.load());

    realExternal.join();
    // externalWorker.join();
}

TEST_CASE("JobSystem - shutdown waits for external worker to deregister (now with RunWorkerFor)") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    ThreadAffinity mainIndex = js.ConvertToWorkerThread();

    std::atomic<bool> deregistered{false};

    // std::thread externalWorker([&]() {
    //     // already converted on this "thread" conceptually -
    //     // NOTE: in practice ConvertToWorkerThread must be called
    //     // from the thread that will use it (m_Index is thread_local)
    // });

    // Since m_Index is thread_local, do the conversion + work on a real separate thread:
    std::thread realExternal([&]() {
        ThreadAffinity otherIndex = js.ConvertToWorkerThread();

        CHECK_NE(mainIndex, otherIndex);

        js.RunWorkerFor(std::chrono::seconds(50)); // will exit when m_Running becomes false during Shutdown
        js.DeregisterWorkerThread();
        deregistered.store(true);
    });

    // Give it time to register and start waiting
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    // Deregister the main thread's converted worker too
    js.DeregisterWorkerThread();

    // Shutdown should block until realExternal calls DeregisterWorkerThread
    ShutdownJS();

    CHECK(deregistered.load());

    realExternal.join();
    // externalWorker.join();
}

// ─────────────────────────────────────────────
// Tag scheduling
// ─────────────────────────────────────────────

JobCoroutine<void> TaggedCoroutine(std::atomic<int>* counter) {
    counter->fetch_add(1);
    co_return;
}

TEST_CASE("JobSystem - tagged function job does not execute until tag is scheduled") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    std::atomic<bool> executed{false};

    js.Schedule([&]() { executed.store(true); }, JobPriority::Medium, InvalidThreadIndex, 42);

    // Give workers time to (incorrectly) run it
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    CHECK_FALSE(executed.load());

    js.ScheduleTag(42);

    auto start = std::chrono::steady_clock::now();
    while (!executed.load()) {
        std::this_thread::yield();
        REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::seconds(5));
    }

    CHECK(executed.load());
    ShutdownJS();
}

TEST_CASE("JobSystem - tagged coroutine does not execute until tag is scheduled") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    std::atomic<int> counter{0};
    JobCoroutine<void> job = TaggedCoroutine(&counter);

    js.Schedule(job, InvalidThreadIndex, JobPriority::Medium, 42);

    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    CHECK_EQ(counter.load(), 0);

    js.ScheduleTag(42);

    auto start = std::chrono::steady_clock::now();
    while (counter.load() == 0) {
        std::this_thread::yield();
        REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::seconds(5));
    }

    CHECK_EQ(counter.load(), 1);
    ShutdownJS();
}

TEST_CASE("JobSystem - multiple jobs under the same tag all fire together") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    constexpr int N = 5;
    std::atomic<int> counter{0};

    for (int i = 0; i < N; ++i)
        js.Schedule([&]() { counter.fetch_add(1); }, JobPriority::Medium, InvalidThreadIndex, 99);

    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    CHECK_EQ(counter.load(), 0);

    js.ScheduleTag(99);

    auto start = std::chrono::steady_clock::now();
    while (counter.load() < N) {
        std::this_thread::yield();
        REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::seconds(5));
    }

    CHECK_EQ(counter.load(), N);
    ShutdownJS();
}

TEST_CASE("JobSystem - jobs under different tags are independent") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    std::atomic<int> counterA{0};
    std::atomic<int> counterB{0};

    js.Schedule([&]() { counterA.fetch_add(1); }, JobPriority::Medium, InvalidThreadIndex, 1);
    js.Schedule([&]() { counterB.fetch_add(1); }, JobPriority::Medium, InvalidThreadIndex, 2);

    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    CHECK_EQ(counterA.load(), 0);
    CHECK_EQ(counterB.load(), 0);

    js.ScheduleTag(1);

    auto start = std::chrono::steady_clock::now();
    while (counterA.load() == 0) {
        std::this_thread::yield();
        REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::seconds(5));
    }

    CHECK_EQ(counterA.load(), 1);
    // Tag 2 was never scheduled, B must still be 0
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    CHECK_EQ(counterB.load(), 0);

    js.ScheduleTag(2);

    start = std::chrono::steady_clock::now();
    while (counterB.load() == 0) {
        std::this_thread::yield();
        REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::seconds(5));
    }

    CHECK_EQ(counterB.load(), 1);
    ShutdownJS();
}

TEST_CASE("JobSystem - scheduling InvalidTag is a no-op") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    // Should not panic or do anything observable
    CHECK_NOTHROW(js.ScheduleTag(InvalidTag));

    ShutdownJS();
}

TEST_CASE("JobSystem - scheduling a tag that has no jobs is a no-op") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    CHECK_NOTHROW(js.ScheduleTag(123));

    ShutdownJS();
}

TEST_CASE("JobSystem - tag can be scheduled multiple times, second fire is a no-op") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    std::atomic<int> counter{0};
    js.Schedule([&]() { counter.fetch_add(1); }, JobPriority::Medium, InvalidThreadIndex, 7);

    js.ScheduleTag(7);

    auto start = std::chrono::steady_clock::now();
    while (counter.load() == 0) {
        std::this_thread::yield();
        REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::seconds(5));
    }

    CHECK_EQ(counter.load(), 1);

    // Fire again — buffer was drained, nothing should execute
    js.ScheduleTag(7);
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    CHECK_EQ(counter.load(), 1);

    ShutdownJS();
}

TEST_CASE("JobSystem - tagged job respects thread affinity when fired") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    std::atomic<ThreadAffinity> ranOn{InvalidThreadIndex};
    std::atomic<bool> done{false};

    js.Schedule(
        [&]() {
            ranOn.store(js.GetThreadIndex());
            done.store(true);
        },
        JobPriority::Medium,
        /*threadId=*/1,
        /*tag=*/55);

    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    CHECK_FALSE(done.load());

    js.ScheduleTag(55);

    auto start = std::chrono::steady_clock::now();
    while (!done.load()) {
        std::this_thread::yield();
        REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::seconds(5));
    }

    CHECK_EQ(ranOn.load(), 1);
    ShutdownJS();
}

TEST_CASE("JobSystem - many tagged jobs stress test") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    constexpr int N = 200; // well within the 256 capacity
    std::atomic<int> counter{0};

    for (int i = 0; i < N; ++i)
        js.Schedule([&]() { counter.fetch_add(1); }, JobPriority::Medium, InvalidThreadIndex, 88);

    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    CHECK_EQ(counter.load(), 0);

    js.ScheduleTag(88);

    auto start = std::chrono::steady_clock::now();
    while (counter.load() < N) {
        std::this_thread::yield();
        REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::seconds(10));
    }

    CHECK_EQ(counter.load(), N);
    ShutdownJS();
}

// ─────────────────────────────────────────────
// WaitTag / WaitThread awaiter tests
// ─────────────────────────────────────────────

JobCoroutine<void> CoroutineWaitsForTag(std::atomic<int>* phase) {
    phase->store(1); // reached the co_await
    co_await WaitTag(200);
    phase->store(2); // resumed after tag fired
    co_return;
}

TEST_CASE("JobSystem - coroutine suspended on WaitTag does not resume until tag is scheduled") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    std::atomic<int> phase{0};
    JobCoroutine<void> job = CoroutineWaitsForTag(&phase);
    js.Schedule(job);

    // Wait until coroutine reaches the co_await
    auto start = std::chrono::steady_clock::now();
    while (phase.load() < 1) {
        std::this_thread::yield();
        REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::seconds(5));
    }

    // Parked on tag — must not advance
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    CHECK_EQ(phase.load(), 1);

    js.ScheduleTag(200);

    start = std::chrono::steady_clock::now();
    while (phase.load() < 2) {
        std::this_thread::yield();
        REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::seconds(5));
    }

    CHECK_EQ(phase.load(), 2);
    ShutdownJS();
}

JobCoroutine<void> CoroutineWaitsForTagTwice(std::atomic<int>* phase) {
    phase->store(1);
    co_await WaitTag(11);
    phase->store(2);
    co_await WaitTag(12);
    phase->store(3);
    co_return;
}

TEST_CASE("JobSystem - coroutine can WaitTag multiple times in sequence") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    std::atomic<int> phase{0};
    JobCoroutine<void> job = CoroutineWaitsForTagTwice(&phase);
    js.Schedule(job);

    // Wait for first park
    auto start = std::chrono::steady_clock::now();
    while (phase.load() < 1) {
        std::this_thread::yield();
        REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::seconds(5));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    CHECK_EQ(phase.load(), 1);

    js.ScheduleTag(11);

    start = std::chrono::steady_clock::now();
    while (phase.load() < 2) {
        std::this_thread::yield();
        REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::seconds(5));
    }

    // Parked on second tag now
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    CHECK_EQ(phase.load(), 2);

    js.ScheduleTag(12);

    start = std::chrono::steady_clock::now();
    while (phase.load() < 3) {
        std::this_thread::yield();
        REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::seconds(5));
    }

    CHECK_EQ(phase.load(), 3);
    ShutdownJS();
}

JobCoroutine<void> CoroutineWaitsForTagThenThread(std::atomic<int>* phase, ThreadAffinity target) {
    phase->store(1);
    co_await WaitTag(77);
    phase->store(2);
    co_await WaitThread(target);
    phase->store(3);
    co_return;
}

TEST_CASE("JobSystem - coroutine can WaitTag then WaitThread in sequence") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    std::atomic<int> phase{0};
    // Schedule on thread 1, tag will release it, then it migrates to thread 0
    JobCoroutine<void> job = CoroutineWaitsForTagThenThread(&phase, 0);
    js.Schedule(job, 1);

    auto start = std::chrono::steady_clock::now();
    while (phase.load() < 1) {
        std::this_thread::yield();
        REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::seconds(5));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    CHECK_EQ(phase.load(), 1);

    js.ScheduleTag(77);

    start = std::chrono::steady_clock::now();
    while (phase.load() < 3) {
        std::this_thread::yield();
        REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::seconds(5));
    }

    CHECK_EQ(phase.load(), 3);
    ShutdownJS();
}

JobCoroutine<int> CoroutineWaitsForTagReturnsValue(std::atomic<int>* phase) {
    phase->store(1);
    co_await WaitTag(33);
    phase->store(2);
    co_return 99;
}

JobCoroutine<void> CoroutineAwaitsTagChild(std::atomic<int>* phase, std::atomic<int>* result) {
    auto [val] = co_await WhenAll(CoroutineWaitsForTagReturnsValue(phase));
    result->store(val);
    co_return;
}

TEST_CASE("JobSystem - parent correctly receives value from child that used WaitTag") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    std::atomic<int> phase{0};
    std::atomic<int> result{0};
    JobCoroutine<void> job = CoroutineAwaitsTagChild(&phase, &result);
    js.Schedule(job);

    // Wait for child to park on tag
    auto start = std::chrono::steady_clock::now();
    while (phase.load() < 1) {
        std::this_thread::yield();
        REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::seconds(5));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    // Parent must also still be suspended (child hasn't finished)
    CHECK_EQ(result.load(), 0);

    js.ScheduleTag(33);

    start = std::chrono::steady_clock::now();
    while (result.load() == 0) {
        std::this_thread::yield();
        REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::seconds(5));
    }

    CHECK_EQ(phase.load(), 2);
    CHECK_EQ(result.load(), 99);
    ShutdownJS();
}

JobCoroutine<void> CoroutineWaitsForTagStress(std::atomic<int>* counter) {
    co_await WaitTag(44);
    counter->fetch_add(1);
    co_return;
}

TEST_CASE("JobSystem - many coroutines parked on same tag all resume when tag fires") {
    InitJS();
    JobSystem& js = JobSystem::GetInstance();

    constexpr int N = 20;
    std::atomic<int> counter{0};

    std::vector<JobCoroutine<void>> jobs;
    jobs.reserve(N);
    for (int i = 0; i < N; ++i)
        jobs.push_back(CoroutineWaitsForTagStress(&counter));

    for (auto& job : jobs)
        js.Schedule(job);

    // Give all coroutines time to reach and park on the tag
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    CHECK_EQ(counter.load(), 0);

    js.ScheduleTag(44);

    auto start = std::chrono::steady_clock::now();
    while (counter.load() < N) {
        std::this_thread::yield();
        REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::seconds(5));
    }

    CHECK_EQ(counter.load(), N);
    ShutdownJS();
}
