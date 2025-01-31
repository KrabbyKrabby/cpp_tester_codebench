#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int solution(int n, int k, std::vector<int> a) {
    sort(a.begin(), a.end());
    int i = 0;
    while (k > 0 && i < n) {
        if (a[i] < 0) {
            a[i] *= -1;
            k--;
        }
        i++;
    }
    i = 0;
    while (k > 0 && i < n) {
        if (a[i] > 0) {
            a[i] *= -1;
            k--;
        }
        i++;
    }
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += a[i];
    }
    return sum;
}
