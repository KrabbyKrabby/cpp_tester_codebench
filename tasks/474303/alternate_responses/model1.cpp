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
    TaskScheduler(size_t numThreads = std::thread::hardware_concurrency())
        : nextTaskId(0), running(true) {
        startThreadPool(numThreads);
    }

    ~TaskScheduler() {
        {
            std::unique_lock<std::mutex> lock(mutex);
            running = false;
        }
        condVar.notify_all();
        for (auto& thread : threadPool) {
            if (thread.joinable()) {
                thread.join();
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
        
        if (taskPtr->scheduleTime > now) {
            return;
        }

        tasks.pop();
        if (taskPtr->status == TaskStatus::PENDING) {
            taskPtr->status = TaskStatus::RUNNING;
            lock.unlock();
            
            taskPtr->func();
            
            lock.lock();
            taskPtr->status = TaskStatus::COMPLETED;
        }
    }

    void runAllConcurrently() {
        std::unique_lock<std::mutex> lock(mutex);
        condVar.notify_all();
    }

    void waitAll() {
        std::unique_lock<std::mutex> lock(mutex);
        while (!tasks.empty() || activeTaskCount > 0) {
            completionVar.wait(lock);
        }
    }

    TaskStatus getTaskStatus(size_t taskId) const {
        std::unique_lock<std::mutex> lock(mutex);
        auto it = taskMap.find(taskId);
        if (it == taskMap.end()) {
            throw std::runtime_error("Task ID not found");
        }
        return it->second->status;
    }

    bool cancelTask(size_t taskId) {
        std::unique_lock<std::mutex> lock(mutex);
        auto it = taskMap.find(taskId);
        if (it == taskMap.end()) {
            return false;
        }

        if (it->second->status != TaskStatus::PENDING) {
            return false;
        }

        it->second->status = TaskStatus::CANCELED;
        return true;
    }

private:
    std::queue<std::shared_ptr<Task>> tasks;
    std::map<size_t, std::shared_ptr<Task>> taskMap;
    std::vector<std::thread> threadPool;
    mutable std::mutex mutex;
    std::condition_variable condVar;
    std::condition_variable completionVar;
    std::atomic<size_t> nextTaskId;
    std::atomic<bool> running;
    std::atomic<size_t> activeTaskCount{0};

    void startThreadPool(size_t numThreads) {
        for (size_t i = 0; i < numThreads; ++i) {
            threadPool.emplace_back([this] { threadWorker(); });
        }
    }

    void threadWorker() {
        while (running) {
            std::shared_ptr<Task> taskPtr;
            {
                std::unique_lock<std::mutex> lock(mutex);
                condVar.wait(lock, [this] { 
                    return !running || (!tasks.empty() && 
                           tasks.front()->scheduleTime <= std::chrono::system_clock::now()); 
                });

                if (!running && tasks.empty()) {
                    return;
                }

                if (!tasks.empty() && 
                    tasks.front()->scheduleTime <= std::chrono::system_clock::now()) {
                    taskPtr = tasks.front();
                    tasks.pop();
                }
            }

            if (taskPtr && taskPtr->status == TaskStatus::PENDING) {
                taskPtr->status = TaskStatus::RUNNING;
                activeTaskCount++;

                taskPtr->func();

                activeTaskCount--;
                taskPtr->status = TaskStatus::COMPLETED;
                completionVar.notify_one();
            }
        }
    }
};