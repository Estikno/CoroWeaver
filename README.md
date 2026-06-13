# CoroWeaver

A simple, yet powerfull job system based on coroutines.

## Features

- C++20 implementation
- Use coroutines for more complex jobs, and functions for simpler ones
- Non-owned threads can also participate in job execution
- Jobs have 3 priorities (low, medium, high)
- Select specifically which thread shall execute your job
- Fully templated
- Platform independent (no assembly; all is done through standard C++20 primitives).
- Exception safe.

## Features pending

- Add your custom memory allocators
- Expand local thread buffers in size as needed
- Support fast bulk operations
- Assign tags to jobs for better synchronization
- Fully independent free implementation

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
      Schedules the given coroutine with the priority and affinity desired. By default there is no thread affinity and the priority is medium.
- `Shutdown()`
      Deallocates all memory and finishes all the jobs that remain.

[^1]: It's not guaranted to spawn `threadCount` threads if there are not sufficient threads available. You can always query the number of working threads via the `ThreadAffinity GetNumThreads() const` method. Also, for now there can be a maximum of 64 worker threads.

Note that the methods `Init` and `Shutdown` are the only **non-thread-safe** methods.

Another thing worth noting is that there can be a maximum of 64 jobs on the local buffer of a specific thread at the same time. That is, the jobs with a specific thread affinity. Upon reaching that limit and trying to push more the system will panic. This practically never happens, and I'm developing a way to increase the capacity when needed, but for now a temporal solution is to change the default size of the local buffers.

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

Coroutines are also able to wait on other coroutines to finish before continuing. This allows job dependencies to be automatically managed and no busy waiting to happen. This is because while the coroutine is suspended waiting for its dependencies to finish, the thread executing the coroutine is still productive, executing other jobs or sleeping if there is nothing else to do.

If no thread affinity was set when scheduling the coroutine, then it's not guaranteed that the thread executing the coroutine after waiting will be the same as before. This, however, allows for much better performance when thread dependency is not needed.

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
    
    // We are guarated to still be on thread 1 becaue explicitly set the 
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

#### Preallocation (correctly using `try_enqueue`)

`try_enqueue`, unlike just plain `enqueue`, will never allocate memory. If there's not enough room in the
queue, it simply returns false. The key to using this method properly, then, is to ensure enough space is
pre-allocated for your desired maximum element count.

The constructor accepts a count of the number of elements that it should reserve space for. Because the
queue works with blocks of elements, however, and not individual elements themselves, the value to pass
in order to obtain an effective number of pre-allocated element slots is non-obvious.

First, be aware that the count passed is rounded up to the next multiple of the block size. Note that the
default block size is 32 (this can be changed via the traits). Second, once a slot in a block has been
enqueued to, that slot cannot be re-used until the rest of the block has been completely filled
up and then completely emptied. This affects the number of blocks you need in order to account for the
overhead of partially-filled blocks. Third, each producer (whether implicit or explicit) claims and recycles
blocks in a different manner, which again affects the number of blocks you need to account for a desired number of
usable slots.

Suppose you want the queue to be able to hold at least `N` elements at any given time. Without delving too
deep into the rather arcane implementation details, here are some simple formulas for the number of elements
to request for pre-allocation in such a case. Note the division is intended to be arithmetic division and not
integer division (in order for `ceil()` to work).

For explicit producers (using tokens to enqueue):

```C++
(ceil(N / BLOCK_SIZE) + 1) * MAX_NUM_PRODUCERS * BLOCK_SIZE
```

For implicit producers (no tokens):

```C++
(ceil(N / BLOCK_SIZE) - 1 + 2 * MAX_NUM_PRODUCERS) * BLOCK_SIZE
```

When using mixed producer types:

```C++
((ceil(N / BLOCK_SIZE) - 1) * (MAX_EXPLICIT_PRODUCERS + 1) + 2 * (MAX_IMPLICIT_PRODUCERS + MAX_EXPLICIT_PRODUCERS)) * BLOCK_SIZE
```

If these formulas seem rather inconvenient, you can use the constructor overload that accepts the minimum
number of elements (`N`) and the maximum number of explicit and implicit producers directly, and let it do the
computation for you.

In addition to blocks, there are other internal data structures that require allocating memory if they need to resize (grow).
If using `try_enqueue` exclusively, the initial sizes may be exceeded, causing subsequent `try_enqueue` operations to fail.
Specifically, the `INITIAL_IMPLICIT_PRODUCER_HASH_SIZE` trait limits the number of implicit producers that can be active at once
before the internal hash needs resizing. Along the same lines, the `IMPLICIT_INITIAL_INDEX_SIZE` trait limits the number of
unconsumed elements that an implicit producer can insert before its internal hash needs resizing. Similarly, the
`EXPLICIT_INITIAL_INDEX_SIZE` trait limits the number of unconsumed elements that an explicit producer can insert before its
internal hash needs resizing. In order to avoid hitting these limits when using `try_enqueue`, it is crucial to adjust the
initial sizes in the traits appropriately, in addition to sizing the number of blocks properly as outlined above.

Finally, it's important to note that because the queue is only eventually consistent and takes advantage of
weak memory ordering for speed, there's always a possibility that under contention `try_enqueue` will fail
even if the queue is correctly pre-sized for the desired number of elements. (e.g. A given thread may think that
the queue's full even when that's no longer the case.) So no matter what, you still need to handle the failure
case (perhaps looping until it succeeds), unless you don't mind dropping elements.

#### Exception safety

The queue is exception safe, and will never become corrupted if used with a type that may throw exceptions.
The queue itself never throws any exceptions (operations fail gracefully (return false) if memory allocation
fails instead of throwing `std::bad_alloc`).

It is important to note that the guarantees of exception safety only hold if the element type never throws
from its destructor, and that any iterators passed into the queue (for bulk operations) never throw either.
Note that in particular this means `std::back_inserter` iterators must be used with care, since the vector
being inserted into may need to allocate and throw a `std::bad_alloc` exception from inside the iterator;
so be sure to reserve enough capacity in the target container first if you do this.

The guarantees are presently as follows:

- Enqueue operations are rolled back completely if an exception is thrown from an element's constructor.
  For bulk enqueue operations, this means that elements are copied instead of moved (in order to avoid
  having only some objects moved in the event of an exception). Non-bulk enqueues always use
  the move constructor if one is available.
- If the assignment operator throws during a dequeue operation (both single and bulk), the element(s) are
  considered dequeued regardless. In such a case, the dequeued elements are all properly destructed before
  the exception is propagated, but there's no way to get the elements themselves back.
- Any exception that is thrown is propagated up the call stack, at which point the queue is in a consistent
  state.

Note: If any of your type's copy constructors/move constructors/assignment operators don't throw, be sure
to annotate them with `noexcept`; this will avoid the exception-checking overhead in the queue where possible
(even with zero-cost exceptions, there's still a code size impact that has to be taken into account).

#### Traits

The queue also supports a traits template argument which defines various types, constants,
and the memory allocation and deallocation functions that are to be used by the queue. The typical pattern
to providing your own traits is to create a class that inherits from the default traits
and override only the values you wish to change. Example:

```C++
struct MyTraits : public moodycamel::ConcurrentQueueDefaultTraits
{
 static const size_t BLOCK_SIZE = 256;  // Use bigger blocks
};

moodycamel::ConcurrentQueue<int, MyTraits> q;
```

#### How to dequeue types without calling the constructor

The normal way to dequeue an item is to pass in an existing object by reference, which
is then assigned to internally by the queue (using the move-assignment operator if possible).
This can pose a problem for types that are
expensive to construct or don't have a default constructor; fortunately, there is a simple
workaround: Create a wrapper class that copies the memory contents of the object when it
is assigned by the queue (a poor man's move, essentially). Note that this only works if
the object contains no internal pointers. Example:

```C++
struct MyObjectMover {
    inline void operator=(MyObject&& obj) {
        std::memcpy(data, &obj, sizeof(MyObject));
        
        // TODO: Cleanup obj so that when it's destructed by the queue
        // it doesn't corrupt the data of the object we just moved it into
    }

    inline MyObject& obj() { return *reinterpret_cast<MyObject*>(data); }

private:
    align(alignof(MyObject)) char data[sizeof(MyObject)];
};
```

A less dodgy alternative, if moves are cheap but default construction is not, is to use a
wrapper that defers construction until the object is assigned, enabling use of the move
constructor:

```C++
struct MyObjectMover {
    inline void operator=(MyObject&& x) {
        new (data) MyObject(std::move(x));
        created = true;
    }

    inline MyObject& obj() {
        assert(created);
        return *reinterpret_cast<MyObject*>(data);
    }

    ~MyObjectMover() {
        if (created)
            obj().~MyObject();
    }

private:
    align(alignof(MyObject)) char data[sizeof(MyObject)];
    bool created = false;
};
```

## Samples

There are some more detailed samples [here][samples.md]. The source of
the [unit tests][unittest-src] and [benchmarks][benchmark-src] are available for reference as well.

## Benchmarks

See my blog post for some [benchmark results][benchmarks] (including versus `boost::lockfree::queue` and `tbb::concurrent_queue`),
or run the benchmarks yourself (requires MinGW and certain GnuWin32 utilities to build on Windows, or a recent
g++ on Linux):

```Shell
cd build
make benchmarks
bin/benchmarks
```

The short version of the benchmarks is that it's so fast (especially the bulk methods), that if you're actually
using the queue to *do* anything, the queue won't be your bottleneck.

## Tests (and bugs)

I've written quite a few unit tests as well as a randomized long-running fuzz tester. I also ran the
core queue algorithm through the [CDSChecker][cdschecker] C++11 memory model model checker. Some of the
inner algorithms were tested separately using the [Relacy][relacy] model checker, and full integration
tests were also performed with Relacy.
I've tested
on Linux (Fedora 19) and Windows (7), but only on x86 processors so far (Intel and AMD). The code was
written to be platform-independent, however, and should work across all processors and OSes.

Due to the complexity of the implementation and the difficult-to-test nature of lock-free code in general,
there may still be bugs. If anyone is seeing buggy behaviour, I'd like to hear about it! (Especially if
a unit test for it can be cooked up.) Just open an issue on GitHub.

## Using vcpkg

You can download and install `moodycamel::ConcurrentQueue` using the [vcpkg](https://github.com/Microsoft/vcpkg) dependency manager:

```Shell
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install
vcpkg install concurrentqueue
```

The `moodycamel::ConcurrentQueue` port in vcpkg is kept up to date by Microsoft team members and community contributors. If the version is out of date, please [create an issue or pull request](https://github.com/Microsoft/vcpkg) on the vcpkg repository.

## License

I'm releasing the source of this repository (with the exception of third-party code, i.e. the Boost queue
(used in the benchmarks for comparison), Intel's TBB library (ditto), CDSChecker, Relacy, and Jeff Preshing's
cross-platform semaphore, which all have their own licenses)
under a simplified BSD license. I'm also dual-licensing under the Boost Software License.
See the [LICENSE.md][license] file for more details.

Note that lock-free programming is a patent minefield, and this code may very
well violate a pending patent (I haven't looked), though it does not to my present knowledge.
I did design and implement this queue from scratch.

## Diving into the code

If you're interested in the source code itself, it helps to have a rough idea of how it's laid out. This
section attempts to describe that.

The queue is formed of several basic parts (listed here in roughly the order they appear in the source). There's the
helper functions (e.g. for rounding to a power of 2). There's the default traits of the queue, which contain the
constants and malloc/free functions used by the queue. There's the producer and consumer tokens. Then there's the queue's
public API itself, starting with the constructor, destructor, and swap/assignment methods. There's the public enqueue methods,
which are all wrappers around a small set of private enqueue methods found later on. There's the dequeue methods, which are
defined inline and are relatively straightforward.

Then there's all the main internal data structures. First, there's a lock-free free list, used for recycling spent blocks (elements
are enqueued to blocks internally). Then there's the block structure itself, which has two different ways of tracking whether
it's fully emptied or not (remember, given two parallel consumers, there's no way to know which one will finish first) depending on where it's used.
Then there's a small base class for the two types of internal SPMC producer queues (one for explicit producers that holds onto memory
but attempts to be faster, and one for implicit ones which attempt to recycle more memory back into the parent but is a little slower).
The explicit producer is defined first, then the implicit one. They both contain the same general four methods: One to enqueue, one to
dequeue, one to enqueue in bulk, and one to dequeue in bulk. (Obviously they have constructors and destructors too, and helper methods.)
The main difference between them is how the block handling is done (they both use the same blocks, but in different ways, and map indices
to them in different ways).

Finally, there's the miscellaneous internal methods: There's the ones that handle the initial block pool (populated when the queue is constructed),
and an abstract block pool that comprises the initial pool and any blocks on the free list. There's ones that handle the producer list
(a lock-free add-only linked list of all the producers in the system). There's ones that handle the implicit producer lookup table (which
is really a sort of specialized TLS lookup). And then there's some helper methods for allocating and freeing objects, and the data members
of the queue itself, followed lastly by the free-standing swap functions.

[concurrentqueue]:[https://github.com/cameron314/concurrentqueue]
[NDTalk]:[https://www.gdcvault.com/play/1022186/Parallelizing-the-Naughty-Dog-Engine]

[samples.md]: https://github.com/cameron314/concurrentqueue/blob/master/samples.md
[unittest-src]: https://github.com/cameron314/concurrentqueue/tree/master/tests/unittests
[benchmarks]: http://moodycamel.com/blog/2014/a-fast-general-purpose-lock-free-queue-for-c++#benchmarks
[benchmark-src]: https://github.com/cameron314/concurrentqueue/tree/master/benchmarks
[license]: https://github.com/cameron314/concurrentqueue/blob/master/LICENSE.md
[cdschecker]: http://demsky.eecs.uci.edu/c11modelchecker.html
[relacy]: http://www.1024cores.net/home/relacy-race-detector
