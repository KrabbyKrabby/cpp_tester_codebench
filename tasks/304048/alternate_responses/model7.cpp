#include <vector>
#include <iostream>
#include <stdexcept>

class IntStack {
private:
    std::vector<int> stackData;
    size_t maxSize;

public:
    IntStack(size_t maxSize) : maxSize(maxSize) {}

    void push(int value) {
        if (stackData.size() >= maxSize) {
            throw std::runtime_error("Stack is full");
        }
        stackData.push_back(value);
    }

    void pop() {
        if (stackData.empty()) {
            throw std::runtime_error("Stack is empty");
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

    void display() const {
        std::cout << "Stack: ";
        for (auto it = stackData.rbegin(); it != stackData.rend(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }

    std::vector<int> monotonicIncreasing() {
        std::vector<int> result;
        for (int value : stackData) {
            while (!result.empty() && value <= result.back()) {
                result.pop_back();
            }
            result.push_back(value);
        }
        return result;
    }
};
