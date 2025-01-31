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
#include <atomic>

// A simple Task struct. 
struct Task {
    size_t id;
    std::function<void()> func;
    std::chrono::system_clock::time_point scheduledTime;
    std::atomic<TaskStatus> status;
};

// Task status
enum class TaskStatus { PENDING, RUNNING, COMPLETED, CANCELED };

// A simple FIFO TaskScheduler that runs tasks one-by-one.
class TaskScheduler {
public:
    TaskScheduler() = default;
    ~TaskScheduler() {
        // Wait for all tasks to finish before destroying the scheduler
        waitAll();
    }

    // Add a task to the queue (returns the index in the queue, but isn't used for anything yet).
    size_t addTask(const std::function<void()>& func) {
        std::lock_guard<std::mutex> lock(mutex_);
        Task task{idCounter_++, func, std::chrono::system_clock::now(), TaskStatus::PENDING};
        tasks_.push(task);
        return task.id;
    }

    // Runs the next task in the queue if one exists, then removes it from the queue.
    void runNext() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (tasks_.empty()) {
            return;
        }
        Task t = tasks_.front();
        tasks_.pop();
        if (t.scheduledTime > std::chrono::system_clock::now()) {
            // If the task is scheduled for the future, put it back in the queue
            tasks_.push(t);
            return;
        }
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
            if (t.scheduledTime > std::chrono::system_clock::now()) {
                // If the task is scheduled for the future, put it back in the queue
                tasks_.push(t);
                continue;
            }
            // Execute
            t.status = TaskStatus::RUNNING;
            std::thread([this, t]() {
                t.func();
                std::lock_guard<std::mutex> lock(mutex_);
                taskStatus_[t.id] = TaskStatus::COMPLETED;
                cv_.notify_all();
            }).detach();
        }
    }

    // Waits for all tasks to finish
    void waitAll() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this]() {
            for (const auto& status : taskStatus_) {
                if (status.second != TaskStatus::COMPLETED && status.second != TaskStatus::CANCELED) {
                    return false;
                }
            }
            return true;
        });
    }

    // Schedules a task for a specific time
    size_t scheduleTask(std::chrono::system_clock::time_point time, std::function<void()> task) {
        std::lock_guard<std::mutex> lock(mutex_);
        Task t{idCounter_++, task, time, TaskStatus::PENDING};
        tasks_.push(t);
        taskStatus_[t.id] = TaskStatus::PENDING;
        return t.id;
    }

    // Gets the status of a task
    TaskStatus getTaskStatus(size_t taskId) const {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = taskStatus_.find(taskId);
        if (it != taskStatus_.end()) {
            return it->second;
        }
        throw std::invalid_argument("Task not found");
    }

    // Cancels a task
    bool cancelTask(size_t taskId) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = taskStatus_.find(taskId);
        if (it != taskStatus_.end() && it->second == TaskStatus::PENDING) {
            it->second = TaskStatus::CANCELED;
            return true;
        }
        return false;
    }

private:
    std::queue<Task> tasks_;
    std::unordered_map<size_t, TaskStatus> taskStatus_;
    std::mutex mutex_;
    std::condition_variable cv_;
    size_t idCounter_ = 0;
};
