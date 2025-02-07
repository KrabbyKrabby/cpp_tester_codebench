#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using i64 = long long;
using u64 = unsigned long long;
using u32 = unsigned;

i64 gcd(i64 a, i64 b) {
    while (b) {
        a %= b;
        std::swap(a, b);
    }
    return a;
}

i64 countValid(i64 x, i64 m) {
    if (x == 1) return m;
    
    i64 count = 0;
    i64 period = x & (-x); // Lowest set bit
    
    // Handle small values directly
    if (m <= 1000) {
        for (i64 y = 1; y <= m; y++) {
            i64 xor_val = x ^ y;
            if (xor_val % x == 0 || xor_val % y == 0) {
                count++;
            }
        }
        return count;
    }
    
    // Handle larger values using pattern recognition
    i64 full_periods = m / period;
    i64 remainder = m % period;
    
    // Calculate for complete periods
    i64 per_period = 0;
    for (i64 i = 1; i <= period; i++) {
        i64 xor_val = x ^ i;
        if (xor_val % x == 0 || xor_val % i == 0) {
            per_period++;
        }
    }
    count = full_periods * per_period;
    
    // Add remaining numbers
    for (i64 y = full_periods * period + 1; y <= m; y++) {
        i64 xor_val = x ^ y;
        if (xor_val % x == 0 || xor_val % y == 0) {
            count++;
        }
    }
    
    return count;
}

std::vector<i64> solution(std::vector<std::pair<int,i64>> vec) {
    std::vector<i64> result;
    result.reserve(vec.size());
    
    for (const auto& [x, m] : vec) {
        result.push_back(countValid(x, m));
    }
    
    return result;
}
