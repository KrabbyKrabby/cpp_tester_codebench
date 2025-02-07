#ifndef DATA_PIPELINE_H
#define DATA_PIPELINE_H

#include <functional>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <stdexcept>
#include <thread>
#include <future>

template <typename T>
class DataPipeline {
public:
    explicit DataPipeline(const std::vector<std::function<T(const T&)>>& stages)
        : stages(stages), stopFlag(false), concurrencyLevel(1), activeItems(0)
    {
        if (stages.empty()) {
            throw std::invalid_argument("Pipeline must have at least one stage.");
        }
    }

    void push(const T& item) {
        std::lock_guard<std::mutex> lock(mutex);
        if (stopFlag) {
            throw std::runtime_error("Cannot push to a stopped pipeline.");
        }
        if (concurrencyLevel == 1) {
            processItemInAllStages(item);
        } else {
            itemQueue.push(item);
            ++activeItems;
            queueCondition.notify_one();
        }
    }

    void enableParallelism(size_t level) {
        if (level < 1) {
            throw std::invalid_argument("Concurrency level cannot be zero.");
        }

        std::lock_guard<std::mutex> lock(mutex);
        if (!workers.empty()) {
            throw std::runtime_error("Parallelism already enabled");
        }

        concurrencyLevel = level;
        if (level > 1) {
            for (size_t i = 0; i < level; ++i) {
                workers.emplace_back([this]() {
                    workerFunction();
                });
            }
        }
    }

    void setErrorHandler(std::function<void(const std::exception&)> handler) {
        std::lock_guard<std::mutex> lock(mutex);
        errorHandler = handler;
    }

    void drain() {
        if (concurrencyLevel == 1) return;

        std::unique_lock<std::mutex> lock(mutex);
        drainCondition.wait(lock, [this]() {
            return activeItems == 0 && itemQueue.empty();
        });
    }

    void stop() {
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (stopFlag) return;
            stopFlag = true;
        }

        queueCondition.notify_all();
        
        for (auto& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
        workers.clear();
    }

    bool isParallel() const {
        return concurrencyLevel > 1;
    }

    size_t getConcurrencyLevel() const {
        return concurrencyLevel;
    }

    size_t queueSize() const {
        std::lock_guard<std::mutex> lock(mutex);
        return itemQueue.size();
    }

private:
    void processItemInAllStages(const T& item) {
        T current = item;
        try {
            for (auto& stage : stages) {
                current = stage(current);
            }
        } catch (const std::exception& e) {
            if (errorHandler) {
                errorHandler(e);
            } else {
                throw;
            }
        }
    }

    void workerFunction() {
        while (true) {
            T item;
            {
                std::unique_lock<std::mutex> lock(mutex);
                queueCondition.wait(lock, [this]() {
                    return stopFlag || !itemQueue.empty();
                });

                if (stopFlag && itemQueue.empty()) {
                    break;
                }

                item = itemQueue.front();
                itemQueue.pop();
            }

            try {
                processItemInAllStages(item);
            } catch (...) {
                // Error already handled in processItemInAllStages
            }

            {
                std::lock_guard<std::mutex> lock(mutex);
                --activeItems;
                if (activeItems == 0 && itemQueue.empty()) {
                    drainCondition.notify_all();
                }
            }
        }
    }

    std::vector<std::function<T(const T&)>> stages;
    std::atomic<bool> stopFlag;
    size_t concurrencyLevel;
    mutable std::mutex mutex;
    std::condition_variable queueCondition;
    std::condition_variable drainCondition;
    std::queue<T> itemQueue;
    std::vector<std::thread> workers;
    std::function<void(const std::exception&)> errorHandler;
    std::atomic<size_t> activeItems;
};

#endif // DATA_PIPELINE_H