#include <iostream>
#include <string>
#include <stdexcept>
#include <type_traits>

// Helper trait to check if T + U is valid via operator+
template <typename T, typename U, typename = void>
struct can_add : std::false_type {};

template <typename T, typename U>
struct can_add<T, U, std::void_t<decltype(std::declval<T>() + std::declval<U>())>> : std::true_type {};

// Helper class to throw std::invalid_argument exception for unsupported cases
struct UnsupportedOperationThrower {
    template <typename T, typename U>
    static void throw_if_not_supported(T, U) {
        throw std::invalid_argument("Unsupported operation");
    }
};

// Single generalized template function to handle all cases
template <typename T, typename U>
auto add(T a, U b) {
    if constexpr (std::is_same_v<T, char> && std::is_same_v<U, char>) {
        // char + char → std::string
        return std::string(1, a) + b;
    } else if constexpr (std::is_arithmetic_v<T> && std::is_arithmetic_v<U>) {
        // Arithmetic + Arithmetic (int, float, double, etc.)
        return a + b;
    } else if constexpr (std::is_same_v<T, char> && std::is_integral_v<U>) {
        // char + int → ASCII sum (int)
        return static_cast<int>(a) + b;
    } else if constexpr (std::is_integral_v<T> && std::is_same_v<U, char>) {
        // int + char → ASCII sum (int)
        return a + static_cast<int>(b);
    } else if constexpr (std::is_same_v<T, char> && std::is_same_v<U, std::string>) {
        // char + std::string → Concatenation (std::string)
        return std::string(1, a) + b;
    } else if constexpr (std::is_same_v<T, std::string> && std::is_same_v<U, char>) {
        // std::string + char → Concatenation (std::string)
        return a + b;
    } else if constexpr (std::is_same_v<T, std::string> && std::is_same_v<U, std::string>) {
        // std::string + std::string → Concatenation (std::string)
        return a + b;
    } else if constexpr (can_add<T, U>::value) {
        // User-Defined Types (T + U valid via operator+)
        return a + b;
    } else {
        // Unsupported operation → Runtime Exception
        UnsupportedOperationThrower::throw_if_not_supported(a, b);
    }
}

