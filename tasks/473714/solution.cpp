#include <iostream>
#include <vector>
#include <utility>

std::pair<std::vector<int>, std::vector<int>> maxProductArrangement(std::vector<int> A, std::vector<int> B){
    
    // Tracks whether the first difference between A and B has been encountered
    bool flag = false;
    

    for (int i = 0; i < A.size(); ++i) {
      // Swap if the current digit arrangement met required condition
      if ((A[i] > B[i]) == flag) {
        std::swap(A[i], B[i]);
      }

      // Update the flag after encountering the first difference
      flag |= (A[i] != B[i]);
    }
    return {A, B};
}