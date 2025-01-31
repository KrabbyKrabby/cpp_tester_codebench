#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int solution(int n, int k, std::vector<int> a) {
    sort(a.begin(), a.end());
    for (int i = 0; i < k; i++) {
        if (a[i] < 0) {
            a[i] *= -1;
        }
    }
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += a[i];
    }
    return sum;
}
