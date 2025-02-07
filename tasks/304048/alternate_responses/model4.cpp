#include <vector>
#include <iostream>
#include <stdexcept>

class IntStack {
private:
    std::vector<int> stackData;
    size_t maxSize;

public:
    IntStack(size_t maxStackSize) : maxSize(maxStackSize) {
        if (maxSize == 0) {
            throw std::runtime_error("Stack size must be greater than 0");
        }
        stackData.reserve(maxSize);
    }

    void push(int value) {
        if (stackData.size() >= maxSize) {
            throw std::runtime_error("Stack overflow");
        }
        stackData.push_back(value);
    }

    void pop() {
        if (stackData.empty()) {
            throw std::runtime_error("Stack underflow");
        }
        stackData.pop_back();
    }

    int top() const {
        if (stackData.empty()) {
            throw std::runtime_error("Stack is empty");
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
        std::vector<int> tempStack;

        for (size_t i = 0; i < stackData.size(); ++i) {
            int currentElement = stackData[i];
            
            // Remove elements from tempStack that are greater than current element
            while (!tempStack.empty() && tempStack.back() > currentElement) {
                tempStack.pop_back();
            }
            
            tempStack.push_back(currentElement);
        }

        return tempStack;
    }
};
