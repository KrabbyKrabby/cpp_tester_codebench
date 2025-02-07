#include <iostream>
#include <string>
#include <stdexcept>
#include <type_traits>

// Helper trait to check if T and U can be added using operator+
template <typename T, typename U>
struct can_add {
    template <typename = decltype(std::declval<T>() + std::declval<U>())>
    static auto test(int) -> std::true_type;

    static auto test(...) -> std::false_type;

    static constexpr bool value = decltype(test(0))::value;
};

// Throw helper class to ensure correct compilation
template <typename T, typename U>
struct UnsupportedOperationThrower {
    static void throwIfUnsupported() {
        if constexpr (!can_add<T, U>::value) {
            throw std::invalid_argument("Unsupported operation");
        }
    }
};

// Single generalized template function for add()
template <typename T, typename U>
auto add(T a, U b) {
    // Check if operation is supported
    UnsupportedOperationThrower<T, U>::throwIfUnsupported();

    // Handle char + char
    if constexpr (std::is_same_v<T, char> && std::is_same_v<U, char>) {
        return std::string(1, a) + b;
    }
    // Handle arithmetic + arithmetic
    else if constexpr (std::is_arithmetic_v<T> && std::is_arithmetic_v<U>) {
        return a + b;
    }
    // Handle char + int (or vice versa)
    else if constexpr ((std::is_same_v<T, char> && std::is_integral_v<U>) ||
                       (std::is_integral_v<T> && std::is_same_v<U, char>)) {
        return static_cast<int>(a) + static_cast<int>(b);
    }
    // Handle char + std::string (or vice versa)
    else if constexpr ((std::is_same_v<T, char> && std::is_same_v<U, std::string>) ||
                       (std::is_same_v<T, std::string> && std::is_same_v<U, char>)) {
        return std::string(1, a) + b;
    }
    // Handle std::string + std::string
    else if constexpr (std::is_same_v<T, std::string> && std::is_same_v<U, std::string>) {
        return a + b;
    }
    // Handle user-defined types
    else {
        return a + b;
    }
}

