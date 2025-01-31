#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <algorithm>

std::pair<std::vector<int>, std::vector<int>> maxProductArrangement(std::vector<int> A, std::vector<int> B) {
    std::vector<std::pair<int, int>> pairs;

    // Combine the elements of A and B into pairs
    for (int i = 0; i < A.size(); i++) {
        pairs.push_back(std::make_pair(A[i], B[i]));
    }

    // Sort the pairs in descending order based on the maximum value in each pair
    std::sort(pairs.begin(), pairs.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return std::max(a.first, a.second) > std::max(b.first, b.second);
    });

    // Initialize the result vectors
    std::vector<int> batch1(A.size());
    std::vector<int> batch2(A.size());

    // Distribute the pairs into the two batches
    for (int i = 0; i < pairs.size(); i++) {
        if (i % 2 == 0) {
            batch1[i / 2] = std::max(pairs[i].first, pairs[i].second);
            batch2[i / 2] = std::min(pairs[i].first, pairs[i].second);
        } else {
            batch1[i / 2] = std::min(pairs[i].first, pairs[i].second);
            batch2[i / 2] = std::max(pairs[i].first, pairs[i].second);
        }
    }

    // Ensure batch1 is lexicographically greater than or equal to batch2
    if (batch1 < batch2) {
        std::swap(batch1, batch2);
    }

    return std::make_pair(batch1, batch2);
}
