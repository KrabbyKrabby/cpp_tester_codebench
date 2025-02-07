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

// Helper class for throwing exceptions during template instantiation
class UnsupportedOperationThrower {
public:
    template<typename T, typename U>
    static void throw_if_invalid() {
        throw std::invalid_argument("Unsupported operation for types");
    }
};

// Main add template function
template<typename T, typename U>
auto add(T&& t, U&& u) {
    using DecayT = std::decay_t<T>;
    using DecayU = std::decay_t<U>;
    
    if constexpr (std::is_arithmetic_v<DecayT> && std::is_arithmetic_v<DecayU>) {
        // Case 2: Arithmetic + Arithmetic
        return std::forward<T>(t) + std::forward<U>(u);
    }
    else if constexpr (std::is_same_v<DecayT, char> && std::is_same_v<DecayU, char>) {
        // Case 1: char + char → string
        return std::string(1, t) + std::string(1, u);
    }
    else if constexpr ((std::is_same_v<DecayT, char> && std::is_integral_v<DecayU>) ||
                      (std::is_integral_v<DecayT> && std::is_same_v<DecayU, char>)) {
        // Case 3: char + int or int + char → ASCII sum
        return static_cast<int>(t) + static_cast<int>(u);
    }
    else if constexpr (std::is_same_v<DecayT, std::string> && std::is_same_v<DecayU, std::string>) {
        // Case 5: string + string → string
        return std::forward<T>(t) + std::forward<U>(u);
    }
    else if constexpr ((std::is_same_v<DecayT, char> && std::is_same_v<DecayU, std::string>) ||
                      (std::is_same_v<DecayT, std::string> && std::is_same_v<DecayU, char>)) {
        // Case 4: char + string or string + char → string
        if constexpr (std::is_same_v<DecayT, char>) {
            return std::string(1, t) + std::forward<U>(u);
        } else {
            return std::forward<T>(t) + std::string(1, u);
        }
    }
    else if constexpr (can_add<DecayT, DecayU>::value) {
        // Case 6: User-defined types with valid operator+
        return std::forward<T>(t) + std::forward<U>(u);
    }
    else {
        // Unsupported operation
        UnsupportedOperationThrower::throw_if_invalid<T, U>();
        // This return is never reached but needed for compilation
        return T{};
    }
}
