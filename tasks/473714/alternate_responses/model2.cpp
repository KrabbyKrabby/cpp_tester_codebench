#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

// Helper function to calculate the number formed by a vector
long long getNumber(const std::vector<int>& vec) {
    long long num = 0;
    for (int digit : vec) {
        num = num * 10 + digit;
    }
    return num;
}

// Helper function to check if vec1 is lexicographically greater than or equal to vec2
bool isLexicographicallyGreaterOrEqual(const std::vector<int>& vec1, const std::vector<int>& vec2) {
    for (size_t i = 0; i < vec1.size(); ++i) {
        if (vec1[i] != vec2[i]) {
            return vec1[i] > vec2[i];
        }
    }
    return true;  // vectors are equal
}

std::pair<std::vector<int>, std::vector<int>> maxProductArrangement(std::vector<int> A, std::vector<int> B) {
    int n = A.size();
    std::vector<int> bestA = A;
    std::vector<int> bestB = B;
    long long maxProduct = getNumber(A) * getNumber(B);
    
    // Try swapping elements at each position
    for (int i = 0; i < n; ++i) {
        std::vector<int> tempA = A;
        std::vector<int> tempB = B;
        
        // Swap elements at position i
        std::swap(tempA[i], tempB[i]);
        
        // Check if this arrangement is valid (maintains lexicographical order)
        if (isLexicographicallyGreaterOrEqual(tempA, tempB)) {
            long long currentProduct = getNumber(tempA) * getNumber(tempB);
            
            // Update best arrangement if current product is better
            if (currentProduct > maxProduct) {
                maxProduct = currentProduct;
                bestA = tempA;
                bestB = tempB;
            }
        }
    }
    
    return std::make_pair(bestA, bestB);
}