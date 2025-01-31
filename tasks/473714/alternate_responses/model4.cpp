#include <iostream>
#include <vector>
#include <utility>
#include <cmath>

// Helper function to calculate the number formed by vector elements
long long vectorToNumber(const std::vector<int>& vec) {
    long long result = 0;
    for (int num : vec) {
        result = result * 10 + num;
    }
    return result;
}

// Helper function to compare two vectors lexicographically
bool isLexicographicallyGreaterOrEqual(const std::vector<int>& vec1, const std::vector<int>& vec2) {
    for (size_t i = 0; i < vec1.size(); ++i) {
        if (vec1[i] != vec2[i]) {
            return vec1[i] > vec2[i];
        }
    }
    return true;
}

std::pair<std::vector<int>, std::vector<int>> maxProductArrangement(std::vector<int> A, std::vector<int> B) {
    int n = A.size();
    std::vector<int> originalA = A;
    std::vector<int> originalB = B;
    long long maxProduct = vectorToNumber(A) * vectorToNumber(B);
    std::vector<int> bestA = A;
    std::vector<int> bestB = B;
    
    // Try swapping at each position
    for (int i = 0; i < n; ++i) {
        std::swap(A[i], B[i]);
        
        // Check if this arrangement is better and maintains lexicographical order
        long long currentProduct = vectorToNumber(A) * vectorToNumber(B);
        if (currentProduct > maxProduct && isLexicographicallyGreaterOrEqual(A, B)) {
            maxProduct = currentProduct;
            bestA = A;
            bestB = B;
        }
        
        // Restore original values for next iteration
        A[i] = originalA[i];
        B[i] = originalB[i];
    }
    
    // If the original arrangement doesn't satisfy lexicographical order, swap the vectors
    if (!isLexicographicallyGreaterOrEqual(bestA, bestB)) {
        std::swap(bestA, bestB);
    }
    
    return std::make_pair(bestA, bestB);
}
