#include <iostream>
#include <vector>
#include <algorithm>
#define loop(n) for(int i=0;i<n;i++)


using namespace std;

int main() {
    int n, k;
    cin >> n >> k;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    sort(a.begin(), a.end());
    for (int i = 0; i < k; i++) {
        a[i] *= -1;
    }
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += a[i];
    }
    cout << sum << endl;
}