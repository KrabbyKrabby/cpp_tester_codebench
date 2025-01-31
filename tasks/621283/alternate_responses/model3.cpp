#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int solution(int n, int k, std::vector<int> a) {
    sort(a.begin(), a.end());
    int i = 0;
    
    // Change signs of negative numbers
    while (i < n && i < k && a[i] < 0) {
        a[i] = -a[i];
        i++;
    }
    
    // If we have remaining changes (k - i) and it's odd
    // Apply it to the smallest remaining number
    if ((k - i) % 2 == 1) {
        int minIndex = min_element(a.begin(), a.end()) - a.begin();
        a[minIndex] = -a[minIndex];
    }
    
    // Calculate sum
    int sum = 0;
    for (int x : a) {
        sum += x;
    }
    return sum;
}