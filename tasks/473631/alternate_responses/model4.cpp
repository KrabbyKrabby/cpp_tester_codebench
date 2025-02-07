#include <iostream>
#include <vector>
#include <algorithm>

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

i64 solve_optimized(int x, i64 m) {
    if (x == 1) return m;
    
    i64 count = 0;
    i64 block_size = x;
    i64 full_blocks = m / block_size;
    i64 remainder = m % block_size;
    
    // Process full blocks
    if (full_blocks > 0) {
        // Count numbers divisible by x in each block
        count += full_blocks * (block_size / gcd(x, block_size));
        
        // Add contribution from numbers that divide their XOR with x
        for (int i = 1; i < x; i++) {
            i64 xor_val = x ^ i;
            if (xor_val % i == 0 || xor_val % x == 0) {
                count += full_blocks;
            }
        }
    }
    
    // Process remaining numbers
    for (i64 i = 1; i <= remainder; i++) {
        i64 xor_val = x ^ i;
        if (xor_val % x == 0 || xor_val % i == 0) {
            count++;
        }
    }
    
    return count;
}

std::vector<i64> solution(std::vector<std::pair<int,i64>> vec) {
    std::vector<i64> result;
    result.reserve(vec.size());
    
    for (const auto& p : vec) {
        result.push_back(solve_optimized(p.first, p.second));
    }
    
    return result;
}
