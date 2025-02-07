#include <iostream>
#include <string>
#include <stdexcept>
#include <type_traits>

// Helper trait to check if operator+ is defined for T and U
template <typename T, typename U, typename = void>
struct can_add : std::false_type {};

template <typename T, typename U>
struct can_add<T, U, std::void_t<decltype(std::declval<T>() + std::declval<U>())>>
    : std::true_type {};

// Helper class to throw std::invalid_argument exception for unsupported operations
class UnsupportedOperationThrower {
public:
    template <typename T, typename U>
    auto operator()(T, U) const {
        throw std::invalid_argument("Unsupported operation");
    }
};

// Generalized add function template
template <typename T, typename U>
auto add(T a, U b) {
    // Check for unsupported operations and throw exception
    if constexpr (!can_add<T, U>::value) {
        return UnsupportedOperationThrower()(a, b);
    }

    // Handle char + char concatenation
    if constexpr (std::is_same_v<T, char> && std::is_same_v<U, char>) {
        return std::string(1, a) + b;
    }

    // Handle char + int (or vice versa) ASCII sum
    if constexpr ((std::is_same_v<T, char> && std::is_integral_v<U>) ||
                  (std::is_integral_v<T> && std::is_same_v<U, char>)) {
        return static_cast<int>(a) + static_cast<int>(b);
    }

    // Handle char + std::string (or vice versa) concatenation
    if constexpr ((std::is_same_v<T, char> && std::is_same_v<U, std::string>) ||
                  (std::is_same_v<T, std::string> && std::is_same_v<U, char>)) {
        return std::string(1, a) + b;
    }

    // Handle std::string + std::string concatenation
    if constexpr (std::is_same_v<T, std::string> && std::is_same_v<U, std::string>) {
        return a + b;
    }

    // Handle arithmetic + arithmetic operations
    if constexpr (std::is_arithmetic_v<T> && std::is_arithmetic_v<U>) {
        return a + b;
    }

    // Handle user-defined types with operator+
    return a + b;
}

