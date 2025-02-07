#include <iostream>
#include <string>
#include <type_traits>
#include <stdexcept>

// Type trait to check if operator+ exists between types
template<typename T, typename U, typename = void>
struct can_add : std::false_type {};

template<typename T, typename U>
struct can_add<T, U, std::void_t<decltype(std::declval<T>() + std::declval<U>())>> 
    : std::true_type {};

// Helper struct for throwing exceptions
struct UnsupportedOperationThrower {
    template<typename T, typename U>
    static void throw_if_invalid() {
        throw std::invalid_argument("Unsupported operation between provided types");
    }
};

// Main add function template
template<typename T, typename U>
auto add(T&& t, U&& u) -> decltype(auto) {
    using DecayT = std::decay_t<T>;
    using DecayU = std::decay_t<U>;
    
    if constexpr (std::is_same_v<DecayT, char> && std::is_same_v<DecayU, char>) {
        // char + char → string
        return std::string(1, t) + std::string(1, u);
    }
    else if constexpr (std::is_arithmetic_v<DecayT> && std::is_arithmetic_v<DecayU>) {
        // arithmetic + arithmetic
        return std::forward<T>(t) + std::forward<U>(u);
    }
    else if constexpr ((std::is_same_v<DecayT, char> && std::is_integral_v<DecayU>) ||
                      (std::is_integral_v<DecayT> && std::is_same_v<DecayU, char>)) {
        // char + int or int + char → ASCII sum
        return static_cast<int>(t) + static_cast<int>(u);
    }
    else if constexpr ((std::is_same_v<DecayT, char> && std::is_same_v<DecayU, std::string>) ||
                      (std::is_same_v<DecayT, std::string> && std::is_same_v<DecayU, char>)) {
        // char + string or string + char
        if constexpr (std::is_same_v<DecayT, char>) {
            return std::string(1, t) + std::forward<U>(u);
        } else {
            return std::forward<T>(t) + std::string(1, u);
        }
    }
    else if constexpr (std::is_same_v<DecayT, std::string> && std::is_same_v<DecayU, std::string>) {
        // string + string
        return std::forward<T>(t) + std::forward<U>(u);
    }
    else if constexpr (can_add<T, U>::value) {
        // User-defined types with valid operator+
        return std::forward<T>(t) + std::forward<U>(u);
    }
    else {
        // Unsupported operation
        UnsupportedOperationThrower::throw_if_invalid<T, U>();
        // This return is never reached but needed for compilation
        return std::forward<T>(t);
    }
}
