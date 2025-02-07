#include <iostream>
#include <vector>

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
    
    m++;

    int n = 1;
    while (n <= x) {
        n *= 2;
    }

    i64 ans = 0;
    for (i64 y = 1; y < std::min<i64>(n, m); y++) {
        i64 z = x ^ y;
        if (z % x == 0 || z % y == 0) {
            ans++;
        }
    }
    for (int y = 0; y < n; y++) {
        i64 z = x ^ y;
        i64 r = m / n + (y < m% n) - 1;
        if (r <= 0) {
            continue;
        }

        i64 u, v;
        i64 g = exgcd(n, x, u, v);
        if (z % g != 0) {
            continue;
        }
        i64 m = x / g;
        u = u * (-z / g) % m;
        if (u < 0) {
            u += m;
        }
        r++;
        ans += r / m;
        ans += u < r% m;
        if (u == 0) {
            ans--;
        }
    }

    return ans;
}

std::vector<i64> solution(std::vector<std::pair<int,i64>> vec) {
    std::vector<i64> res;
    for (auto p : vec) {
        res.push_back(solve(p.first, p.second));
    }
    return res;
}