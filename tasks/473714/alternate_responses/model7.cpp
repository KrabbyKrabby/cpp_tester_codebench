#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

std::pair<std::vector<int>, std::vector<int>> maxProductArrangement(std::vector<int> A, std::vector<int> B) {
    int n = A.size();
    std::vector<std::vector<std::pair<int, int>>> dp(n + 1, std::vector<std::pair<int, int>>(2, {-1, -1}));
    dp[0][0] = {1, 0};
    dp[0][1] = {1, 0};
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 2; j++) {
            if (dp[i][j].first == -1) continue;
            if (j == 0) {
                dp[i + 1][0] = std::max(dp[i + 1][0], {dp[i][j].first * 10 + A[i], dp[i][j].second});
                dp[i + 1][1] = std::max(dp[i + 1][1], {dp[i][j].first * 10 + B[i], dp[i][j].second + 1});
            } else {
                dp[i + 1][0] = std::max(dp[i + 1][0], {dp[i][j].first * 10 + B[i], dp[i][j].second + 1});
                dp[i + 1][1] = std::max(dp[i + 1][1], {dp[i][j].first * 10 + A[i], dp[i][j].second});
            }
        }
    }
    
    std::vector<int> batch1, batch2;
    int cur = dp[n][0] > dp[n][1] ? 0 : 1;
    for (int i = n - 1; i >= 0; i--) {
        if (cur == 0) {
            if (dp[i + 1][0].first == dp[i][0].first * 10 + A[i]) {
                batch1.push_back(A[i]);
                cur = 0;
            } else {
                batch1.push_back(B[i]);
                cur = 1;
            }
        } else {
            if (dp[i + 1][1].first == dp[i][1].first * 10 + A[i]) {
                batch2.push_back(A[i]);
                cur = 1;
            } else {
                batch2.push_back(B[i]);
                cur = 0;
            }
        }
    }
    
    std::reverse(batch1.begin(), batch1.end());
    std::reverse(batch2.begin(), batch2.end());
    
    return {batch1, batch2};
}
