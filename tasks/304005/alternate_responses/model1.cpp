#include <iostream>
#include <vector>
#include <pthread.h>
#include <stdexcept>
#include <algorithm>

template <typename T>
class Stack {
private:
    // Data storage
    std::vector<T> data;
    size_t capacity;

    // Synchronization primitives
    mutable pthread_mutex_t mutex;
    mutable pthread_cond_t not_full;
    mutable pthread_cond_t not_empty;

    // Resizing logic
    void resize(size_t newCapacity) {
        // Note: This method should only be called when mutex is already locked
        if (newCapacity < data.size()) {
            return; // Cannot resize smaller than current data size
        }
        
        try {
            data.reserve(newCapacity);
            capacity = newCapacity;
        } catch (const std::bad_alloc& e) {
            std::cerr << "Memory allocation failed during resize: " << e.what() << std::endl;
        }
    }

public:
    // Constructor
    Stack(size_t maxCapacity) : capacity(maxCapacity) {
        pthread_mutex_init(&mutex, nullptr);
        pthread_cond_init(&not_full, nullptr);
        pthread_cond_init(&not_empty, nullptr);
        data.reserve(capacity);
    }

    // Destructor
    ~Stack() {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&not_full);
        pthread_cond_destroy(&not_empty);
    }

    // Core Operations
    void push(const T& element) {
        pthread_mutex_lock(&mutex);

        while (data.size() >= capacity) {
            // Attempt to resize if stack is full
            size_t newCapacity = capacity * 2;
            if (newCapacity > capacity) { // Check for overflow
                resize(newCapacity);
                if (data.size() < capacity) {
                    break;
                }
            }
            pthread_cond_wait(&not_full, &mutex);
        }

        try {
            data.push_back(element);
            pthread_cond_signal(&not_empty);
        } catch (const std::exception& e) {
            std::cerr << "Error during push operation: " << e.what() << std::endl;
        }

        pthread_mutex_unlock(&mutex);
    }

    void pop() {
        pthread_mutex_lock(&mutex);

        while (data.empty()) {
            std::cerr << "Stack underflow: Cannot pop element, stack is empty.\n";
            pthread_cond_wait(&not_empty, &mutex);
        }

        data.pop_back();
        pthread_cond_signal(&not_full);

        // Consider shrinking capacity if utilization is low
        if (data.size() < capacity / 4 && capacity > 16) {
            resize(capacity / 2);
        }

        pthread_mutex_unlock(&mutex);
    }

    T top() const {
        pthread_mutex_lock(&mutex);

        if (data.empty()) {
            pthread_mutex_unlock(&mutex);
            throw std::runtime_error("Stack is empty: No top element.");
        }

        T result = data.back();
        pthread_mutex_unlock(&mutex);
        return result;
    }

    // Utility Functions
    bool isEmpty() const {
        pthread_mutex_lock(&mutex);
        bool empty = data.empty();
        pthread_mutex_unlock(&mutex);
        return empty;
    }

    bool isFull() const {
        pthread_mutex_lock(&mutex);
        bool full = (data.size() >= capacity);
        pthread_mutex_unlock(&mutex);
        return full;
    }

    size_t getCapacity() const {
        pthread_mutex_lock(&mutex);
        size_t current_capacity = capacity;
        pthread_mutex_unlock(&mutex);
        return current_capacity;
    }

    size_t getSize() const {
        pthread_mutex_lock(&mutex);
        size_t size = data.size();
        pthread_mutex_unlock(&mutex);
        return size;
    }
};
