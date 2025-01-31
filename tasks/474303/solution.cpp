#include <functional>
#include <queue>
#include <vector>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <mutex>
#include <condition_variable>
#include <future>
#include <iostream>
#include <atomic>

// Task status enumeration
enum class TaskStatus {
    PENDING,
    RUNNING,
    COMPLETED,
    CANCELED
};



// A structure to represent a task
struct Task {
    size_t id;
    std::function<void()> func;
    TaskStatus status;
    std::chrono::system_clock::time_point scheduledTime;

    bool operator>(const Task& other) const {
        return scheduledTime > other.scheduledTime;
    }
};

class TaskScheduler {
public:
    TaskScheduler() : nextTaskId(0), stopWorkers(false) {}

    ~TaskScheduler() {
        stopAllWorkers();
    }

    // Add a task to be executed immediately
    size_t addTask(const std::function<void()>& func) {
        auto now = std::chrono::system_clock::now();
        return scheduleTask(now, func);
    }

    // Schedule a task to be executed no earlier than the specified time
    size_t scheduleTask(std::chrono::system_clock::time_point time, const std::function<void()>& func) {
        std::lock_guard<std::mutex> lock(mtx);
        size_t id = nextTaskId++;
        Task task{id, func, TaskStatus::PENDING, time};
        tasks[id] = task;
        taskQueue.push(task);
        cv.notify_all();
        return id;
    }

    // Cancel a specific task if it's still pending
    bool cancelTask(size_t taskId) {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = tasks.find(taskId);
        if (it == tasks.end()) {
            return false;
        }

        if (it->second.status == TaskStatus::PENDING) {

            it->second.status = TaskStatus::CANCELED;
            return true;
        }

        return false;
    }


    // Run the next available task in a single-threaded manner
    void runNext() {
        std::unique_lock<std::mutex> lock(mtx);

        auto now = std::chrono::system_clock::now();
        while (!taskQueue.empty()) {
            Task task = taskQueue.top();

            // If the task is not ready, exit without popping
            if (task.scheduledTime > now || task.status != TaskStatus::PENDING) {
                return;
            }

            if (tasks[task.id].status == TaskStatus::CANCELED) {
                taskQueue.pop();  // Remove from queue and continue
                continue;
            }

            // Pop and run the task
            taskQueue.pop();
            tasks[task.id].status = TaskStatus::RUNNING;
            lock.unlock();

            task.func();

            lock.lock();
            tasks[task.id].status = TaskStatus::COMPLETED;
            break;
        }
    }


    // Execute all tasks concurrently
    void runAllConcurrently() {
        std::lock_guard<std::mutex> lock(mtx);
        if (!workers.empty()) {
            return; // Workers already running
        }

        stopWorkers = false;
        size_t numThreads = std::thread::hardware_concurrency();
        if (numThreads == 0) {
            numThreads = 2;
        }

        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back(&TaskScheduler::workerThread, this);
        }
    }

    // Wait for all tasks to complete
    void waitAll() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]() {
            for (const auto& [id, task] : tasks) {
                if (task.status == TaskStatus::PENDING || task.status == TaskStatus::RUNNING) {
                    return false;
                }
            }
            return true;
        });
    }

    // Get the status of a specific task
    TaskStatus getTaskStatus(size_t taskId) const {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = tasks.find(taskId);
        if (it == tasks.end()) {
            throw std::out_of_range("Task ID not found.");
        }
        return it->second.status;
    }


private:
    mutable std::mutex mtx;
    std::condition_variable cv;
    std::priority_queue<Task, std::vector<Task>, std::greater<Task>> taskQueue;
    std::unordered_map<size_t, Task> tasks;
    std::vector<std::thread> workers;
    size_t nextTaskId;
    std::atomic<bool> stopWorkers;

    void workerThread() {
        while (!stopWorkers) {
            std::unique_lock<std::mutex> lock(mtx);

            auto now = std::chrono::system_clock::now();
            cv.wait(lock, [this, now]() {
                return stopWorkers || (!taskQueue.empty() && taskQueue.top().scheduledTime <= now);
            });

            if (stopWorkers) {
                break;
            }

            if (!taskQueue.empty()) {
                Task task = taskQueue.top();
                if (task.scheduledTime <= now && task.status == TaskStatus::PENDING) {
                    taskQueue.pop();
                    tasks[task.id].status = TaskStatus::RUNNING;
                    lock.unlock();

                    task.func();

                    lock.lock();
                    tasks[task.id].status = TaskStatus::COMPLETED;
                    cv.notify_all();
                }
            }
        }
    }

    void stopAllWorkers() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            stopWorkers = true;
            cv.notify_all();
        }

        for (auto& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }

        workers.clear();
    }
};