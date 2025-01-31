#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int solution(int n, int k, vector<int> a) {
    // Convert negative numbers to positive and count them
    int negatives = 0;
    for (int i = 0; i < n; i++) {
        if (a[i] < 0) {
            a[i] = -a[i];
            negatives++;
        }
    }
    
    // If k is less than number of negatives,
    // flip k smallest numbers to negative
    if (k <= negatives) {
        sort(a.begin(), a.end());
        for (int i = 0; i < k; i++) {
            a[i] = -a[i];
        }
    }
    // If k > negatives and (k-negatives) is odd,
    // flip the smallest number to negative
    else if ((k - negatives) % 2 == 1) {
        sort(a.begin(), a.end());
        a[0] = -a[0];
    }
    
    // Calculate sum
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += a[i];
    }
    return sum;
}
