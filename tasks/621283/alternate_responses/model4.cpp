#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int solution(int n, int k, vector<int> a) {
    sort(a.begin(), a.end());  // Sort in ascending order
    int i = 0;
    
    // Change signs of negative numbers as long as we have changes left
    while (i < n && i < k && a[i] < 0) {
        a[i] = -a[i];
        i++;
    }
    
    // If we have remaining changes
    if (i < k) {
        // If remaining changes are odd
        if ((k - i) % 2 == 1) {
            // Find the smallest number and change its sign
            int min_idx = min_element(a.begin(), a.end()) - a.begin();
            a[min_idx] = -a[min_idx];
        }
    }
    
    // Calculate sum
    int sum = 0;
    for (int x : a) {
        sum += x;
    }
    return sum;
}