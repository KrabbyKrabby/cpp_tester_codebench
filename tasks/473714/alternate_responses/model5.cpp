#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

// Helper function to convert vector to number
long long vectorToNumber(const std::vector<int>& vec) {
    long long result = 0;
    for (int num : vec) {
        result = result * 10 + num;
    }
    return result;
}

// Helper function to check if vec1 is lexicographically greater than or equal to vec2
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
    std::vector<int> bestA = A;
    std::vector<int> bestB = B;
    long long maxProduct = vectorToNumber(A) * vectorToNumber(B);
    
    // Try all possible swaps at each position
    for (int i = 0; i < n; ++i) {
        std::vector<int> tempA = A;
        std::vector<int> tempB = B;
        
        // Perform swap at position i
        std::swap(tempA[i], tempB[i]);
        
        // Calculate new product
        long long newProduct = vectorToNumber(tempA) * vectorToNumber(tempB);
        
        // Update if new arrangement gives better product and maintains lexicographical order
        if (newProduct > maxProduct && isLexicographicallyGreaterOrEqual(tempA, tempB)) {
            maxProduct = newProduct;
            bestA = tempA;
            bestB = tempB;
        }
    }
    
    // If the initial arrangement doesn't satisfy lexicographical order, swap the vectors
    if (!isLexicographicallyGreaterOrEqual(bestA, bestB)) {
        std::swap(bestA, bestB);
    }
    
    return {bestA, bestB};
}
