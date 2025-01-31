#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int solution(int n, int k, std::vector<int> a) {
    // Sort the array in descending order based on absolute values
    sort(a.begin(), a.end(), [](int x, int y) {
        return abs(x) > abs(y);
    });

    // Negate the first k elements
    for (int i = 0; i < k; i++) {
        a[i] *= -1;
    }

    // Calculate the sum
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += a[i];
    }

    return sum;
}