#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int countDays(int days, vector<vector<int>>& tasks) {
    sort(tasks.begin(), tasks.end());
    int ans = 0;
    for (int i = 1; i < tasks.size(); i++) {
        if (tasks[i][0] > tasks[i - 1][1]) {
            ans += (tasks[i][0] - tasks[i - 1][1] - 1);
        }
    }
    ans += (days - tasks.back()[1]);
    return ans;
}

