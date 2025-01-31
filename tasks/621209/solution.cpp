#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int countDays(int days, vector<vector<int>>& tasks) {
    // Sort the tasks by their start days
    sort(tasks.begin(), tasks.end());
    
    int ans = 0;
    int lastEndDay = 0;
    
    for (auto& task : tasks) {
        // If the task starts after the last end day, add the gap to the answer
        if (task[0] > lastEndDay) {
            ans += (task[0] - lastEndDay - 1);
        }
        // Update the last end day
        lastEndDay = max(lastEndDay, task[1]);
    }
    
    // Add the remaining days after the last task
    ans += (days - lastEndDay);
    
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