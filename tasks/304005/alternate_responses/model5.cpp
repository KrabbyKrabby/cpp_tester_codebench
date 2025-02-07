#include <iostream>
#include <vector>
#include <pthread.h>
#include <stdexcept>
#include <algorithm>

template <typename T>
class Stack {
private:
    // Data Members
    std::vector<T> data;
    size_t capacity;
    
    // Synchronization primitives
    mutable pthread_mutex_t mutex;
    mutable pthread_cond_t not_full;
    mutable pthread_cond_t not_empty;

    // Resizing Logic
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

    // Dynamic capacity adjustment
    void adjustCapacity() {
        // Note: This method should only be called when mutex is already locked
        if (data.size() >= capacity * 0.9) {  // 90% full - grow
            resize(capacity * 2);
        } else if (data.size() <= capacity * 0.25 && capacity > 10) {  // 25% full - shrink
            resize(capacity / 2);
        }
    }

public:
    // Constructor
    Stack(size_t maxCapacity = 10) : capacity(maxCapacity) {
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

    // Core Stack Operations
    void push(const T& element) {
        pthread_mutex_lock(&mutex);
        
        while (data.size() >= capacity) {
            pthread_cond_wait(&not_full, &mutex);
        }

        try {
            data.push_back(element);
            adjustCapacity();
            pthread_cond_signal(&not_empty);
        } catch (...) {
            std::cerr << "Stack overflow: Cannot push element, stack is full.\n";
        }

        pthread_mutex_unlock(&mutex);
    }

    void pop() {
        pthread_mutex_lock(&mutex);
        
        if (data.empty()) {
            std::cerr << "Stack underflow: Cannot pop element, stack is empty.\n";
            pthread_mutex_unlock(&mutex);
            return;
        }

        data.pop_back();
        adjustCapacity();
        pthread_cond_signal(&not_full);
        
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

    // State Check Operations
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

    // Utility Methods
    size_t getCurrentCapacity() const {
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
