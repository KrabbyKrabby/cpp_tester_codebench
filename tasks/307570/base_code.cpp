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
    // (TODO) For parallel mode, the item should be queued and processed by worker threads.
    void push(const T& item) {
        std::lock_guard<std::mutex> lock(mutex);
        if (stopFlag) {
            throw std::runtime_error("Cannot push to a stopped pipeline.");
        }
        if (concurrencyLevel == 1) {
            // Single-threaded immediate processing.
            processItemInAllStages(item);
        } else {
            // TODO: In parallel mode, enqueue the item for worker threads to process.
            // Possibly notify a condition variable.
        }
    }

    // (TODO) Enables or disables concurrency with the given number of worker threads.
    // Must be called before pushing items. 
    // If concurrencyLevel > 1, the pipeline spawns worker threads that process items in parallel.
    // Use at least a thread-safe queue to hold incoming items, and a mechanism to 
    // distribute them across multiple threads.
    void enableParallelism(size_t concurrencyLevel) {
        // TODO: Implement logic to set concurrency, spawn threads, and handle safe shutdown.
        // HINT: store concurrencyLevel in a class member, create worker threads that repeatedly 
        // grab an item from a thread-safe queue, process it, etc.
        // Must be careful with stopFlag and draining logic.
        if (concurrencyLevel < 1) {
            throw std::invalid_argument("Concurrency level cannot be zero.");
        }
    }

    // (TODO) A user-defined function that is called if any stage throws an exception.
    // The pipeline code should catch stage exceptions and invoke this callback.
    // If no callback is set, rethrow the exception to halt processing.
    void setErrorHandler(std::function<void(const std::exception&)> handler) {
        // TODO: Store the handler for usage in stage execution.
    }

    // (TODO) Drains the pipeline: block until all queued items are fully processed 
    // (or until an unrecoverable error occurs).
    // For single-thread mode, this might do nothing if each push is processed synchronously. 
    // For parallel mode, it must wait for all worker threads to complete processing of existing items.
    void drain() {
        // TODO: Implement logic to wait until the pipeline is empty and 
        // no in-flight items remain in parallel mode.
    }

    // Stops the pipeline and releases resources. 
    // After calling stop(), no new items can be pushed.
    // Worker threads (if any) should finish processing current items but ignore new pushes.
    // This function can be called multiple times safely.
    void stop() {
        std::lock_guard<std::mutex> lock(mutex);
        stopFlag = true;
        // For concurrency: notify worker threads, shut them down gracefully.
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
    // (TODO) Implement properly when concurrency is enabled.
    size_t queueSize() const {
        // TODO: Return the current number of items in the queue in parallel mode.
        return 0; // default
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
            // TODO: If user errorHandler is set, call it. Otherwise rethrow.
            // For now, just rethrow.
            throw;
        }
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

    // (TODO) condition_variable, or other data structures to manage concurrency in parallel mode.
    // e.g. an std::queue<T> itemQueue, plus worker threads that pop from itemQueue.

    // (TODO) user-defined error handler to handle stage exceptions.
    // e.g. std::function<void(const std::exception&)> errorHandler;
};

#endif // DATA_PIPELINE_H
