#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int solution(int n, int k, vector<int> a) {
    sort(a.begin(), a.end());
    int i = 0;
    
    // Convert most negative numbers to positive
    while (i < n && i < k && a[i] < 0) {
        a[i] = -a[i];
        i++;
    }
    
    // If k is not fully used and k-i is odd
    // change the smallest remaining number to negative
    if (i < k && (k-i) % 2 == 1) {
        int minIndex = min_element(a.begin(), a.end()) - a.begin();
        a[minIndex] = -a[minIndex];
    }
    
    // Calculate sum
    int sum = 0;
    for (int num : a) {
        sum += num;
    }
    return sum;
}
