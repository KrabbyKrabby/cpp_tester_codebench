#include <iostream>
#include <vector>
#include <pthread.h>
#include <stdexcept>
#include <memory>

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
        try {
            data.reserve(newCapacity);
            capacity = newCapacity;
        } catch (const std::bad_alloc& e) {
            throw std::runtime_error("Failed to resize stack: Memory allocation failed");
        }
    }

    // RAII Mutex Guard
    class MutexGuard {
    private:
        pthread_mutex_t& mutex;
    public:
        explicit MutexGuard(pthread_mutex_t& m) : mutex(m) {
            pthread_mutex_lock(&mutex);
        }
        ~MutexGuard() {
            pthread_mutex_unlock(&mutex);
        }
    };

public:
    // Constructor
    explicit Stack(size_t maxCapacity = 100) : capacity(maxCapacity) {
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
        MutexGuard guard(mutex);

        while (isFull()) {
            // Automatically increase capacity if stack is full
            try {
                resize(capacity * 2);
            } catch (const std::runtime_error& e) {
                std::cerr << "Stack overflow: " << e.what() << std::endl;
                pthread_cond_wait(&not_full, &mutex);
                continue;
            }
        }

        data.push_back(element);
        pthread_cond_signal(&not_empty);
    }

    void pop() {
        MutexGuard guard(mutex);

        while (isEmpty()) {
            std::cerr << "Stack underflow: Cannot pop element, stack is empty.\n";
            pthread_cond_wait(&not_empty, &mutex);
        }

        data.pop_back();
        pthread_cond_signal(&not_full);

        // Optionally shrink capacity if utilization is low
        if (data.size() < capacity / 4 && capacity > 100) {
            try {
                resize(capacity / 2);
            } catch (const std::runtime_error&) {
                // Ignore resize failure during shrink
            }
        }
    }

    T top() const {
        MutexGuard guard(mutex);
        
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty: No top element.");
        }
        return data.back();
    }

    // State Checking Methods
    bool isEmpty() const {
        MutexGuard guard(mutex);
        return data.empty();
    }

    bool isFull() const {
        MutexGuard guard(mutex);
        return data.size() >= capacity;
    }

    // Utility Methods
    size_t getCapacity() const {
        MutexGuard guard(mutex);
        return capacity;
    }

    size_t size() const {
        MutexGuard guard(mutex);
        return data.size();
    }
};
