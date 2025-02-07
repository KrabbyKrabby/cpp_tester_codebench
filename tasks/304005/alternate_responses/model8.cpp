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
    mutable pthread_cond_t cond; // Condition variable for synchronization

public:
    // Constructor with specified capacity
    Stack(size_t maxCapacity) : capacity(maxCapacity), data(maxCapacity) {
        pthread_mutex_init(&mtx, NULL);
        pthread_cond_init(&cond, NULL);
    }

    // Destructor to clean up synchronization primitives
    ~Stack() {
        pthread_mutex_destroy(&mtx);
        pthread_cond_destroy(&cond);
    }

    // Push an element onto the stack
    void push(const T& element) {
        pthread_mutex_lock(&mtx);
        while (isFull()) {
            pthread_cond_wait(&cond, &mtx);
        }
        if (data.size() >= capacity) {
            resize(capacity * 2); // Double the capacity if full
        }
        data.push_back(element);
        pthread_mutex_unlock(&mtx);
        pthread_cond_signal(&cond);
    }

    // Pop the top element from the stack
    void pop() {
        pthread_mutex_lock(&mtx);
        while (isEmpty()) {
            pthread_cond_wait(&cond, &mtx);
        }
        if (data.empty()) {
            std::cerr << "Stack underflow: Cannot pop element, stack is empty.\n";
            pthread_mutex_unlock(&mtx);
            return;
        }
        data.pop_back();
        if (data.size() <= capacity / 4) {
            resize(capacity / 2); // Halve the capacity if quarter full
        }
        pthread_mutex_unlock(&mtx);
        pthread_cond_signal(&cond);
    }

    // Peek at the top element without removing it
    T top() const {
        pthread_mutex_lock(&mtx);
        if (data.empty()) {
            pthread_mutex_unlock(&mtx);
            throw std::runtime_error("Stack is empty: No top element.");
        }
        T topElement = data.back();
        pthread_mutex_unlock(&mtx);
        return topElement;
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
        bool full = (data.size() >= capacity);
        pthread_mutex_unlock(&mtx);
        return full;
    }

private:
    // Resizing logic (assumes mutex is already locked)
    void resize(size_t newCapacity) {
        capacity = newCapacity;
        data.reserve(capacity);
    }
};
