# CoroWeaver

A simple, yet powerful job system based on coroutines.

## Features

- C++20 implementation
- Use coroutines for more complex jobs, and functions for simpler and faster ones
- Non-owned threads can also participate in job execution
- 3 available priorities (low, medium, high)
- Custom thread affinity
- Fully templated
- Platform independent (no assembly; all is done through standard C++20 primitives).

## Features pending

- Add your custom memory allocators
- Expand local thread buffers in size as needed
- Support fast bulk operations
- Assign tags to jobs for better synchronization
- Fully independent implementation (without external dependencies)
- Fully exception safe.

## Reasons to use

When I started to make this project, I encountered a lot of fiber-based implementation based on Naughty Dog's own system explained in the [Parallelizing the Naughty Dog Engine][NDTalk] talk given by Christian Gyrling at the 2015 GDC. However, something I didn't like about it and still don't like to this day is that the fiber code has to be done in assembly because of the lack of support on platforms, which leaves you with the task of writing the fiber code for each platform and architecture you want to support (I know Windows does indeed support fibers, but this doesn't change the fact that it's very platform-dependent).

Luckily for me, coroutines were added in C++20 as part of the STL, so they're platform-independent from the start. It also has very good debug tools that can detect coroutine frames and help you visualize what is happening.

My job system implementation allows you to schedule both coroutines (for more complex and convenient jobs) and functions (for simpler and faster jobs). It also allows you to specify a priority to each job (low, medium, or high) as well as a restriction on which thread can execute the given job. This is very useful for things that do need to run continuously on the same thread, like window libraries, etc. If you use coroutines, dependencies between jobs are managed automatically, and while those dependencies are being executed in parallel, in the meantime, the thread can freely do other jobs or sleep if there is nothing else to do.

Because I'm still learning about coroutines, the code is simple, so you can add new features or optimize the existing ones in any way you desire.

## Reasons *not* to use

As I stated before, I'm still learning and the system is not fully developed and tested and new feature will be added. So if **stability**, **fully tested** and **optimized** are things that are indispensable for your use case then I can't recommend my design at the moment.

Internally it uses [moodycamel::ConcurrentQueue][concurrentqueue] as a dependency so you will need to also include it wherever you use CoroWeaver. Because of this dependecy it also has moodycamel's limitations which you can read on his github in more detail but that can be summarized as: **not linearizable** and **not NUMA aware**.

I'm working on improving and optimizing the code, but for now this is how the situation is.

## Basic use

The entire job system implementation is contained in **one header**, [`CorowWeaver.hpp`][Coroweaver.hpp].
Download and include that to use the system, you will also have to include **moodycamel's concurrentqueue** as my implementation depends on it.

Simple example:

```C++
#include <coroweaver/CoroWeaver.hpp>

// Define a simple coroutine
cw::JobCoroutine<void> SimpleCoroutine(std::atomic<bool>* executed) {
    *executed = true;
    co_return;
}

// Initialize the system with 2 worker threads
cw::JobSystem::Init(2);
cw::JobSystem& js = cw::JobSystem::GetInstance();

// Schedule a coroutine that will set executed to true
std::atomic<bool> executed{false};
cw::JobCoroutine<void> job = SimpleCoroutine(&executed);
js.Schedule(job);

// More operations...

// When we finish we simply shutdown the system and everything will be handled automatically
cw::JobSystem::Shutdown();
```

Description of basic methods/types:

- `JobCoroutine<T>`
      This is what every job coroutine must return in order to be able to be scheduled
- `JobSystem::Init(ThreadAffinity threadCount)`
      Static method that will construct the system's singleton with `threadCount` worker threads [^1]
- `Schedule(JobCoroutine<T>& job, ThreadAffinity threadId, JobPriority priority)`
      Schedules the given coroutine with the priority and affinity desired. By default there is no thread affinity,      and the priority is medium.
- `Shutdown()`
      Static method that deallocates all memory and finishes all remaining jobs.

[^1]: It's not guaranted to spawn `threadCount` threads if there are not sufficient threads available. You can always query the number of working threads via the `ThreadAffinity GetNumThreads() const` method. Also, for now there can be a maximum of 64 worker threads.

Note that the methods `Init` and `Shutdown` are the only **non-thread-safe** methods.

Another thing worth noting is that there can be a maximum of 64 jobs on the local buffer of a specific thread at the same time. That is, the jobs with a specific thread affinity. Upon reaching that limit and trying to push more the system will panic. This practically never happens, and I'm already developing a way to increase the capacity when needed, but for now a temporal solution is to change the default size of the local buffers.

Full API (pseudocode):

    # Initialization and shutting down of the system
    static Init(threadCont) : void
    static Shutdown() : void

    # Gets the instance of the system
    GetInstance() : JobSystem&
    
    # Schedule jobs
    Schedule(jobFunction, priority, threadId) : void
    Schedule(jobCoroutine, priority, threadId) : void
    
    # Gets the number of worker threads at the moment
    GetNumThreads() : ThreadAffinity
    # Get the thread id/index of the calling external worker thread
    GetThreadIndex(): ThreadAffinity
    
    # Convert non-owned threads to worker ones
    ConvertToWorkerThread() : ThreadAffinity
    # After conversion you need to deregister it before shutting down
    DeregisterWorkerThread() : void
    
    # Run external worker threads
    RunWorkerUntil(stop_token) : void
    RunWorkerFor(time) : void

## Coroutine features

#### Change Threads

A coroutine can suspend itself and change threads if needed. This is useful if only a specific section of a job is single-threaded or has other thread limitations.

```C++
cw::JobCoroutine<void> SimpleCoroutineChangeThread(ThreadAffinity thread) {
    // Operations on thread 1...
    // We can wait untill the the wanted thread is excecuting the coroutine
    co_await thread;
    // Other operations but now on thread 0...
    co_return;
}

// Initialize the system
cw::JobSystem::Init(2);

cw::JobSystem& js = cw::JobSystem::GetInstance();
cw::JobCoroutine<void> job = SimpleCoroutineChangeThread(0);

// We schedule the job on thread 1
js.Schedule(job, 1);

// Shutdown the system
cw::JobSystem::Shutdown();
```

You can always check the available worker threads via the `GetNumThreads()` method and the specific thread index of an external worker thread via `GetThreadIndex` method.

#### Wait other coroutines to finish before proceeding

Coroutines are also able to wait on other coroutines to finish before continuing[^2]. This allows job dependencies to be automatically managed and no busy waiting to happen. This is because while the coroutine is suspended waiting for its dependencies to finish, the thread executing the coroutine is still productive, executing other jobs or sleeping if there is nothing else to do.

[^2]: This is only the case between coroutines. There isn't a way of syncronizing functions and coroutines. So a coroutine waiting on a function is not allowed and vice versa. There's neither a way of synchronizing functions between them. Functions shall only be used in simple and independent jobs.

If no thread affinity was not set when scheduling the coroutine, then it's not guaranteed that the thread executing the coroutine after waiting will be the same as before. This, however, allows for much better performance when thread affinity is not required.

```C++
using namespace cw;

JobCoroutine<void> Children2() {
    // Now we are on thread 0
    co_return;
}

JobCoroutine<void> Children1() {
    // Operations on thread 1
    
    // Before continuing with the current coroutine we wait on Children2();
    co_await WhenAll(0, Children2());
    
    // We can also wait on multiple children on paralel (No affinity was set in this case):
    // co_await WhenAll(Children2(), Children3());
    
    // We are guarated to still be on thread 1 becaue we explicitly set the 
    // thread affinity of the current coroutine
    co_return;
}

// Initialize the system
JobSystem::Init(2);

JobSystem& js = JobSystem::GetInstance();
JobCoroutine<void> job = Children1();
js.Schedule(job, 1);

// Shutdown the system
JobSystem::Shutdown();
```

## External Workers

You can register/convert new worker threads once the system is initialized and use them as much as you want. However, it's necessary to follow the following steps.

#### Registering

It's as simple as calling the `ConvertToWorkerThread` method on an unregistered thread. This will prepare the thread so that it's able to execute jobs. This method returns the thread ID of the new worker.

```C++
// Initialize the system
cw::JobSystem::Init(2);
cw::JobSystem& js = cw::JobSystem::GetInstance();

// Convert the current thread into a worker
cw::ThreadAffinity id = js.ConvertToWorkerThread();

// Now there are 3 worker threads in total, this one and 2 more that are managed by the system
```

#### Working

There are 2 ways of executing jobs: `RunWorkerUntil` and `RunWorkerFor`. The first one will execute jobs until the stop_source requests a stop; the second option will keep running for a specified amount of time.

```C++
// Execute jobs for 50 milliseconds
js.RunWorkerFor(std::chrono::milliseconds(50));

// Excecute until source requets a stop
std::stop_source source;
js.RunWorkerUntil(source.get_token());
```

#### Deregistering

When shutting down the system, this will wait until all external workers have been deregistered via the `DeregisterWorkerThread` method, so if you forget to deregister even one, the system will sleep forever.

This step is only necessary on threads manually registered via `ConvertToWorkerThread`; those that are created in the `Init` method are managed automatically, **do not** try to manually deregister those threads in any way.

```C++
// Once we are done we deregister the worker
js.DeregisterWorkerThread();

// Now we can safely shut down the system
cw::JobSystem::Shutdown();
```

## Samples (work in progress)

## Benchmarks (work in progress)

## Tests

I haven't written an extended amount of tests, so there might be unknown bugs that are really easy to trigger. With time I'll write a decent amount so that every edge case will be documented.

## License

The source code of this repository has the Apache License V2.0, you can check the [LICENSE][license] document for more details.

As I stated above my implementation depents on moodycamel concurrent queue, which is under a simplified BSD license and dual-licensing under the Boost Software License. See [LICENSE MOODYCAMEL][moodylicense] for more details.

This project also uses Doctest for testing, which is under the MIT license.

[concurrentqueue]:https://github.com/cameron314/concurrentqueue
[NDTalk]:https://www.gdcvault.com/play/1022186/Parallelizing-the-Naughty-Dog-Engine
[Coroweaver.hpp]: https://github.com/Estikno/CoroWeaver/blob/main/include/coroweaver/CoroWeaver.hpp
[license]: https://github.com/Estikno/CoroWeaver/blob/main/LICENSE
[moodylicense]:https://github.com/cameron314/concurrentqueue/blob/master/LICENSE.md
