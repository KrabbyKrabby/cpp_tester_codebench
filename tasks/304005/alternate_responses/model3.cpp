#include <iostream>
#include <vector>
#include <pthread.h>
#include <stdexcept>
#include <memory>

template <typename T>
class Stack {
private:
    // Data members
    std::vector<T> data;
    size_t capacity;
    
    // Synchronization primitives
    mutable pthread_mutex_t mutex;
    mutable pthread_cond_t not_full;
    mutable pthread_cond_t not_empty;

    // Private helper method for resizing
    void resize(size_t new_capacity) {
        // Note: This method should only be called when mutex is already locked
        if (new_capacity < data.size()) {
            return; // Cannot resize smaller than current data size
        }
        
        try {
            data.reserve(new_capacity);
            capacity = new_capacity;
        } catch (const std::bad_alloc& e) {
            throw std::runtime_error("Memory allocation failed during resize");
        }
    }

public:
    // Constructor
    explicit Stack(size_t maxCapacity = 10) : capacity(maxCapacity) {
        // Initialize synchronization primitives
        if (pthread_mutex_init(&mutex, nullptr) != 0) {
            throw std::runtime_error("Failed to initialize mutex");
        }
        if (pthread_cond_init(&not_full, nullptr) != 0) {
            pthread_mutex_destroy(&mutex);
            throw std::runtime_error("Failed to initialize condition variable (not_full)");
        }
        if (pthread_cond_init(&not_empty, nullptr) != 0) {
            pthread_mutex_destroy(&mutex);
            pthread_cond_destroy(&not_full);
            throw std::runtime_error("Failed to initialize condition variable (not_empty)");
        }

        // Initialize vector with initial capacity
        data.reserve(capacity);
    }

    // Destructor
    ~Stack() {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&not_full);
        pthread_cond_destroy(&not_empty);
    }

    // Push operation
    void push(const T& element) {
        pthread_mutex_lock(&mutex);
        
        try {
            if (data.size() >= capacity) {
                // Attempt to grow the stack
                size_t new_capacity = capacity * 2;
                resize(new_capacity);
            }
            
            data.push_back(element);
            pthread_cond_signal(&not_empty);
        } catch (...) {
            pthread_mutex_unlock(&mutex);
            throw;
        }
        
        pthread_mutex_unlock(&mutex);
    }

    // Pop operation
    void pop() {
        pthread_mutex_lock(&mutex);
        
        if (data.empty()) {
            std::cerr << "Stack underflow: Cannot pop element, stack is empty.\n";
            pthread_mutex_unlock(&mutex);
            return;
        }

        data.pop_back();
        pthread_cond_signal(&not_full);
        
        // Shrink capacity if utilization is low
        if (data.size() < capacity / 4 && capacity > 10) {
            resize(capacity / 2);
        }
        
        pthread_mutex_unlock(&mutex);
    }

    // Top operation
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

    // Check if empty
    bool isEmpty() const {
        pthread_mutex_lock(&mutex);
        bool empty = data.empty();
        pthread_mutex_unlock(&mutex);
        return empty;
    }

    // Check if full
    bool isFull() const {
        pthread_mutex_lock(&mutex);
        bool full = (data.size() >= capacity);
        pthread_mutex_unlock(&mutex);
        return full;
    }

    // Get current size
    size_t size() const {
        pthread_mutex_lock(&mutex);
        size_t current_size = data.size();
        pthread_mutex_unlock(&mutex);
        return current_size;
    }

    // Get current capacity
    size_t getCapacity() const {
        pthread_mutex_lock(&mutex);
        size_t current_capacity = capacity;
        pthread_mutex_unlock(&mutex);
        return current_capacity;
    }

    // Delete copy constructor and assignment operator
    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;
};
