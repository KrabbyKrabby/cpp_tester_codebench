#include <functional>
#include <queue>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <map>
#include <atomic>
#include <memory>

enum class TaskStatus {
    PENDING,
    RUNNING,
    COMPLETED,
    CANCELED
};

struct Task {
    size_t id;
    std::function<void()> func;
    std::chrono::system_clock::time_point scheduledTime;
    std::atomic<TaskStatus> status;
    
    Task(size_t taskId, const std::function<void()>& taskFunc, 
         std::chrono::system_clock::time_point time = std::chrono::system_clock::now())
        : id(taskId), func(taskFunc), scheduledTime(time), status(TaskStatus::PENDING) {}
};

class TaskScheduler {
public:
    TaskScheduler(size_t threadCount = std::thread::hardware_concurrency())
        : nextTaskId(0), isRunning(false) {
        threadPool.reserve(threadCount);
    }

    ~TaskScheduler() {
        stopThreads();
    }

    size_t addTask(const std::function<void()>& func) {
        auto taskId = getNextTaskId();
        auto task = std::make_shared<Task>(taskId, func);
        
        std::lock_guard<std::mutex> lock(mutex);
        tasks.push(task);
        taskMap[taskId] = task;
        return taskId;
    }

    size_t scheduleTask(std::chrono::system_clock::time_point time, 
                       std::function<void()> task) {
        auto taskId = getNextTaskId();
        auto taskPtr = std::make_shared<Task>(taskId, task, time);
        
        std::lock_guard<std::mutex> lock(mutex);
        tasks.push(taskPtr);
        taskMap[taskId] = taskPtr;
        return taskId;
    }

    void runNext() {
        std::lock_guard<std::mutex> lock(mutex);
        if (tasks.empty()) {
            return;
        }

        auto now = std::chrono::system_clock::now();
        auto task = tasks.front();
        
        if (task->scheduledTime <= now && 
            task->status.load() == TaskStatus::PENDING) {
            tasks.pop();
            executeTask(task);
        }
    }

    void runAllConcurrently() {
        std::lock_guard<std::mutex> lock(mutex);
        if (!isRunning) {
            isRunning = true;
            for (size_t i = 0; i < threadPool.capacity(); ++i) {
                threadPool.emplace_back(&TaskScheduler::workerThread, this);
            }
        }
    }

    void waitAll() {
        std::unique_lock<std::mutex> lock(mutex);
        if (tasks.empty() && runningTasks.empty()) {
            return;
        }
        
        completionCV.wait(lock, [this]() {
            return tasks.empty() && runningTasks.empty();
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
        if (it != taskMap.end() && 
            it->second->status.load() == TaskStatus::PENDING) {
            it->second->status.store(TaskStatus::CANCELED);
            return true;
        }
        return false;
    }

private:
    std::queue<std::shared_ptr<Task>> tasks;
    std::map<size_t, std::shared_ptr<Task>> taskMap;
    std::vector<std::thread> threadPool;
    std::set<std::shared_ptr<Task>> runningTasks;
    mutable std::mutex mutex;
    std::condition_variable completionCV;
    std::atomic<size_t> nextTaskId;
    bool isRunning;

    size_t getNextTaskId() {
        return nextTaskId++;
    }

    void stopThreads() {
        {
            std::lock_guard<std::mutex> lock(mutex);
            isRunning = false;
        }
        completionCV.notify_all();
        
        for (auto& thread : threadPool) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

    void executeTask(std::shared_ptr<Task> task) {
        if (task->status.load() != TaskStatus::PENDING) {
            return;
        }
        
        task->status.store(TaskStatus::RUNNING);
        runningTasks.insert(task);
        
        try {
            task->func();
            task->status.store(TaskStatus::COMPLETED);
        } catch (...) {
            task->status.store(TaskStatus::COMPLETED);
        }
        
        runningTasks.erase(task);
        if (tasks.empty() && runningTasks.empty()) {
            completionCV.notify_all();
        }
    }

    void workerThread() {
        while (true) {
            std::unique_lock<std::mutex> lock(mutex);
            if (!isRunning && tasks.empty()) {
                break;
            }
            
            if (tasks.empty()) {
                completionCV.wait(lock);
                continue;
            }

            auto now = std::chrono::system_clock::now();
            auto task = tasks.front();
            
            if (task->scheduledTime > now) {
                auto waitTime = std::chrono::duration_cast<std::chrono::milliseconds>
                              (task->scheduledTime - now);
                completionCV.wait_for(lock, waitTime);
                continue;
            }

            tasks.pop();
            lock.unlock();
            
            if (task->status.load() == TaskStatus::PENDING) {
                executeTask(task);
            }
        }
    }
};
