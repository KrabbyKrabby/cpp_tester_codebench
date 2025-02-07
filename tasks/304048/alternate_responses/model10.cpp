#include <iostream>
#include <vector>
#include <stdexcept>

class IntStack {
private:
    std::vector<int> stackData;
    size_t maxSize;

public:
    IntStack(size_t maxSize) : maxSize(maxSize) {
        if (maxSize == 0) {
            throw std::runtime_error("Stack size cannot be zero");
        }
    }

    void push(int value) {
        if (stackData.size() == maxSize) {
            throw std::runtime_error("Stack is full. Cannot push more elements.");
        }
        stackData.push_back(value);
    }

    void pop() {
        if (stackData.empty()) {
            throw std::runtime_error("Stack is empty. Cannot pop more elements.");
        }
        stackData.pop_back();
    }

    int top() const {
        if (stackData.empty()) {
            throw std::runtime_error("Stack is empty. No top element.");
        }
        return stackData.back();
    }

    size_t size() const {
        return stackData.size();
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
        for (const auto& element : stackData) {
            while (!result.empty() && result.back() <= element) {
                result.pop_back();
            }
            result.push_back(element);
        }
        return result;
    }
};
