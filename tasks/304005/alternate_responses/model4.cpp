#include <iostream>
#include <vector>
#include <pthread.h>
#include <stdexcept>
#include <cmath>

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
    
    // Resizing constants
    static constexpr double GROWTH_FACTOR = 1.5;
    static constexpr double SHRINK_FACTOR = 0.25;
    
    // Resize helper function (called with mutex already locked)
    void resize(size_t newCapacity) {
        if (newCapacity < data.size()) return;  // Never shrink below current size
        
        try {
            data.reserve(newCapacity);
            capacity = newCapacity;
        } catch (const std::bad_alloc& e) {
            // Handle allocation failure
            std::cerr << "Memory allocation failed during resize: " << e.what() << std::endl;
        }
    }

public:
    // Constructor
    explicit Stack(size_t maxCapacity) : capacity(maxCapacity) {
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
    
    // Push operation
    void push(const T& element) {
        pthread_mutex_lock(&mutex);
        
        try {
            // Check if resize is needed
            if (data.size() >= capacity) {
                size_t newCapacity = static_cast<size_t>(capacity * GROWTH_FACTOR);
                resize(newCapacity);
            }
            
            if (data.size() < capacity) {
                data.push_back(element);
                pthread_cond_signal(&not_empty);
            } else {
                std::cerr << "Stack overflow: Cannot push element, stack is full.\n";
            }
        } catch (...) {
            pthread_mutex_unlock(&mutex);
            throw;
        }
        
        pthread_mutex_unlock(&mutex);
    }
    
    // Pop operation
    void pop() {
        pthread_mutex_lock(&mutex);
        
        try {
            if (data.empty()) {
                std::cerr << "Stack underflow: Cannot pop element, stack is empty.\n";
            } else {
                data.pop_back();
                pthread_cond_signal(&not_full);
                
                // Check if shrinking is needed
                if (data.size() < capacity * SHRINK_FACTOR) {
                    size_t newCapacity = std::max(
                        static_cast<size_t>(capacity * SHRINK_FACTOR),
                        static_cast<size_t>(1)
                    );
                    resize(newCapacity);
                }
            }
        } catch (...) {
            pthread_mutex_unlock(&mutex);
            throw;
        }
        
        pthread_mutex_unlock(&mutex);
    }
    
    // Top operation
    T top() const {
        pthread_mutex_lock(&mutex);
        
        try {
            if (data.empty()) {
                pthread_mutex_unlock(&mutex);
                throw std::runtime_error("Stack is empty: No top element.");
            }
            T result = data.back();
            pthread_mutex_unlock(&mutex);
            return result;
        } catch (...) {
            pthread_mutex_unlock(&mutex);
            throw;
        }
    }
    
    // Status check operations
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
    
    // Get current size (thread-safe)
    size_t size() const {
        pthread_mutex_lock(&mutex);
        size_t currentSize = data.size();
        pthread_mutex_unlock(&mutex);
        return currentSize;
    }
    
    // Get current capacity (thread-safe)
    size_t getCurrentCapacity() const {
        pthread_mutex_lock(&mutex);
        size_t currentCapacity = capacity;
        pthread_mutex_unlock(&mutex);
        return currentCapacity;
    }
};
