#include <functional>
#include <queue>
#include <stdexcept>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <map>
#include <atomic>
#include <chrono>
#include <memory>

enum class TaskStatus {
    PENDING,
    RUNNING,
    COMPLETED,
    CANCELED
};

struct Task {
    std::function<void()> func;
    size_t id;
    std::chrono::system_clock::time_point scheduleTime;
    std::atomic<TaskStatus> status;
    
    Task(std::function<void()> f, size_t taskId, 
         std::chrono::system_clock::time_point time = std::chrono::system_clock::now())
        : func(f), id(taskId), scheduleTime(time), status(TaskStatus::PENDING) {}
};

class TaskScheduler {
public:
    TaskScheduler(size_t threadCount = 4) : nextTaskId(0), isRunning(true) {
        for (size_t i = 0; i < threadCount; ++i) {
            workers.emplace_back([this] { workerFunction(); });
        }
    }

    ~TaskScheduler() {
        {
            std::unique_lock<std::mutex> lock(mutex);
            isRunning = false;
            condition.notify_all();
        }
        for (auto& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    size_t addTask(const std::function<void()>& func) {
        return scheduleTask(std::chrono::system_clock::now(), func);
    }

    size_t scheduleTask(std::chrono::system_clock::time_point time, 
                       std::function<void()> task) {
        std::unique_lock<std::mutex> lock(mutex);
        size_t taskId = nextTaskId++;
        auto taskPtr = std::make_shared<Task>(task, taskId, time);
        tasks.push(taskPtr);
        taskMap[taskId] = taskPtr;
        return taskId;
    }

    void runNext() {
        std::unique_lock<std::mutex> lock(mutex);
        if (tasks.empty()) {
            return;
        }

        auto now = std::chrono::system_clock::now();
        auto taskPtr = tasks.front();
        
        if (taskPtr->scheduleTime <= now && 
            taskPtr->status.load() == TaskStatus::PENDING) {
            tasks.pop();
            lock.unlock();
            
            taskPtr->status.store(TaskStatus::RUNNING);
            taskPtr->func();
            taskPtr->status.store(TaskStatus::COMPLETED);
        }
    }

    void runAllConcurrently() {
        std::unique_lock<std::mutex> lock(mutex);
        condition.notify_all();
    }

    void waitAll() {
        std::unique_lock<std::mutex> lock(mutex);
        while (!tasks.empty() || activeTaskCount > 0) {
            completionCondition.wait(lock);
        }
    }

    TaskStatus getTaskStatus(size_t taskId) const {
        std::unique_lock<std::mutex> lock(mutex);
        auto it = taskMap.find(taskId);
        if (it == taskMap.end()) {
            throw std::runtime_error("Task not found");
        }
        return it->second->status.load();
    }

    bool cancelTask(size_t taskId) {
        std::unique_lock<std::mutex> lock(mutex);
        auto it = taskMap.find(taskId);
        if (it == taskMap.end()) {
            return false;
        }

        auto currentStatus = it->second->status.load();
        if (currentStatus != TaskStatus::PENDING) {
            return false;
        }

        it->second->status.store(TaskStatus::CANCELED);
        return true;
    }

private:
    void workerFunction() {
        while (true) {
            std::shared_ptr<Task> taskPtr;
            {
                std::unique_lock<std::mutex> lock(mutex);
                condition.wait(lock, [this] {
                    return !isRunning || (!tasks.empty() && 
                            tasks.front()->scheduleTime <= std::chrono::system_clock::now() &&
                            tasks.front()->status.load() == TaskStatus::PENDING);
                });

                if (!isRunning && tasks.empty()) {
                    return;
                }

                if (!tasks.empty()) {
                    taskPtr = tasks.front();
                    tasks.pop();
                    ++activeTaskCount;
                }
            }

            if (taskPtr) {
                taskPtr->status.store(TaskStatus::RUNNING);
                taskPtr->func();
                taskPtr->status.store(TaskStatus::COMPLETED);

                {
                    std::unique_lock<std::mutex> lock(mutex);
                    --activeTaskCount;
                    completionCondition.notify_all();
                }
            }
        }
    }

    std::queue<std::shared_ptr<Task>> tasks;
    std::map<size_t, std::shared_ptr<Task>> taskMap;
    std::vector<std::thread> workers;
    mutable std::mutex mutex;
    std::condition_variable condition;
    std::condition_variable completionCondition;
    std::atomic<size_t> nextTaskId;
    std::atomic<bool> isRunning;
    std::atomic<size_t> activeTaskCount{0};
};