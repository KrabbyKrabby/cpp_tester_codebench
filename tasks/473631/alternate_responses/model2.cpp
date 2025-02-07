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

i64 count_divisible(i64 x, i64 m) {
    if (x == 0) return 0;
    
    i64 count = 0;
    i64 block_size = 1LL << (64 - __builtin_clzll(x));
    
    // Handle small values directly
    for (i64 y = 1; y <= std::min(m, block_size); y++) {
        i64 xor_result = x ^ y;
        if (xor_result % x == 0 || xor_result % y == 0) {
            count++;
        }
    }
    
    if (m <= block_size) return count;
    
    // Handle larger values using pattern recognition
    i64 remaining = m - block_size;
    i64 full_blocks = remaining / block_size;
    i64 partial_block = remaining % block_size;
    
    // Calculate pattern repetition
    i64 pattern_count = 0;
    for (i64 y = 0; y < block_size; y++) {
        i64 xor_result = x ^ y;
        if (xor_result % x == 0 || (y != 0 && xor_result % y == 0)) {
            pattern_count++;
        }
    }
    
    count += full_blocks * pattern_count;
    
    // Handle remaining partial block
    for (i64 y = 0; y < partial_block; y++) {
        i64 actual_y = block_size + y;
        i64 xor_result = x ^ actual_y;
        if (xor_result % x == 0 || xor_result % actual_y == 0) {
            count++;
        }
    }
    
    return count;
}

std::vector<i64> solution(std::vector<std::pair<int,i64>> vec) {
    std::vector<i64> result;
    result.reserve(vec.size());
    
    for (const auto& [x, m] : vec) {
        result.push_back(count_divisible(x, m));
    }
    
    return result;
}
