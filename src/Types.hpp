#pragma once

#include <cstdint>

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

    template <typename T, typename tag>
    struct StrongType {
        constexpr explicit StrongType<T, tag>(T value)
            : m_Value(value) {}

        constexpr const T& Get() const noexcept {
            return m_Value;
        }

        constexpr StrongType operator+(StrongType rhs) const {
            return StrongType{m_Value + rhs.m_Value};
        }

        constexpr StrongType operator-(StrongType rhs) const {
            return StrongType{m_Value - rhs.m_Value};
        }

        constexpr auto operator<=>(const StrongType&) const = default;

        T m_Value;
    };

    using ThreadAffinity = StrongType<u8, struct ThreadTag>;
    using Tag = StrongType<u8, struct TagTag>;


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
