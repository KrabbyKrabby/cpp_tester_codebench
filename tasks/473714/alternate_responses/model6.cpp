#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

std::pair<std::vector<int>, std::vector<int>> maxProductArrangement(std::vector<int> A, std::vector<int> B) {
    std::vector<int> batch1, batch2;
    int n = A.size();

    for (int i = 0; i < n; i++) {
        if (A[i] > B[i]) {
            batch1.push_back(A[i]);
            batch2.push_back(B[i]);
        } else {
            batch1.push_back(B[i]);
            batch2.push_back(A[i]);
        }
    }

    return std::make_pair(batch1, batch2);
}