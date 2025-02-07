#include <iostream>
#include <vector>
#include <pthread.h>
#include <stdexcept>
#include <cassert>

// Generic Stack class template
template <typename T>
class Stack {
private:
    std::vector<T> data;       // Internal storage for stack elements
    size_t capacity;           // Maximum capacity of the stack
    mutable pthread_mutex_t mtx; // Mutex for thread-safety (must be mutable to use in const methods)

public:
    // Constructor with specified capacity
    Stack(size_t maxCapacity) : capacity(maxCapacity) {
        pthread_mutex_init(&mtx, NULL);
    }

    // Destructor to clean up mutex
    ~Stack() {
        pthread_mutex_destroy(&mtx);
    }

    // Push an element onto the stack
    void push(const T& element) {
        pthread_mutex_lock(&mtx);  // Lock for thread-safety
        if (data.size() >= capacity) {
            // Dynamic resizing: double capacity if full
            capacity *= 2;
            std::cout << "Stack resized to capacity: " << capacity << std::endl;
        }
        data.push_back(element);
        pthread_mutex_unlock(&mtx);  // Unlock after operation
    }

    // Pop the top element from the stack
    void pop() {
        pthread_mutex_lock(&mtx);
        if (data.empty()) {
            std::cerr << "Stack underflow: Cannot pop element, stack is empty.\n";
            pthread_mutex_unlock(&mtx);
            return;
        }
        data.pop_back();
        pthread_mutex_unlock(&mtx);
    }

    // Peek at the top element without removing it (const method)
    T top() const {
        pthread_mutex_lock(&mtx);
        if (data.empty()) {
            pthread_mutex_unlock(&mtx);
            throw std::runtime_error("Stack is empty: No top element.");
        }
        T result = data.back();
        pthread_mutex_unlock(&mtx);
        return result;
    }

    // Check if the stack is empty (const method)
    bool isEmpty() const {
        pthread_mutex_lock(&mtx);
        bool result = data.empty();
        pthread_mutex_unlock(&mtx);
        return result;
    }

    // Check if the stack is full (const method)
    bool isFull() const {
        pthread_mutex_lock(&mtx);
        bool result = (data.size() >= capacity);
        pthread_mutex_unlock(&mtx);
        return result;
    }
};