#include <functional>
#include <queue>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <map>
#include <atomic>
#include <chrono>

enum class TaskStatus {
    PENDING,
    RUNNING,
    COMPLETED,
    CANCELED
};

struct Task {
    size_t id;
    std::function<void()> func;
    std::chrono::system_clock::time_point scheduleTime;
    TaskStatus status;
    
    Task(size_t id, const std::function<void()>& f, 
         std::chrono::system_clock::time_point time = std::chrono::system_clock::now())
        : id(id), func(f), scheduleTime(time), status(TaskStatus::PENDING) {}
};

class TaskScheduler {
public:
    TaskScheduler(size_t threadCount = std::thread::hardware_concurrency())
        : nextTaskId(0), isRunning(false) {
        threadPoolSize = threadCount > 0 ? threadCount : 1;
    }

    ~TaskScheduler() {
        stopThreadPool();
    }

    size_t addTask(const std::function<void()>& func) {
        std::lock_guard<std::mutex> lock(mutex);
        size_t taskId = nextTaskId++;
        tasks.push(Task(taskId, func));
        updateTaskStatus(taskId, TaskStatus::PENDING);
        return taskId;
    }

    size_t scheduleTask(std::chrono::system_clock::time_point time, 
                       std::function<void()> task) {
        std::lock_guard<std::mutex> lock(mutex);
        size_t taskId = nextTaskId++;
        tasks.push(Task(taskId, task, time));
        updateTaskStatus(taskId, TaskStatus::PENDING);
        return taskId;
    }

    void runNext() {
        std::lock_guard<std::mutex> lock(mutex);
        if (tasks.empty()) return;

        auto now = std::chrono::system_clock::now();
        Task task = tasks.front();
        
        if (task.scheduleTime <= now && 
            taskStatuses[task.id] == TaskStatus::PENDING) {
            tasks.pop();
            executeTask(task);
        }
    }

    void runAllConcurrently() {
        std::lock_guard<std::mutex> lock(mutex);
        if (!isRunning) {
            isRunning = true;
            startThreadPool();
        }
        condition.notify_all();
    }

    void waitAll() {
        std::unique_lock<std::mutex> lock(mutex);
        waitCondition.wait(lock, [this]() {
            return tasks.empty() && activeTaskCount == 0;
        });
    }

    TaskStatus getTaskStatus(size_t taskId) const {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = taskStatuses.find(taskId);
        if (it == taskStatuses.end()) {
            throw std::runtime_error("Task ID not found");
        }
        return it->second;
    }

    bool cancelTask(size_t taskId) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = taskStatuses.find(taskId);
        if (it != taskStatuses.end() && it->second == TaskStatus::PENDING) {
            it->second = TaskStatus::CANCELED;
            return true;
        }
        return false;
    }

private:
    std::queue<Task> tasks;
    mutable std::mutex mutex;
    std::condition_variable condition;
    std::condition_variable waitCondition;
    std::atomic<size_t> nextTaskId;
    std::map<size_t, TaskStatus> taskStatuses;
    std::vector<std::thread> threadPool;
    size_t threadPoolSize;
    std::atomic<bool> isRunning;
    std::atomic<size_t> activeTaskCount{0};

    void updateTaskStatus(size_t taskId, TaskStatus status) {
        taskStatuses[taskId] = status;
    }

    void executeTask(Task& task) {
        updateTaskStatus(task.id, TaskStatus::RUNNING);
        activeTaskCount++;
        
        try {
            task.func();
            updateTaskStatus(task.id, TaskStatus::COMPLETED);
        } catch (...) {
            updateTaskStatus(task.id, TaskStatus::COMPLETED);
        }
        
        activeTaskCount--;
        waitCondition.notify_all();
    }

    void startThreadPool() {
        for (size_t i = 0; i < threadPoolSize; ++i) {
            threadPool.emplace_back([this]() {
                while (isRunning) {
                    std::unique_lock<std::mutex> lock(mutex);
                    condition.wait(lock, [this]() {
                        return !tasks.empty() || !isRunning;
                    });

                    if (!isRunning) break;

                    auto now = std::chrono::system_clock::now();
                    if (!tasks.empty() && 
                        tasks.front().scheduleTime <= now && 
                        taskStatuses[tasks.front().id] == TaskStatus::PENDING) {
                        Task task = tasks.front();
                        tasks.pop();
                        lock.unlock();
                        executeTask(task);
                    } else {
                        lock.unlock();
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    }
                }
            });
        }
    }

    void stopThreadPool() {
        {
            std::lock_guard<std::mutex> lock(mutex);
            isRunning = false;
        }
        condition.notify_all();
        for (auto& thread : threadPool) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        threadPool.clear();
    }
};