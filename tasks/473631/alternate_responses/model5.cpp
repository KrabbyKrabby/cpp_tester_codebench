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

i64 countValid(int x, i64 m) {
    if (x == 1) return m;
    
    i64 count = 0;
    i64 sqrtM = std::sqrt(m);
    
    // Handle small values directly
    for (i64 y = 1; y <= std::min(sqrtM, (i64)x); y++) {
        if (m < y) break;
        i64 xorResult = x ^ y;
        if (xorResult % x == 0 || xorResult % y == 0) {
            count++;
        }
    }
    
    // Handle larger values using mathematical properties
    for (i64 i = 1; i <= m / x; i++) {
        i64 start = std::max((i - 1) * x + 1, sqrtM + 1);
        i64 end = std::min(i * x, m);
        if (start > end) continue;
        
        count += end - start + 1;
    }
    
    // Adjust for overlapping counts
    for (i64 y = sqrtM + 1; y <= m; y++) {
        i64 xorResult = x ^ y;
        if (y > x && xorResult % y == 0) {
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
