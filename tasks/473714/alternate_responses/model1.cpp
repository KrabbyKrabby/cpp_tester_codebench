#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

std::pair<std::vector<int>, std::vector<int>> maxProductArrangement(std::vector<int> A, std::vector<int> B) {
    int n = A.size();
    
    // Process each position from left to right (highest to lowest priority)
    for (int i = 0; i < n; ++i) {
        // If A[i] < B[i], consider swapping to maintain lexicographic order
        if (A[i] < B[i]) {
            std::swap(A[i], B[i]);
        }
        // If A[i] == B[i], look ahead to make the optimal choice
        else if (A[i] == B[i] && i < n - 1) {
            // Compare next elements to decide whether to swap current elements
            int j = i + 1;
            while (j < n && A[j] == B[j]) {
                ++j;
            }
            if (j < n && A[j] < B[j]) {
                std::swap(A[i], B[i]);
            }
        }
    }
    
    // Final verification to ensure A is lexicographically greater than or equal to B
    bool needsSwap = false;
    for (int i = 0; i < n; ++i) {
        if (A[i] < B[i]) {
            needsSwap = true;
            break;
        }
        else if (A[i] > B[i]) {
            needsSwap = false;
            break;
        }
    }
    
    if (needsSwap) {
        std::swap(A, B);
    }
    
    return std::make_pair(A, B);
}