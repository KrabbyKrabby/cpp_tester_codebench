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

// Helper to check if type is arithmetic (excluding char)
template<typename T>
struct is_arithmetic_not_char : 
    std::conjunction<
        std::is_arithmetic<T>,
        std::negation<std::is_same<T, char>>
    > {};

// Primary template for add function
template<typename T, typename U>
auto add(T&& t, U&& u) {
    using clean_T = std::remove_cvref_t<T>;
    using clean_U = std::remove_cvref_t<U>;
    
    if constexpr (std::is_same_v<clean_T, char> && std::is_same_v<clean_U, char>) {
        // char + char → string
        return std::string(1, t) + u;
    }
    else if constexpr (std::conjunction_v<is_arithmetic_not_char<clean_T>, 
                                        is_arithmetic_not_char<clean_U>>) {
        // arithmetic + arithmetic
        return std::forward<T>(t) + std::forward<U>(u);
    }
    else if constexpr ((std::is_same_v<clean_T, char> && std::is_integral_v<clean_U>) ||
                      (std::is_integral_v<clean_T> && std::is_same_v<clean_U, char>)) {
        // char + int or int + char → ASCII sum
        return static_cast<int>(t) + static_cast<int>(u);
    }
    else if constexpr (std::is_same_v<clean_T, std::string> && std::is_same_v<clean_U, std::string>) {
        // string + string → concatenation
        return std::forward<T>(t) + std::forward<U>(u);
    }
    else if constexpr ((std::is_same_v<clean_T, char> && std::is_same_v<clean_U, std::string>) ||
                      (std::is_same_v<clean_T, std::string> && std::is_same_v<clean_U, char>)) {
        // char + string or string + char → concatenation
        if constexpr (std::is_same_v<clean_T, char>) {
            return std::string(1, t) + std::forward<U>(u);
        } else {
            return std::forward<T>(t) + std::string(1, u);
        }
    }
    else if constexpr (can_add<T, U>::value) {
        // User-defined types with valid operator+
        return std::forward<T>(t) + std::forward<U>(u);
    }
    else {
        throw std::invalid_argument("Unsupported operation for the given types");
    }
}
