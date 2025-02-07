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

enum class Priority { Low, Medium, High };

struct Task {
    Priority priority;
    size_t order;
    std::function<void()> func;
    
    bool operator<(const Task& other) const {
        if (priority != other.priority)
            return priority < other.priority;
        return order > other.order;
    }
};

class ThreadPool {
public:
    ThreadPool(size_t numThreads)
        : stop_(false)
        , taskOrder_(0)
        , activeThreads_(0)
        , totalTasksExecuted_(0)
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

    template <class F, class... Args>
    auto submit(Priority priority, F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            if(stop_) {
                throw std::runtime_error("Submit on stopped ThreadPool");
            }

            tasks_.push(Task{
                priority,
                taskOrder_++,
                [task](){ (*task)(); }
            });
        }
        condition_.notify_one();
        return res;
    }

    template <class F, class... Args>
    auto submit(Priority priority,
                std::vector<std::future<void>>&& dependencies,
                F&& f, Args&&... args)
        -> std::future<typename std::result_of<F(Args...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            [deps = std::move(dependencies), 
             func = std::bind(std::forward<F>(f), std::forward<Args>(args)...)]() mutable {
                for(auto& dep : deps) {
                    dep.wait();
                }
                return func();
            }
        );

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            if(stop_) {
                throw std::runtime_error("Submit on stopped ThreadPool");
            }

            tasks_.push(Task{
                priority,
                taskOrder_++,
                [task](){ (*task)(); }
            });
        }
        condition_.notify_one();
        return res;
    }

    void resize(size_t newSize) {
        std::unique_lock<std::mutex> lock(queueMutex_);
        
        size_t currentSize = workers_.size();
        if (newSize > currentSize) {
            // Spawn additional threads
            for (size_t i = 0; i < newSize - currentSize; ++i) {
                createWorker();
            }
        } else if (newSize < currentSize) {
            // Mark excess threads for termination
            size_t threadsToRemove = currentSize - newSize;
            for (size_t i = 0; i < threadsToRemove; ++i) {
                tasks_.push(Task{
                    Priority::High,
                    taskOrder_++,
                    [this]() { terminateWorker(); }
                });
            }
        }
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

protected:
    void createWorker() {
        workers_.emplace_back([this] {
            while(true) {
                Task task;
                {
                    std::unique_lock<std::mutex> lock(queueMutex_);
                    condition_.wait(lock, [this] {
                        return stop_.load() || !tasks_.empty();
                    });

                    if(stop_.load() && tasks_.empty()) {
                        return;
                    }

                    task = std::move(tasks_.top());
                    tasks_.pop();
                }

                activeThreads_++;
                task.func();
                totalTasksExecuted_++;
                activeThreads_--;
            }
        });
    }

    void createWorkers(size_t numThreads) {
        for(size_t i = 0; i < numThreads; ++i) {
            createWorker();
        }
    }

    void terminateWorker() {
        std::unique_lock<std::mutex> lock(queueMutex_);
        if (!workers_.empty()) {
            workers_.back().detach();
            workers_.pop_back();
        }
    }

private:
    std::vector<std::thread> workers_;
    std::priority_queue<Task> tasks_;
    
    mutable std::mutex queueMutex_;
    std::condition_variable condition_;
    std::atomic<bool> stop_;
    
    size_t taskOrder_;
    std::atomic<size_t> activeThreads_;
    std::atomic<size_t> totalTasksExecuted_;
};

#endif // THREADPOOL_H
