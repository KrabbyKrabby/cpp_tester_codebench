#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int countDays(int days, vector<vector<int>>& tasks) {
    // Handle edge cases
    if (days <= 0) return 0;
    if (tasks.empty()) return days;
    
    // Sort tasks based on start day
    sort(tasks.begin(), tasks.end());
    
    int freeDays = 0;
    
    // Count days before first task
    freeDays += tasks[0][0] - 1;
    
    // Count days between tasks
    for (int i = 1; i < tasks.size(); i++) {
        int gap = tasks[i][0] - tasks[i-1][1] - 1;
        if (gap > 0) {
            freeDays += gap;
        }
    }
    
    // Count days after last task
    if (tasks.back()[1] < days) {
        freeDays += days - tasks.back()[1];
    }
    
    return freeDays;
}

