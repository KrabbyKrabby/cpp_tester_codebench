#include <iostream>
#include <vector>
#include <pthread.h>
#include <stdexcept>

template <typename T>
class Stack {
private:
    std::vector<T> data; // Internal storage for stack elements
    size_t capacity;     // Maximum capacity of the stack
    mutable pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex for synchronization
    mutable pthread_cond_t cond = PTHREAD_COND_INITIALIZER;    // Condition variable for synchronization

public:
    // Constructor with specified capacity
    Stack(size_t maxCapacity) : capacity(maxCapacity), data(maxCapacity) {}

    // Destructor to release synchronization resources
    ~Stack() {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond);
    }

    // Core Functionality

    // Push an element onto the stack
    void push(const T& element) {
        pthread_mutex_lock(&mutex);
        if (data.size() >= capacity) {
            resize(capacity * 2); // Double the capacity when stack is full
        }
        data.push_back(element);
        pthread_cond_signal(&cond); // Signal waiting threads that data is available
        pthread_mutex_unlock(&mutex);
    }

    // Pop the top element from the stack
    void pop() {
        pthread_mutex_lock(&mutex);
        while (data.empty()) {
            pthread_cond_wait(&cond, &mutex); // Wait until data is available
        }
        if (data.size() <= capacity / 4) {
            resize(capacity / 2); // Shrink capacity when stack is a quarter full
        }
        data.pop_back();
        pthread_mutex_unlock(&mutex);
    }

    // Peek at the top element without removing it
    T top() const {
        pthread_mutex_lock(&mutex);
        if (data.empty()) {
            throw std::runtime_error("Stack is empty: No top element.");
        }
        T topElement = data.back();
        pthread_mutex_unlock(&mutex);
        return topElement;
    }

    // Check if the stack is empty
    bool isEmpty() const {
        pthread_mutex_lock(&mutex);
        bool empty = data.empty();
        pthread_mutex_unlock(&mutex);
        return empty;
    }

    // Check if the stack is full
    bool isFull() const {
        pthread_mutex_lock(&mutex);
        bool full = data.size() >= capacity;
        pthread_mutex_unlock(&mutex);
        return full;
    }

    // Resizing Logic

    // Resize the stack's capacity while maintaining thread safety
    void resize(size_t newCapacity) {
        // Ensure new capacity is within reasonable bounds
        if (newCapacity < 10 || newCapacity > 10000) {
            return;
        }
        capacity = newCapacity;
        data.resize(capacity);
    }
};
