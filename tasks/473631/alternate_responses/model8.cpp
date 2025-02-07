#include <iostream>
#include <vector>
#include <unordered_map>

using i64 = long long;
using u64 = unsigned long long;
using u32 = unsigned;

// Function to calculate the greatest common divisor (GCD) using the Euclidean algorithm
i64 gcd(i64 a, i64 b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

// Function to calculate the number of integers y that satisfy the condition
i64 solve(int x, i64 m) {
    if (x > m) return 0; // Early exit condition

    // Calculate the number of multiples of x within the range [1, m]
    i64 count = m / x;

    // Calculate the number of integers y such that x XOR y is divisible by x
    for (i64 y = 1; y <= m; y++) {
        if ((x ^ y) % x == 0) {
            count++;
        }
    }

    return count;
}

// Function to calculate the results for a vector of pairs
std::vector<i64> solution(std::vector<std::pair<int, i64>> vec) {
    std::vector<i64> res;
    std::unordered_map<std::pair<int, i64>, i64> memo; // Memoization map

    for (auto p : vec) {
        // Check if the result is already memoized
        if (memo.find(p) != memo.end()) {
            res.push_back(memo[p]);
        } else {
            // Calculate the result and memoize it
            i64 result = solve(p.first, p.second);
            memo[p] = result;
            res.push_back(result);
        }
    }

    return res;
}
