#include <iostream>
#include <vector>
#include <stdexcept>
#include <pthread.h>

template <typename T>
class Stack {
private:
    std::vector<T> data; // Internal storage for stack elements
    size_t capacity;     // Maximum capacity of the stack
    mutable pthread_mutex_t mtx; // Mutex for synchronization
    mutable pthread_cond_t cond; // Condition variable for signaling

    // Resizing Logic
    void resize(size_t newCapacity) {
        if (newCapacity <= capacity) return; // Avoid unnecessary resizing
        std::vector<T> newData(newCapacity);
        for (size_t i = 0; i < data.size(); ++i) {
            newData[i] = data[i];
        }
        data.swap(newData);
        capacity = newCapacity;
    }

public:
    // Constructor with specified capacity
    Stack(size_t maxCapacity) : capacity(maxCapacity), data(maxCapacity) {
        pthread_mutex_init(&mtx, nullptr);
        pthread_cond_init(&cond, nullptr);
    }

    // Destructor to clean up synchronization primitives
    ~Stack() {
        pthread_mutex_destroy(&mtx);
        pthread_cond_destroy(&cond);
    }

    // Core Functionality
    void push(const T& element) {
        pthread_mutex_lock(&mtx);
        if (data.size() >= capacity) {
            resize(capacity * 2); // Double capacity when full
        }
        data.push_back(element);
        pthread_cond_signal(&cond); // Signal waiting threads
        pthread_mutex_unlock(&mtx);
    }

    void pop() {
        pthread_mutex_lock(&mtx);
        while (data.empty()) {
            std::cerr << "Stack underflow: Cannot pop element, stack is empty.\n";
            pthread_cond_wait(&cond, &mtx); // Wait for push signal
        }
        data.pop_back();
        pthread_mutex_unlock(&mtx);
    }

    T top() const {
        pthread_mutex_lock(&mtx);
        if (data.empty()) {
            throw std::runtime_error("Stack is empty: No top element.");
        }
        T topElement = data.back();
        pthread_mutex_unlock(&mtx);
        return topElement;
    }

    bool isEmpty() const {
        pthread_mutex_lock(&mtx);
        bool empty = data.empty();
        pthread_mutex_unlock(&mtx);
        return empty;
    }

    bool isFull() const {
        pthread_mutex_lock(&mtx);
        bool full = data.size() >= capacity;
        pthread_mutex_unlock(&mtx);
        return full;
    }
};
