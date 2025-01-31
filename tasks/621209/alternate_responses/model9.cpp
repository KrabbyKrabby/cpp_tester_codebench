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

int main() {
    int days = 10;
    vector<vector<int>> tasks = {
        {1, 2},
        {4, 5},
        {7, 8}
    };

    int result = countDays(days, tasks);
    cout << "Number of days without tasks: " << result << endl;

    return 0;
}