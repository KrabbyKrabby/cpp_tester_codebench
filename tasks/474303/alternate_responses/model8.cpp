#include <functional>
#include <queue>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <atomic>

// A simple Task struct. Now, it holds a function, task ID, status, schedule time, and a flag for cancellation.
struct Task {
    std::function<void()> func;
    size_t taskId;
    std::atomic<TaskStatus> status;
    std::chrono::system_clock::time_point scheduleTime;
    std::atomic<bool> canceled;

    Task(std::function<void()> func, size_t taskId, std::chrono::system_clock::time_point scheduleTime)
        : func(func), taskId(taskId), status(TaskStatus::PENDING), scheduleTime(scheduleTime), canceled(false) {}
};

// A simple FIFO TaskScheduler that runs tasks one-by-one.
class TaskScheduler {
public:
    TaskScheduler() = default;
    ~TaskScheduler() = default;

    // Add a task to the queue (returns the index in the queue, but isn't used for anything yet).
    size_t addTask(const std::function<void()>& func) {
        std::lock_guard<std::mutex> lock(mutex_);
        Task task{func, tasks_.size(), std::chrono::system_clock::now()};
        tasks_.push(task);
        return tasks_.size() - 1; // simplistic; not truly an ID
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
        t.func();
    }

    // Runs all tasks concurrently using multiple threads.
    void runAllConcurrently() {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<std::thread> threads;
        while (!tasks_.empty()) {
            Task t = tasks_.front();
            tasks_.pop();
            threads.emplace_back([this, t]() {
                std::lock_guard<std::mutex> lock(mutex_);
                t.func();
                t.status = TaskStatus::COMPLETED;
            });
        }
        for (auto& thread : threads) {
            thread.detach();
        }
    }

    // Waits for all tasks to complete.
    void waitAll() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this]() {
            return tasks_.empty();
        });
    }

    // Schedules a task to be executed not before the specified time.
    size_t scheduleTask(std::chrono::system_clock::time_point time, std::function<void()> task) {
        std::lock_guard<std::mutex> lock(mutex_);
        Task t{task, tasks_.size(), time};
        tasks_.push(t);
        return t.taskId;
    }

    // Returns the status of a task.
    TaskStatus getTaskStatus(size_t taskId) const {
        std::lock_guard<std::mutex> lock(mutex_);
        for (const auto& task : tasks_) {
            if (task.taskId == taskId) {
                return task.status;
            }
        }
        return TaskStatus::CANCELED; // Task not found
    }

    // Cancels a task.
    bool cancelTask(size_t taskId) {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto& task : tasks_) {
            if (task.taskId == taskId && task.status == TaskStatus::PENDING) {
                task.canceled = true;
                return true;
            }
        }
        return false; // Task not found or not pending
    }

private:
    std::queue<Task> tasks_;
    mutable std::mutex mutex_;
    std::condition_variable cv_;
};

enum class TaskStatus { PENDING, RUNNING, COMPLETED, CANCELED };
