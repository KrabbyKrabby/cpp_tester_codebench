#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int countDays(int days, vector<vector<int>>& tasks) {
    // Handle empty tasks case
    if (tasks.empty()) {
        return days;
    }

    // Validate days
    if (days <= 0) {
        return 0;
    }

    // Sort tasks based on start day
    sort(tasks.begin(), tasks.end());

    // Validate tasks
    for (const auto& task : tasks) {
        if (task[0] <= 0 || task[1] <= 0 || task[0] > task[1] || task[1] > days) {
            return 0;  // Invalid input
        }
    }

    int freeDays = 0;

    // Count days before first task
    freeDays += tasks[0][0] - 1;

    // Count days between tasks
    for (int i = 1; i < tasks.size(); i++) {
        if (tasks[i][0] > tasks[i-1][1]) {
            freeDays += tasks[i][0] - tasks[i-1][1] - 1;
        }
    }

    // Count days after last task
    freeDays += days - tasks.back()[1];

    return freeDays;
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