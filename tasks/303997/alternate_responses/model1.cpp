#include <iostream>
#include <string>
#include <type_traits>
#include <stdexcept>

// Type trait to check if T + U is valid
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

// Main add function template
template<typename T, typename U>
auto add(T&& t, U&& u) {
    using DecayT = std::decay_t<T>;
    using DecayU = std::decay_t<U>;
    
    // Case 1: char + char → string
    if constexpr (std::is_same_v<DecayT, char> && std::is_same_v<DecayU, char>) {
        return std::string(1, t) + std::string(1, u);
    }
    
    // Case 2: Arithmetic + Arithmetic
    else if constexpr (std::is_arithmetic_v<DecayT> && std::is_arithmetic_v<DecayU>) {
        if constexpr (std::is_same_v<DecayT, char> && std::is_integral_v<DecayU>) {
            return static_cast<int>(t) + u;  // Case 3: char + int
        }
        else if constexpr (std::is_integral_v<DecayT> && std::is_same_v<DecayU, char>) {
            return t + static_cast<int>(u);  // Case 3: int + char
        }
        else {
            return std::forward<T>(t) + std::forward<U>(u);
        }
    }
    
    // Case 4: char + string or string + char
    else if constexpr ((std::is_same_v<DecayT, char> && std::is_same_v<DecayU, std::string>) ||
                      (std::is_same_v<DecayT, std::string> && std::is_same_v<DecayU, char>)) {
        if constexpr (std::is_same_v<DecayT, char>) {
            return std::string(1, t) + std::forward<U>(u);
        }
        else {
            return std::forward<T>(t) + std::string(1, u);
        }
    }
    
    // Case 5: string + string
    else if constexpr (std::is_same_v<DecayT, std::string> && std::is_same_v<DecayU, std::string>) {
        return std::forward<T>(t) + std::forward<U>(u);
    }
    
    // Case 6: User-defined types with operator+
    else if constexpr (can_add<T, U>::value) {
        return std::forward<T>(t) + std::forward<U>(u);
    }
    
    // Unsupported operations
    else {
        UnsupportedOperationThrower::throw_if_invalid<T, U>();
        return T{}; // Never reached, just to satisfy compiler
    }
}
