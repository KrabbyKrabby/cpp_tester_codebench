#include <iostream>
#include <string>
#include <type_traits>
#include <stdexcept>

// Type trait to check if two types can be added using operator+
template<typename T, typename U, typename = void>
struct can_add : std::false_type {};

template<typename T, typename U>
struct can_add<T, U, std::void_t<decltype(std::declval<T>() + std::declval<U>())>> 
    : std::true_type {};

// Helper struct for throwing exceptions
struct UnsupportedOperationThrower {
    template<typename T, typename U>
    static void throw_if_unsupported() {
        throw std::invalid_argument("Unsupported operation for given types");
    }
};

// Main add function template
template<typename T, typename U>
auto add(T&& t, U&& u) -> 
    typename std::enable_if_t<can_add<T, U>::value || can_add<U, T>::value, 
        decltype(std::forward<T>(t) + std::forward<U>(u))> {
    
    if constexpr (std::is_same_v<std::decay_t<T>, char> && 
                  std::is_same_v<std::decay_t<U>, char>) {
        // char + char → string
        return std::string(1, t) + std::string(1, u);
    }
    else if constexpr ((std::is_arithmetic_v<std::decay_t<T>> && 
                       std::is_arithmetic_v<std::decay_t<U>>) &&
                      !(std::is_same_v<std::decay_t<T>, char> && 
                        std::is_same_v<std::decay_t<U>, char>)) {
        // Arithmetic + Arithmetic (including char + int)
        return std::forward<T>(t) + std::forward<U>(u);
    }
    else if constexpr ((std::is_same_v<std::decay_t<T>, char> && 
                       std::is_same_v<std::decay_t<U>, std::string>) ||
                      (std::is_same_v<std::decay_t<T>, std::string> && 
                       std::is_same_v<std::decay_t<U>, char>)) {
        // char + string or string + char
        if constexpr (std::is_same_v<std::decay_t<T>, char>) {
            return std::string(1, t) + std::forward<U>(u);
        } else {
            return std::forward<T>(t) + std::string(1, u);
        }
    }
    else if constexpr (std::is_same_v<std::decay_t<T>, std::string> && 
                      std::is_same_v<std::decay_t<U>, std::string>) {
        // string + string
        return std::forward<T>(t) + std::forward<U>(u);
    }
    else if constexpr (can_add<T, U>::value) {
        // User-defined types with valid operator+
        return std::forward<T>(t) + std::forward<U>(u);
    }
    else {
        UnsupportedOperationThrower::throw_if_unsupported<T, U>();
        return std::forward<T>(t) + std::forward<U>(u); // Never reached
    }
}
