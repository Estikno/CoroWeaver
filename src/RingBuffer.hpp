#pragma once

#include <atomic>
#include <array>
#include <semaphore>

#include "Types.hpp"

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
