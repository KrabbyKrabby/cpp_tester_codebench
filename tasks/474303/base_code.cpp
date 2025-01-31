#include <functional>
#include <queue>
#include <stdexcept>
#include <vector>
#include <iostream>


// A simple Task struct. Right now, it only holds a function.
// In your enhancements, you may need to expand this (e.g., ID, status, schedule time, etc.).
struct Task {
    std::function<void()> func;
};

// A simple FIFO TaskScheduler that runs tasks one-by-one.
class TaskScheduler {
public:
    TaskScheduler() = default;
    ~TaskScheduler() = default;

    // Add a task to the queue (returns the index in the queue, but isn't used for anything yet).
    size_t addTask(const std::function<void()>& func) {
        Task task{func};
        tasks.push(task);
        return tasks.size() - 1; // simplistic; not truly an ID
    }

    // Runs the next task in the queue if one exists, then removes it from the queue.
    void runNext() {
        if (tasks.empty()) {
            return;
        }
        Task t = tasks.front();
        tasks.pop();
        // Execute
        t.func();
    }


private:
    std::queue<Task> tasks;
};