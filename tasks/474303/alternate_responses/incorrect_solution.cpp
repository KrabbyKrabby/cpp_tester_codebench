#include <functional>
#include <queue>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <unordered_map>

// A simple Task struct. Right now, it only holds a function.
// In your enhancements, you may need to expand this (e.g., ID, status, schedule time, etc.).
struct Task {
    size_t id;
    std::function<void()> func;
    std::chrono::system_clock::time_point scheduledTime;
    std::atomic<TaskStatus> status;
};

// A simple FIFO TaskScheduler that runs tasks one-by-one.
class TaskScheduler {
public:
    TaskScheduler() = default;
    ~TaskScheduler() {
        stop();
    }

    // Add a task to the queue (returns the index in the queue, but isn't used for anything yet).
    size_t addTask(const std::function<void()>& func) {
        Task task{getNextId(), func, std::chrono::system_clock::now(), TaskStatus::PENDING};
        {
            std::lock_guard<std::mutex> lock(tasksMutex);
            tasks.push(task);
        }
        return task.id;
    }

    // Runs the next task in the queue if one exists, then removes it from the queue.
    void runNext() {
        Task task;
        {
            std::lock_guard<std::mutex> lock(tasksMutex);
            if (tasks.empty()) {
                return;
            }
            task = tasks.front();
            tasks.pop();
        }
        runTask(task);
    }

    // Runs all tasks concurrently
    void runAllConcurrently() {
        std::vector<Task> tasksToRun;
        {
            std::lock_guard<std::mutex> lock(tasksMutex);
            while (!tasks.empty()) {
                tasksToRun.push_back(tasks.front());
                tasks.pop();
            }
        }
        for (auto& task : tasksToRun) {
            std::thread([this, task]() { runTask(task); }).detach();
        }
    }

    // Waits for all tasks to complete
    void waitAll() {
        std::unique_lock<std::mutex> lock(waitMutex);
        waitCondition.wait(lock, [this] { return tasks.empty() && runningTasks == 0; });
    }

    // Schedules a task to run at a specific time
    size_t scheduleTask(std::chrono::system_clock::time_point time, std::function<void()> task) {
        Task newTask{getNextId(), task, time, TaskStatus::PENDING};
        {
            std::lock_guard<std::mutex> lock(tasksMutex);
            tasks.push(newTask);
        }
        return newTask.id;
    }

    // Gets the status of a task
    TaskStatus getTaskStatus(size_t taskId) const {
        std::lock_guard<std::mutex> lock(tasksMutex);
        for (const auto& task : tasks) {
            if (task.id == taskId) {
                return task.status;
            }
        }
        return TaskStatus::COMPLETED; // Assume completed if not found
    }

    // Cancels a task
    bool cancelTask(size_t taskId) {
        std::lock_guard<std::mutex> lock(tasksMutex);
        for (auto& task : tasks) {
            if (task.id == taskId && task.status == TaskStatus::PENDING) {
                task.status = TaskStatus::CANCELED;
                return true;
            }
        }
        return false;
    }

    // Stops the scheduler
    void stop() {
        {
            std::lock_guard<std::mutex> lock(tasksMutex);
            while (!tasks.empty()) {
                tasks.pop();
            }
        }
        waitCondition.notify_all();
    }

private:
    std::queue<Task> tasks;
    mutable std::mutex tasksMutex;
    std::condition_variable waitCondition;
    std::mutex waitMutex;
    std::atomic<size_t> nextId{0};
    std::atomic<size_t> runningTasks{0};

    size_t getNextId() {
        return nextId++;
    }

    void runTask(Task& task) {
        if (task.scheduledTime > std::chrono::system_clock::now()) {
            std::this_thread::sleep_until(task.scheduledTime);
        }
        if (task.status == TaskStatus::CANCELED) {
            return;
        }
        task.status = TaskStatus::RUNNING;
        runningTasks++;
        try {
            task.func();
        } catch (...) {
            // Handle exception
        }
        task.status = TaskStatus::COMPLETED;
        runningTasks--;
        waitCondition.notify_one();
    }
};

enum class TaskStatus { PENDING, RUNNING, COMPLETED, CANCELED };