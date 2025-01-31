#include <iostream>
#include <vector>
#include <utility>

std::pair<std::vector<int>, std::vector<int>> maxProductArrangement(std::vector<int> A, std::vector<int> B) {
    int n = A.size();
    std::vector<int> batch1 = A, batch2 = B;
    for (int i = 0; i < n; ++i) {
        if (batch1[i] < batch2[i]) {
            std::swap(batch1[i], batch2[i]);
        }
    }
    return std::make_pair(batch1, batch2);
}