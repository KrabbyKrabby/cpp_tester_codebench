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
    // Constructs the pipeline with given stages (function objects).
    // Each stage is a function T -> T for simplicity. 
    explicit DataPipeline(const std::vector<std::function<T(const T&)>>& stages)
        : stages(stages), stopFlag(false), concurrencyLevel(1)
    {
        if (stages.empty()) {
            throw std::invalid_argument("Pipeline must have at least one stage.");
        }
    }

    // Adds a single item to the pipeline's input queue.
    // In single-threaded mode, this item is processed immediately in a blocking manner.
    // For parallel mode, the item should be queued and processed by worker threads.
    void push(const T& item) {
        std::lock_guard<std::mutex> lock(mutex);
        if (stopFlag) {
            throw std::runtime_error("Cannot push to a stopped pipeline.");
        }
        if (concurrencyLevel == 1) {
            // Single-threaded immediate processing.
            processItemInAllStages(item);
        } else {
            // In parallel mode, enqueue the item for worker threads to process.
            itemQueue.push(item);
            cv.notify_one(); // Notify a waiting worker thread.
        }
    }

    // Enables or disables concurrency with the given number of worker threads.
    // Must be called before pushing items. 
    // If concurrencyLevel > 1, the pipeline spawns worker threads that process items in parallel.
    // Use at least a thread-safe queue to hold incoming items, and a mechanism to 
    // distribute them across multiple threads.
    void enableParallelism(size_t concurrencyLevel) {
        if (concurrencyLevel < 1) {
            throw std::invalid_argument("Concurrency level cannot be zero.");
        }

        std::lock_guard<std::mutex> lock(mutex);
        if (this->concurrencyLevel > 1) {
            // Already in parallel mode. Wait for worker threads to finish.
            stopWorkerThreads();
        }

        this->concurrencyLevel = concurrencyLevel;

        if (concurrencyLevel > 1) {
            // Spawn worker threads.
            workerThreads.reserve(concurrencyLevel);
            for (size_t i = 0; i < concurrencyLevel; ++i) {
                workerThreads.emplace_back([this] {
                    workerThreadMain();
                });
            }
        }
    }

    // A user-defined function that is called if any stage throws an exception.
    // The pipeline code should catch stage exceptions and invoke this callback.
    // If no callback is set, rethrow the exception to halt processing.
    void setErrorHandler(std::function<void(const std::exception&)> handler) {
        errorHandler = std::move(handler);
    }

    // Drains the pipeline: block until all queued items are fully processed 
    // (or until an unrecoverable error occurs).
    // For single-thread mode, this might do nothing if each push is processed synchronously. 
    // For parallel mode, it must wait for all worker threads to complete processing of existing items.
    void drain() {
        if (concurrencyLevel == 1) {
            // Single-threaded mode: nothing to drain.
            return;
        }

        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [this] { return itemQueue.empty(); });
    }

    // Stops the pipeline and releases resources. 
    // After calling stop(), no new items can be pushed.
    // Worker threads (if any) should finish processing current items but ignore new pushes.
    // This function can be called multiple times safely.
    void stop() {
        std::lock_guard<std::mutex> lock(mutex);
        stopFlag = true;
        cv.notify_all(); // Notify all waiting worker threads.

        if (concurrencyLevel > 1) {
            stopWorkerThreads();
        }
    }

    // Returns true if concurrency is enabled.
    bool isParallel() const {
        return concurrencyLevel > 1;
    }

    // For testing: returns the number of worker threads (0 or concurrencyLevel if in parallel mode).
    size_t getConcurrencyLevel() const {
        return concurrencyLevel;
    }

    // For debugging or advanced usage, allows checking the size of the item queue.
    // For single-threaded mode, it's always 0 or 1.
    size_t queueSize() const {
        std::lock_guard<std::mutex> lock(mutex);
        return itemQueue.size();
    }

private:
    // Helper method that processes a single item in all pipeline stages (synchronously).
    // Catches exceptions from stages. If an exception occurs and an error handler is defined, call it.
    // If no handler is defined, rethrow the exception.
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

    // Main function for each worker thread.
    void workerThreadMain() {
        while (true) {
            T item;
            {
                std::unique_lock<std::mutex> lock(mutex);
                cv.wait(lock, [this] { return stopFlag || !itemQueue.empty(); });
                if (stopFlag && itemQueue.empty()) {
                    break; // Exit the worker thread.
                }

                item = std::move(itemQueue.front());
                itemQueue.pop();
            }

            processItemInAllStages(item);
        }
    }

    // Stops all worker threads.
    void stopWorkerThreads() {
        for (auto& thread : workerThreads) {
            thread.join();
        }
        workerThreads.clear();
    }

    // Vector of stages, each a function T -> T.
    std::vector<std::function<T(const T&)>> stages;

    // Whether or not we've been stopped.
    bool stopFlag;

    // Current concurrency level: 1 means single-threaded immediate processing.
    // >1 means we have concurrencyLevel-1 worker threads or concurrencyLevel threads.
    size_t concurrencyLevel;

    // Mutex for general pipeline state.
    mutable std::mutex mutex;

    // Condition variable for worker threads to wait on.
    std::condition_variable cv;

    // Queue of items to process.
    std::queue<T> itemQueue;

    // User-defined error handler to handle stage exceptions.
    std::function<void(const std::exception&)> errorHandler;

    // Worker threads for parallel processing.
    std::vector<std::thread> workerThreads;
};

#endif // DATA_PIPELINE_H
