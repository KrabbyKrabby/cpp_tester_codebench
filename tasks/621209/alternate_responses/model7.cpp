#include <iostream>
#include <vector>
#include <algorithm>

int countDays(int days, std::vector<std::vector<int>>& tasks) {
    int ans = 0;
    std::sort(tasks.begin(), tasks.end());
    for (int i = 1; i < tasks.size(); i++) {
        if (tasks[i][0] > tasks[i - 1][1]) {
            ans += (tasks[i][0] - tasks[i - 1][1] - 1);
        }
    }
    if (tasks.size() > 0) {
        ans += (days - tasks.back()[1]);
    } else {
        ans = days;
    }
    return ans;
}

