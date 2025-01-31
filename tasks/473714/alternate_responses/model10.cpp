#include <iostream>
#include <vector>
#include <utility>

std::pair<std::vector<int>, std::vector<int>> maxProductArrangement(std::vector<int> A, std::vector<int> B) {
    int n = A.size();
    std::vector<int> batch1(n), batch2(n);
    bool swapped = false;

    for (int i = 0; i < n; i++) {
        if (A[i] > B[i]) {
            batch1[i] = A[i];
            batch2[i] = B[i];
        } else if (A[i] < B[i]) {
            batch1[i] = B[i];
            batch2[i] = A[i];
            swapped = true;
        } else {
            if (swapped) {
                batch1[i] = B[i];
                batch2[i] = A[i];
            } else {
                batch1[i] = A[i];
                batch2[i] = B[i];
            }
        }
    }

    return std::make_pair(batch1, batch2);
}