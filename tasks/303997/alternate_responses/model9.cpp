#include <iostream>
#include <string>
#include <stdexcept>
#include <type_traits>
#include <utility>

// Helper trait to check if operator+ is defined for T and U
template <typename T, typename U>
struct can_add {
    template <typename T1, typename U1>
    static auto test(T1&& t, U1&& u) -> decltype(std::forward<T1>(t) + std::forward<U1>(u), void(), std::true_type{});
    static auto test(...) -> std::false_type;
    static constexpr bool value = decltype(test(std::declval<T>(), std::declval<U>()))::value;
};

// Helper class to throw std::invalid_argument exception for unsupported operations
class UnsupportedOperationThrower {
public:
    template <typename T, typename U>
    static void throwIfUnsupported(T, U) {
        throw std::invalid_argument("Unsupported operation");
    }
};

// Generalized add function
template <typename T, typename U>
auto add(T t, U u) {
    if constexpr (std::is_same_v<T, char> && std::is_same_v<U, char>) {
        // char + char -> std::string
        return std::string(1, t) + u;
    } else if constexpr (std::is_arithmetic_v<T> && std::is_arithmetic_v<U>) {
        // Arithmetic + Arithmetic
        return t + u;
    } else if constexpr (std::is_same_v<T, char> && std::is_integral_v<U>) {
        // char + int -> ASCII sum
        return static_cast<int>(t) + u;
    } else if constexpr (std::is_integral_v<T> && std::is_same_v<U, char>) {
        // int + char -> ASCII sum
        return t + static_cast<int>(u);
    } else if constexpr (std::is_same_v<T, char> && std::is_same_v<U, std::string>) {
        // char + std::string -> Concatenation
        return std::string(1, t) + u;
    } else if constexpr (std::is_same_v<T, std::string> && std::is_same_v<U, char>) {
        // std::string + char -> Concatenation
        return t + u;
    } else if constexpr (std::is_same_v<T, std::string> && std::is_same_v<U, std::string>) {
        // std::string + std::string -> Concatenation
        return t + u;
    } else if constexpr (can_add<T, U>::value) {
        // User-Defined Types (T + U valid via operator+)
        return t + u;
    } else {
        // Unsupported operations
        UnsupportedOperationThrower::throwIfUnsupported(t, u);
    }
}

