#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <future>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>

enum class Priority {
    High,
    Medium,
    Low
};

struct Task {
    Priority priority;
    size_t order;
    std::function<void()> func;
    std::vector<std::shared_future<void>> dependencies;

    bool operator<(const Task& other) const {
        if (priority != other.priority)
            return priority > other.priority;
        return order > other.order;
    }
};

class ThreadPool {
public:
    ThreadPool(size_t numThreads)
        : stop_(false), 
          taskOrder_(0),
          activeThreads_(0),
          totalTasksExecuted_(0)
    {
        createWorkers(numThreads);
    }

    ~ThreadPool() {
        shutdown();
    }

    void shutdown() {
        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            stop_.store(true);
        }
        condition_.notify_all();
        for(std::thread &worker: workers_) {
            if(worker.joinable())
                worker.join();
        }
    }

    void resize(size_t newSize) {
        std::unique_lock<std::mutex> lock(queueMutex_);
        
        size_t currentSize = workers_.size();
        if (newSize > currentSize) {
            // Spawn additional threads
            size_t threadsToAdd = newSize - currentSize;
            createWorkers(threadsToAdd);
        }
        else if (newSize < currentSize) {
            // Mark excess threads for termination
            size_t threadsToRemove = currentSize - newSize;
            for (size_t i = 0; i < threadsToRemove; ++i) {
                tasks_.emplace(Task{
                    Priority::High,
                    taskOrder_++,
                    [this]() { throw ThreadTerminationException(); }
                });
            }
        }
        condition_.notify_all();
    }

    template <class F, class... Args>
    auto submit(Priority priority, F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type;
        return submitInternal(priority, std::vector<std::future<void>>(),
                            std::forward<F>(f), std::forward<Args>(args)...);
    }

    template <class F, class... Args>
    auto submit(Priority priority,
                std::vector<std::future<void>>&& dependencies,
                F&& f, Args&&... args)
        -> std::future<typename std::result_of<F(Args...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type;
        return submitInternal(priority, std::move(dependencies),
                            std::forward<F>(f), std::forward<Args>(args)...);
    }

    size_t getTaskQueueSize() const {
        std::unique_lock<std::mutex> lock(queueMutex_);
        return tasks_.size();
    }

    size_t getActiveThreadCount() const {
        return activeThreads_.load();
    }

    size_t getTotalTasksExecuted() const {
        return totalTasksExecuted_.load();
    }

private:
    class ThreadTerminationException {};

    template <class F, class... Args>
    auto submitInternal(Priority priority,
                       std::vector<std::future<void>> dependencies,
                       F&& f, Args&&... args)
        -> std::future<typename std::result_of<F(Args...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<return_type> result = task->get_future();

        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            if(stop_) {
                throw std::runtime_error("Submit on stopped ThreadPool");
            }

            // Convert dependencies to shared_futures for multiple gets
            std::vector<std::shared_future<void>> sharedDeps;
            for (auto& dep : dependencies) {
                sharedDeps.push_back(dep.share());
            }

            tasks_.emplace(Task{
                priority,
                taskOrder_++,
                [task, sharedDeps]() {
                    // Wait for all dependencies to complete
                    for (auto& dep : sharedDeps) {
                        dep.get();
                    }
                    (*task)();
                }
            });
        }
        condition_.notify_one();
        return result;
    }

    void createWorkers(size_t count) {
        for(size_t i = 0; i < count; ++i) {
            workers_.emplace_back([this] {
                while(true) {
                    Task task;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex_);
                        condition_.wait(lock, [this] {
                            return stop_ || !tasks_.empty();
                        });

                        if(stop_ && tasks_.empty()) {
                            return;
                        }

                        task = std::move(tasks_.top());
                        tasks_.pop();
                    }

                    try {
                        activeThreads_++;
                        task.func();
                        totalTasksExecuted_++;
                        activeThreads_--;
                    }
                    catch(const ThreadTerminationException&) {
                        return;
                    }
                    catch(...) {
                        activeThreads_--;
                        throw;
                    }
                }
            });
        }
    }

    std::vector<std::thread> workers_;
    std::priority_queue<Task> tasks_;
    mutable std::mutex queueMutex_;
    std::condition_variable condition_;
    std::atomic<bool> stop_;
    std::atomic<size_t> taskOrder_;
    std::atomic<size_t> activeThreads_;
    std::atomic<size_t> totalTasksExecuted_;
};

#endif // THREADPOOL_H
