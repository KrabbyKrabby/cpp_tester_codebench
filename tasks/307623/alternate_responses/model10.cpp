// ThreadPool.h
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
#include <iostream>

// Enum for task priority levels
enum class Priority { High, Medium, Low };

// Struct to represent a Task with priority and order for FIFO within the same priority
struct Task {
    Priority priority;
    size_t order;
    std::function<void()> func;

    bool operator<(const Task& other) const {
        // Higher priority tasks come first
        if (priority != other.priority)
            return priority > other.priority;
        // Within same priority, earlier order comes first (FIFO)
        return order > other.order;
    }
};

class ThreadPool {
public:
    ThreadPool(size_t numThreads)
        : stop_(false), totalTasksExecuted_(0)
    {
        for(size_t i = 0;i<numThreads;++i)
            workers_.emplace_back(
                [this]
                {
                    while(true)
                    {
                        std::function<void()> task;

                        {
                            std::unique_lock<std::mutex> lock(this->queueMutex_);
                            this->condition_.wait(lock, 
                                [this]{ return this->stop_.load() || !this->tasks_.empty(); });
                            if(this->stop_.load() && this->tasks_.empty())
                                return;
                            task = std::move(this->tasks_.front());
                            this->tasks_.pop();
                        }

                        task();
                        ++totalTasksExecuted_;
                    }
                }
            );
    }

    ~ThreadPool()
    {
        shutdown();
    }

    void shutdown()
    {
        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            stop_.store(true);
        }
        condition_.notify_all();
        for(std::thread &worker: workers_)
            if(worker.joinable())
                worker.join();
    }

    void resize(size_t newSize)
    {
        if (newSize > workers_.size()) {
            for (size_t i = workers_.size(); i < newSize; ++i) {
                workers_.emplace_back(
                    [this]
                    {
                        while(true)
                        {
                            std::function<void()> task;

                            {
                                std::unique_lock<std::mutex> lock(this->queueMutex_);
                                this->condition_.wait(lock, 
                                    [this]{ return this->stop_.load() || !this->tasks_.empty(); });
                                if(this->stop_.load() && this->tasks_.empty())
                                    return;
                                task = std::move(this->tasks_.front());
                                this->tasks_.pop();
                            }

                            task();
                            ++totalTasksExecuted_;
                        }
                    }
                );
            }
        } else if (newSize < workers_.size()) {
            {
                std::unique_lock<std::mutex> lock(queueMutex_);
                stop_.store(true);
            }
            condition_.notify_all();
            for (size_t i = newSize; i < workers_.size(); ++i) {
                if (workers_[i].joinable()) {
                    workers_[i].join();
                }
            }
            workers_.resize(newSize);
            stop_.store(false);
        }
    }

    template <class F, class... Args>
    auto submit(Priority priority, F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queueMutex_);

            // Don't allow enqueueing after stopping the pool
            if(stop_) {
                throw std::runtime_error("Submit on stopped ThreadPool");
            }

            tasks_.emplace(Task{priority, tasks_.size(), [task](){ (*task)(); }});
        }
        condition_.notify_one();
        return res;
    }

    template <class F, class... Args>
    auto submit(Priority priority, std::vector<std::future<void>>&& dependencies, F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queueMutex_);

            // Don't allow enqueueing after stopping the pool
            if(stop_) {
                throw std::runtime_error("Submit on stopped ThreadPool");
            }

            tasks_.emplace(Task{priority, tasks_.size(), 
                [task, dependencies = std::move(dependencies)](){
                    for (auto& dep : dependencies) {
                        dep.wait();
                    }
                    (*task)();
                }
            });
        }
        condition_.notify_one();
        return res;
    }

    size_t getTaskQueueSize() const {
        std::unique_lock<std::mutex> lock(queueMutex_);
        return tasks_.size();
    }

    size_t getActiveThreadCount() const {
        std::unique_lock<std::mutex> lock(queueMutex_);
        return workers_.size();
    }

    size_t getTotalTasksExecuted() const {
        std::unique_lock<std::mutex> lock(queueMutex_);
        return totalTasksExecuted_;
    }

protected:
    // Worker threads
    std::vector<std::thread> workers_;

    // Task queue
    std::priority_queue<Task> tasks_;

    // Synchronization
    std::mutex queueMutex_;
    std::condition_variable condition_;
    std::atomic<bool> stop_;
    std::atomic<size_t> totalTasksExecuted_;
};

#endif // THREADPOOL_H
