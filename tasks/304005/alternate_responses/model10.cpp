#include <iostream>
#include <vector>
#include <pthread.h>
#include <stdexcept>

template <typename T>
class Stack {
private:
    std::vector<T> data; // Internal storage for stack elements
    size_t capacity;     // Maximum capacity of the stack
    mutable pthread_mutex_t mutex; // Mutex for synchronization
    mutable pthread_cond_t condVar; // Condition variable for synchronization

    // Resizing logic
    void resize(size_t newCapacity) {
        data.resize(newCapacity);
        capacity = newCapacity;
    }

public:
    // Constructor with specified capacity
    Stack(size_t maxCapacity) : capacity(maxCapacity), data(maxCapacity) {
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&condVar, NULL);
    }

    // Destructor
    ~Stack() {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&condVar);
    }

    // Push an element onto the stack
    void push(const T& element) {
        pthread_mutex_lock(&mutex);
        if (data.size() >= capacity) {
            // Increase capacity dynamically
            resize(capacity * 2);
        }
        data.push_back(element);
        pthread_cond_signal(&condVar);
        pthread_mutex_unlock(&mutex);
    }

    // Pop the top element from the stack
    void pop() {
        pthread_mutex_lock(&mutex);
        while (data.empty()) {
            std::cerr << "Stack underflow: Cannot pop element, stack is empty.\n";
            pthread_cond_wait(&condVar, &mutex);
        }
        data.pop_back();
        if (data.size() < capacity / 2) {
            // Decrease capacity dynamically
            resize(capacity / 2);
        }
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
        bool isEmpty = data.empty();
        pthread_mutex_unlock(&mutex);
        return isEmpty;
    }

    // Check if the stack is full
    bool isFull() const {
        pthread_mutex_lock(&mutex);
        bool isFull = data.size() >= capacity;
        pthread_mutex_unlock(&mutex);
        return isFull;
    }
};
