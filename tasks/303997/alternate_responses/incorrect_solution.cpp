#include <iostream>
#include <string>
#include <type_traits>
#include <stdexcept>

// Type trait to check if two types support operator+
template<typename T, typename U, typename = void>
struct can_add : std::false_type {};

template<typename T, typename U>
struct can_add<T, U, std::void_t<decltype(std::declval<T>() + std::declval<U>())>> 
    : std::true_type {};

template<typename T, typename U>
auto add(T&& t, U&& u) {
    using DecayT = std::decay_t<T>;
    using DecayU = std::decay_t<U>;

    if constexpr (std::is_same_v<DecayT, char> && std::is_same_v<DecayU, char>) {
        return std::string(1, t) + std::string(1, u);
    }
    else if constexpr (std::is_arithmetic_v<DecayT> && std::is_arithmetic_v<DecayU>) {
        return std::forward<T>(t) + std::forward<U>(u);
    }
    else if constexpr ((std::is_same_v<DecayT, char> && std::is_integral_v<DecayU>) ||
                      (std::is_integral_v<DecayT> && std::is_same_v<DecayU, char>)) {
        return static_cast<int>(t) + static_cast<int>(u);
    }
    else if constexpr ((std::is_same_v<DecayT, char> && std::is_same_v<DecayU, std::string>) ||
                      (std::is_same_v<DecayT, std::string> && std::is_same_v<DecayU, char>)) {
        if constexpr (std::is_same_v<DecayT, char>) {
            return std::string(1, t) + std::forward<U>(u);
        } else {
            return std::forward<T>(t) + std::string(1, u);
        }
    }
    else if constexpr (std::is_same_v<DecayT, std::string> && std::is_same_v<DecayU, std::string>) {
        return std::forward<T>(t) + std::forward<U>(u);
    }
    else if constexpr (can_add<T, U>::value) {
        return std::forward<T>(t) + std::forward<U>(u);
    }
    else {
        return T{};
    }
}
