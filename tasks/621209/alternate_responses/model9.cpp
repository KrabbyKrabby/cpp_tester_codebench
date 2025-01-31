#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int countDays(int days, vector<vector<int>>& tasks) {
    // Sort the tasks based on the start day
    sort(tasks.begin(), tasks.end());

    int ans = 0;
    int lastEndDay = 0; // Keep track of the last end day

    for (const auto& task : tasks) {
        // Count the days without tasks between the last end day and the current start day
        ans += max(0, task[0] - lastEndDay - 1);
        // Update the last end day
        lastEndDay = max(lastEndDay, task[1]);
    }

    // Count the remaining days without tasks after the last end day
    ans += max(0, days - lastEndDay);

    return ans;
}

