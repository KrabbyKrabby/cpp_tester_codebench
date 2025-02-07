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

// Helper class to throw an exception for unsupported operations
struct UnsupportedOperationThrower {
    template <typename T, typename U>
    auto operator()(T, U) const {
        throw std::invalid_argument("Unsupported operation");
    }
};

// Single generalized template function for add()
template <typename T, typename U,
          std::enable_if_t<can_add<T, U>::value>* = nullptr>
auto add(T a, U b) {
    if constexpr (std::is_same_v<T, char> && std::is_same_v<U, char>) {
        // char + char → std::string
        return std::string(1, a) + b;
    } else if constexpr (std::is_arithmetic_v<T> && std::is_arithmetic_v<U>) {
        // Arithmetic + Arithmetic
        return a + b;
    } else if constexpr ((std::is_same_v<T, char> && std::is_integral_v<U>) ||
                         (std::is_integral_v<T> && std::is_same_v<U, char>)) {
        // char + int (or vice versa) → ASCII sum
        return static_cast<int>(a) + static_cast<int>(b);
    } else if constexpr ((std::is_same_v<T, char> && std::is_same_v<U, std::string>) ||
                         (std::is_same_v<T, std::string> && std::is_same_v<U, char>)) {
        // char + std::string (or vice versa) → Concatenation
        return std::string(1, a) + b;
    } else if constexpr (std::is_same_v<T, std::string> && std::is_same_v<U, std::string>) {
        // std::string + std::string → Concatenation
        return a + b;
    } else {
        // User-Defined Types (T + U valid via operator+)
        return a + b;
    }
}

// Specialization for unsupported operations
template <typename T, typename U,
          std::enable_if_t<!can_add<T, U>::value>* = nullptr>
auto add(T a, U b) {
    return UnsupportedOperationThrower()(a, b);
}

