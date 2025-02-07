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

enum class Priority { Low, Medium, High };

// Task struct with priority and ordering
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
            for (size_t i = currentSize; i < newSize; ++i) {
                createWorker();
            }
        }
        else if (newSize < currentSize) {
            // Mark excess threads for termination
            threadsToTerminate_ = currentSize - newSize;
            condition_.notify_all();
        }
    }

    template <class F, class... Args>
    auto submit(Priority priority, F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type;
        return submitTask(priority, std::vector<std::future<void>>(), 
                         std::forward<F>(f), std::forward<Args>(args)...);
    }

    template <class F, class... Args>
    auto submit(Priority priority,
                std::vector<std::future<void>>&& dependencies,
                F&& f, Args&&... args)
        -> std::future<typename std::result_of<F(Args...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type;
        return submitTask(priority, std::move(dependencies),
                         std::forward<F>(f), std::forward<Args>(args)...);
    }

    size_t getTaskQueueSize() const {
        std::lock_guard<std::mutex> lock(queueMutex_);
        return tasks_.size();
    }

    size_t getActiveThreadCount() const {
        return activeThreads_.load();
    }

    size_t getTotalTasksExecuted() const {
        return totalTasksExecuted_.load();
    }

protected:
    template <class F, class... Args>
    auto submitTask(Priority priority,
                   std::vector<std::future<void>> dependencies,
                   F&& f, Args&&... args)
        -> std::future<typename std::result_of<F(Args...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            [func = std::bind(std::forward<F>(f), std::forward<Args>(args)...),
             deps = std::move(dependencies)]() {
                // Wait for all dependencies to complete
                for (auto& dep : deps) {
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

    void createWorker() {
        workers_.emplace_back([this] {
            while(true) {
                Task task;
                {
                    std::unique_lock<std::mutex> lock(queueMutex_);
                    condition_.wait(lock, [this] {
                        return stop_ || !tasks_.empty() || threadsToTerminate_ > 0;
                    });

                    if((stop_ && tasks_.empty()) || 
                       (threadsToTerminate_ > 0 && tasks_.empty())) {
                        if (threadsToTerminate_ > 0) {
                            --threadsToTerminate_;
                        }
                        return;
                    }

                    if(tasks_.empty()) continue;

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

private:
    std::vector<std::thread> workers_;
    std::priority_queue<Task> tasks_;
    
    mutable std::mutex queueMutex_;
    std::condition_variable condition_;
    std::atomic<bool> stop_;
    
    size_t taskOrder_;
    std::atomic<size_t> threadsToTerminate_{0};
    std::atomic<size_t> activeThreads_;
    std::atomic<size_t> totalTasksExecuted_;
};

#endif // THREADPOOL_H
