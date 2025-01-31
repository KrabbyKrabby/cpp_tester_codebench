#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int countDays(int days, vector<vector<int>>& tasks) {
    // If no tasks, all days are free
    if (tasks.empty()) {
        return days;
    }

    // Sort tasks by start day
    sort(tasks.begin(), tasks.end());

    int freeDays = 0;

    // Count days before first task
    freeDays += tasks[0][0] - 1;

    // Count days between tasks
    for (int i = 1; i < tasks.size(); i++) {
        int prevEnd = tasks[i-1][1];
        int currStart = tasks[i][0];
        
        if (currStart > prevEnd) {
            freeDays += currStart - prevEnd - 1;
        }
    }

    // Count days after last task
    freeDays += days - tasks.back()[1];

    return freeDays;
}

