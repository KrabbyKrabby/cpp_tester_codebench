#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int countDays(int days, vector<vector<int>>& tasks) {
    int ans = 0;
    // Sort the tasks by start day
    sort(tasks.begin(), tasks.end());

    // Count days before the first task
    if (tasks[0][0] > 1) {
        ans += tasks[0][0] - 1;
    }

    // Count days between tasks
    for (int i = 1; i < tasks.size(); i++) {
        if (tasks[i][0] > tasks[i - 1][1]) {
            ans += tasks[i][0] - tasks[i - 1][1] - 1;
        }
    }

    // Count days after the last task
    if (tasks.back()[1] < days) {
        ans += days - tasks.back()[1];
    }

    return ans;
}

