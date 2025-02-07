#include <vector>
#include <iostream>
#include <stdexcept>

class IntStack {
private:
    std::vector<int> stackData;
    size_t maxSize;

public:
    IntStack(size_t maxStackSize) : maxSize(maxStackSize) {
        if (maxStackSize == 0) {
            throw std::runtime_error("Stack size must be greater than 0");
        }
        stackData.reserve(maxSize);
    }

    void push(int value) {
        if (stackData.size() >= maxSize) {
            throw std::runtime_error("Stack overflow: Maximum size reached");
        }
        stackData.push_back(value);
    }

    void pop() {
        if (stackData.empty()) {
            throw std::runtime_error("Stack underflow: Cannot pop from empty stack");
        }
        stackData.pop_back();
    }

    int top() const {
        if (stackData.empty()) {
            throw std::runtime_error("Stack is empty: Cannot get top element");
        }
        return stackData.back();
    }

    size_t size() const {
        return stackData.size();
    }

    bool empty() const {
        return stackData.empty();
    }

    void display() const {
        std::cout << "Stack: ";
        for (auto it = stackData.rbegin(); it != stackData.rend(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }

    std::vector<int> monotonicIncreasing() const {
        std::vector<int> result;
        
        for (size_t i = 0; i < stackData.size(); ++i) {
            while (!result.empty() && result.back() > stackData[i]) {
                result.pop_back();
            }
            result.push_back(stackData[i]);
        }
        
        return result;
    }
};
