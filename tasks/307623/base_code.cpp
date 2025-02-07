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
        : stop_(false)
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


    template <class F, class... Args>
    auto ThreadPool::submit(F&& f, Args&&... args) 
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

            tasks_.emplace([task](){ (*task)(); });
        }
        condition_.notify_one();
        return res;
    }

    size_t getTaskQueueSize() const{

    }

    size_t getActiveThreadCount() const{

    }

    size_t getTotalTasksExecuted() const{
        
    }


protected:
    // Worker threads
    std::vector<std::thread> workers_;

    // Task queue
    std::queue<std::function<void()>> tasks_;

    // Synchronization
    std::mutex queueMutex_;
    std::condition_variable condition_;
    std::atomic<bool> stop_;
};


#endif // THREADPOOL_H