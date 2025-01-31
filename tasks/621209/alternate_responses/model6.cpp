#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

int countDays(int days, std::vector<std::vector<int>>& tasks) {
    // Check if tasks is empty
    if (tasks.empty()) {
        return days; // If no tasks, all days are without tasks
    }

    // Sort tasks based on start day
    std::sort(tasks.begin(), tasks.end(), [](const std::vector<int>& a, const std::vector<int>& b) {
        return a[0] < b[0];
    });

    int ans = 0;

    // Check if there are days without tasks before the first task
    if (tasks[0][0] > 1) {
        ans += (tasks[0][0] - 1);
    }

    for (int i = 1; i < tasks.size(); i++) {
        if (tasks[i][0] > tasks[i - 1][1]) {
            ans += (tasks[i][0] - tasks[i - 1][1] - 1);
        }
    }

    // Check if there are days without tasks after the last task
    if (days > tasks.back()[1]) {
        ans += (days - tasks.back()[1]);
    }

    return ans;
}

int main() {
    int days = 10;
    std::vector<std::vector<int>> tasks = {
        {1, 2},
        {4, 5},
        {7, 8}
    };

    int result = countDays(days, tasks);
    std::cout << "Number of days without tasks: " << result << std::endl;

    return 0;
}