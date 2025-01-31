#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

std::pair<std::vector<int>, std::vector<int>> maxProductArrangement(std::vector<int> A, std::vector<int> B) {
    int n = A.size();
    std::vector<int> batch1 = A;
    std::vector<int> batch2 = B;
    
    // Process each position from left to right
    for (int i = 0; i < n; ++i) {
        // If swapping at current position would result in a better arrangement
        // while maintaining lexicographical order, perform the swap
        if (batch2[i] > batch1[i]) {
            // Before swapping, check if it maintains lexicographical order
            bool canSwap = true;
            std::swap(batch1[i], batch2[i]);
            
            // Check if batch1 is still lexicographically greater than or equal to batch2
            for (int j = 0; j < n; ++j) {
                if (batch1[j] < batch2[j]) {
                    canSwap = false;
                    break;
                }
                if (batch1[j] > batch2[j]) {
                    break;
                }
            }
            
            // If swap violates lexicographical order, revert it
            if (!canSwap) {
                std::swap(batch1[i], batch2[i]);
            }
        }
    }
    
    return std::make_pair(batch1, batch2);
}
