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

i64 countValidY(int x, i64 m) {
    if (x == 1) return m;
    
    i64 count = 0;
    i64 sqrtM = std::sqrt(m);
    
    // Handle small values directly
    for (i64 y = 1; y <= std::min(sqrtM, (i64)x); y++) {
        if ((x ^ y) % x == 0 || (x ^ y) % y == 0) {
            count++;
        }
    }
    
    // Use mathematical properties for larger values
    for (i64 i = 1; i * i <= m; i++) {
        i64 y = m / i;
        if (y <= x) continue;
        
        i64 g = gcd(x, y);
        if (g > 1) {
            count += (m / y - m / (y + 1));
        }
        
        if ((x ^ y) % x == 0 || (x ^ y) % y == 0) {
            count++;
        }
    }
    
    return count;
}

std::vector<i64> solution(std::vector<std::pair<int,i64>> vec) {
    std::vector<i64> result;
    result.reserve(vec.size());
    
    for (const auto& [x, m] : vec) {
        result.push_back(countValidY(x, m));
    }
    
    return result;
}
