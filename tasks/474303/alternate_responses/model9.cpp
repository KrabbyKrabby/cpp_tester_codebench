#include <functional>
#include <queue>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <future>
#include <unordered_map>

// A simple Task struct. Right now, it only holds a function.
// In your enhancements, you may need to expand this (e.g., ID, status, schedule time, etc.).
struct Task {
    size_t id;
    std::function<void()> func;
    std::chrono::system_clock::time_point scheduleTime;
    std::atomic<TaskStatus> status;
};

enum class TaskStatus { PENDING, RUNNING, COMPLETED, CANCELED };

// A simple FIFO TaskScheduler that runs tasks one-by-one.
class TaskScheduler {
public:
    TaskScheduler() = default;
    ~TaskScheduler() = default;

    // Add a task to the queue (returns the index in the queue, but isn't used for anything yet).
    size_t addTask(const std::function<void()>& func) {
        std::lock_guard<std::mutex> lock(mutex_);
        size_t id = nextTaskId_++;
        Task task{id, func, std::chrono::system_clock::now(), TaskStatus::PENDING};
        tasks_.push(task);
        taskMap_[id] = task;
        return id;
    }

    // Runs the next task in the queue if one exists, then removes it from the queue.
    void runNext() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (tasks_.empty()) {
            return;
        }
        Task t = tasks_.front();
        tasks_.pop();
        // Execute
        t.status = TaskStatus::RUNNING;
        t.func();
        t.status = TaskStatus::COMPLETED;
    }

    // Runs all tasks concurrently
    void runAllConcurrently() {
        std::lock_guard<std::mutex> lock(mutex_);
        while (!tasks_.empty()) {
            Task t = tasks_.front();
            tasks_.pop();
            // Check if task is scheduled for the future
            if (t.scheduleTime > std::chrono::system_clock::now()) {
                tasks_.push(t); // Put it back in the queue
                break;
            }
            // Execute
            t.status = TaskStatus::RUNNING;
            std::thread([this, t]() {
                t.func();
                {
                    std::lock_guard<std::mutex> lock(mutex_);
                    taskMap_[t.id].status = TaskStatus::COMPLETED;
                }
            }).detach();
        }
    }

    // Wait for all tasks to complete
    void waitAll() {
        std::unique_lock<std::mutex> lock(mutex_);
        while (true) {
            bool allCompleted = true;
            for (const auto& task : taskMap_) {
                if (task.second.status != TaskStatus::COMPLETED) {
                    allCompleted = false;
                    break;
                }
            }
            if (allCompleted) {
                break;
            }
            condVar_.wait(lock);
        }
    }

    // Schedule a task for a specific time
    size_t scheduleTask(std::chrono::system_clock::time_point time, std::function<void()> task) {
        std::lock_guard<std::mutex> lock(mutex_);
        size_t id = nextTaskId_++;
        Task t{id, task, time, TaskStatus::PENDING};
        tasks_.push(t);
        taskMap_[id] = t;
        return id;
    }

    // Get the status of a task
    TaskStatus getTaskStatus(size_t taskId) const {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = taskMap_.find(taskId);
        if (it == taskMap_.end()) {
            throw std::invalid_argument("Task ID not found");
        }
        return it->second.status;
    }

    // Cancel a task
    bool cancelTask(size_t taskId) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = taskMap_.find(taskId);
        if (it == taskMap_.end()) {
            throw std::invalid_argument("Task ID not found");
        }
        if (it->second.status == TaskStatus::PENDING) {
            it->second.status = TaskStatus::CANCELED;
            return true;
        }
        return false;
    }

private:
    std::queue<Task> tasks_;
    std::unordered_map<size_t, Task> taskMap_;
    std::mutex mutex_;
    std::condition_variable condVar_;
    size_t nextTaskId_ = 0;
};