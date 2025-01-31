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
    std::atomic<TaskStatus> status;
    
    Task(size_t taskId, const std::function<void()>& taskFunc, 
         std::chrono::system_clock::time_point time = std::chrono::system_clock::now())
        : id(taskId), func(taskFunc), scheduleTime(time), status(TaskStatus::PENDING) {}
};

class TaskScheduler {
public:
    TaskScheduler(size_t threadCount = std::thread::hardware_concurrency())
        : nextTaskId(0), isRunning(false) {
        if (threadCount == 0) threadCount = 1;
        this->threadCount = threadCount;
    }

    ~TaskScheduler() {
        stopThreads();
    }

    size_t addTask(const std::function<void()>& func) {
        return scheduleTask(std::chrono::system_clock::now(), func);
    }

    size_t scheduleTask(std::chrono::system_clock::time_point time, std::function<void()> task) {
        std::lock_guard<std::mutex> lock(mutex);
        size_t taskId = nextTaskId++;
        auto newTask = std::make_shared<Task>(taskId, task, time);
        tasks.push(newTask);
        taskMap[taskId] = newTask;
        return taskId;
    }

    void runNext() {
        std::lock_guard<std::mutex> lock(mutex);
        if (tasks.empty()) return;

        auto now = std::chrono::system_clock::now();
        auto task = tasks.front();
        
        if (task->scheduleTime <= now && task->status == TaskStatus::PENDING) {
            tasks.pop();
            executeTask(task);
        }
    }

    void runAllConcurrently() {
        std::lock_guard<std::mutex> lock(mutex);
        if (!isRunning) {
            isRunning = true;
            for (size_t i = 0; i < threadCount; ++i) {
                workers.emplace_back([this] { workerThread(); });
            }
        }
    }

    void waitAll() {
        std::unique_lock<std::mutex> lock(mutex);
        completionCV.wait(lock, [this] {
            return tasks.empty() && activeTaskCount == 0;
        });
    }

    TaskStatus getTaskStatus(size_t taskId) const {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = taskMap.find(taskId);
        if (it != taskMap.end()) {
            return it->second->status;
        }
        throw std::runtime_error("Task not found");
    }

    bool cancelTask(size_t taskId) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = taskMap.find(taskId);
        if (it != taskMap.end() && it->second->status == TaskStatus::PENDING) {
            it->second->status = TaskStatus::CANCELED;
            return true;
        }
        return false;
    }

private:
    std::queue<std::shared_ptr<Task>> tasks;
    std::map<size_t, std::shared_ptr<Task>> taskMap;
    std::vector<std::thread> workers;
    mutable std::mutex mutex;
    std::condition_variable completionCV;
    std::atomic<size_t> nextTaskId;
    std::atomic<bool> isRunning;
    std::atomic<size_t> activeTaskCount{0};
    size_t threadCount;

    void executeTask(std::shared_ptr<Task> task) {
        if (task->status != TaskStatus::PENDING) return;
        
        task->status = TaskStatus::RUNNING;
        activeTaskCount++;
        
        try {
            task->func();
            task->status = TaskStatus::COMPLETED;
        } catch (...) {
            task->status = TaskStatus::COMPLETED;  // or handle error differently
        }
        
        activeTaskCount--;
        completionCV.notify_all();
    }

    void workerThread() {
        while (isRunning) {
            std::shared_ptr<Task> task = nullptr;
            {
                std::unique_lock<std::mutex> lock(mutex);
                auto now = std::chrono::system_clock::now();
                if (!tasks.empty() && tasks.front()->scheduleTime <= now &&
                    tasks.front()->status == TaskStatus::PENDING) {
                    task = tasks.front();
                    tasks.pop();
                }
            }
            
            if (task) {
                executeTask(task);
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }

    void stopThreads() {
        {
            std::lock_guard<std::mutex> lock(mutex);
            isRunning = false;
        }
        
        for (auto& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
        workers.clear();
    }
};
