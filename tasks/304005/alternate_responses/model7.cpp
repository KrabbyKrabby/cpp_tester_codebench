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
    mutable pthread_cond_t cond; // Condition variable for waiting

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

    // Push an element onto the stack
    void push(const T& element) {
        pthread_mutex_lock(&mtx);
        if (data.size() >= capacity) {
            resize(capacity * 2); // Double the capacity if full
        }
        data.push_back(element);
        pthread_mutex_unlock(&mtx);
        pthread_cond_signal(&cond); // Notify waiting threads
    }

    // Pop the top element from the stack
    void pop() {
        pthread_mutex_lock(&mtx);
        while (data.empty()) {
            std::cerr << "Stack underflow: Cannot pop element, stack is empty.\n";
            pthread_cond_wait(&cond, &mtx); // Wait for new elements
        }
        data.pop_back();
        if (data.size() <= capacity / 4) {
            resize(capacity / 2); // Halve the capacity if quarter full
        }
        pthread_mutex_unlock(&mtx);
    }

    // Peek at the top element without removing it
    T top() const {
        pthread_mutex_lock(&mtx);
        if (data.empty()) {
            throw std::runtime_error("Stack is empty: No top element.");
        }
        T element = data.back();
        pthread_mutex_unlock(&mtx);
        return element;
    }

    // Check if the stack is empty
    bool isEmpty() const {
        pthread_mutex_lock(&mtx);
        bool empty = data.empty();
        pthread_mutex_unlock(&mtx);
        return empty;
    }

    // Check if the stack is full
    bool isFull() const {
        pthread_mutex_lock(&mtx);
        bool full = data.size() >= capacity;
        pthread_mutex_unlock(&mtx);
        return full;
    }

    // Resizing Logic

    // Resize the stack's capacity while maintaining thread safety
    void resize(size_t newCapacity) {
        // Create a new vector with the desired capacity
        std::vector<T> newData(newCapacity);
        // Copy elements from the old vector to the new vector
        std::copy(data.begin(), data.end(), newData.begin());
        // Swap the old vector with the new vector
        data.swap(newData);
        capacity = newCapacity;
    }
};
