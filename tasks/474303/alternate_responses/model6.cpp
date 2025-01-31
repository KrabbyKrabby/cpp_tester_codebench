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
#include <unordered_map>

// A simple Task struct. Now it holds a function, a unique ID, status and scheduled time.
struct Task {
    size_t id;
    std::function<void()> func;
    std::chrono::system_clock::time_point scheduleTime;
    std::atomic<TaskStatus> status;

    Task(size_t id, std::function<void()> func, std::chrono::system_clock::time_point scheduleTime)
        : id(id), func(func), scheduleTime(scheduleTime), status(TaskStatus::PENDING) {}
};

// A simple FIFO TaskScheduler that runs tasks one-by-one.
class TaskScheduler {
public:
    TaskScheduler() = default;
    ~TaskScheduler() {
        // Wait for all tasks to complete before destroying the scheduler
        waitAll();
    }

    // Add a task to the queue (returns the index in the queue, but isn't used for anything yet).
    size_t addTask(const std::function<void()>& func) {
        std::lock_guard<std::mutex> lock(mutex_);
        size_t id = nextId_++;
        Task task(id, func, std::chrono::system_clock::now());
        tasks_.push_back(task);
        return id;
    }

    // Runs the next task in the queue if one exists, then removes it from the queue.
    void runNext() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (tasks_.empty()) {
            return;
        }
        Task& t = tasks_.front();
        if (t.scheduleTime > std::chrono::system_clock::now()) {
            return;
        }
        t.status = TaskStatus::RUNNING;
        lock.unlock();
        t.func();
        lock.lock();
        t.status = TaskStatus::COMPLETED;
        tasks_.pop_front();
    }

    // Execute all tasks concurrently
    void runAllConcurrently() {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto& task : tasks_) {
            if (task.scheduleTime > std::chrono::system_clock::now()) {
                continue;
            }
            task.status = TaskStatus::RUNNING;
            std::thread([this, &task]() {
                task.func();
                std::lock_guard<std::mutex> lock(mutex_);
                task.status = TaskStatus::COMPLETED;
            }).detach();
        }
        tasks_.erase(std::remove_if(tasks_.begin(), tasks_.end(),
            [](const Task& task) { return task.status == TaskStatus::COMPLETED; }), tasks_.end());
    }

    // Wait for all tasks to complete
    void waitAll() {
        std::unique_lock<std::mutex> lock(mutex_);
        while (!tasks_.empty()) {
            condVar_.wait(lock, [this] { return tasks_.empty(); });
        }
    }

    // Schedule a task to be executed not before the specified time
    size_t scheduleTask(std::chrono::system_clock::time_point time, std::function<void()> task) {
        std::lock_guard<std::mutex> lock(mutex_);
        size_t id = nextId_++;
        tasks_.emplace_back(id, task, time);
        return id;
    }

    // Get the status of a task
    TaskStatus getTaskStatus(size_t taskId) const {
        std::lock_guard<std::mutex> lock(mutex_);
        for (const auto& task : tasks_) {
            if (task.id == taskId) {
                return task.status;
            }
        }
        throw std::invalid_argument("Task ID not found");
    }

    // Cancel a task
    bool cancelTask(size_t taskId) {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto& task : tasks_) {
            if (task.id == taskId && task.status == TaskStatus::PENDING) {
                task.status = TaskStatus::CANCELED;
                return true;
            }
        }
        return false;
    }

private:
    std::mutex mutex_;
    std::condition_variable condVar_;
    std::deque<Task> tasks_;
    size_t nextId_ = 0;
};

enum class TaskStatus { PENDING, RUNNING, COMPLETED, CANCELED };
