#include <iostream>
#include <vector>
#include <utility>

using i64 = long long;
using u64 = unsigned long long;
using u32 = unsigned;

i64 exgcd(i64 a, i64 b, i64& x, i64& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    i64 g = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return g;
}

i64 solve(int x, i64 m) {
    i64 ans = 0;
    for (i64 y = 1; y <= m; y++) {
        i64 z = x ^ y;
        if (z % x == 0 || z % y == 0) {
            ans++;
        }
    }
    return ans;
}

std::vector<i64> solution(std::vector<std::pair<int, i64>> vec) {
    std::vector<i64> res;
    for (auto p : vec) {
        res.push_back(solve(p.first, p.second));
    }
    return res;
}
