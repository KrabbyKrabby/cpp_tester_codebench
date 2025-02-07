#include <iostream>
#include <vector>

template <typename T>
class Stack {
private:
    std::vector<T> data; // Internal storage for stack elements
    size_t capacity;     // Maximum capacity of the stack

public:
    // Constructor with specified capacity
    Stack(size_t maxCapacity) : capacity(maxCapacity) {}

    // Push an element onto the stack
    void push(const T& element) {
        if (data.size() >= capacity) {
            std::cerr << "Stack overflow: Cannot push element, stack is full.\n";
            return;
        }
        // TODO: Add the element to the stack
    }

    // Pop the top element from the stack
    void pop() {
        if (data.empty()) {
            std::cerr << "Stack underflow: Cannot pop element, stack is empty.\n";
            return;
        }
        // TODO: Remove the top element from the stack
    }

    // Peek at the top element without removing it
    T top() const {
        if (data.empty()) {
            throw std::runtime_error("Stack is empty: No top element.");
        }
        // TODO: Return the top element
    }

    // Check if the stack is empty
    bool isEmpty() const {
        // TODO: Return whether the stack is empty
    }

    // Check if the stack is full
    bool isFull() const {
        return data.size() >= capacity;
    }
};