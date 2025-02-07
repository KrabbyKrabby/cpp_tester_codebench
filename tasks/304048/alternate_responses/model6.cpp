#include <iostream>
#include <vector>
#include <stdexcept>

class IntStack {
private:
    std::vector<int> stackData;
    size_t maxSize;

public:
    IntStack(size_t max_size) : maxSize(max_size) {
        if (max_size == 0) {
            throw std::runtime_error("Stack size must be greater than 0.");
        }
    }

    void push(int value) {
        if (size() == maxSize) {
            throw std::runtime_error("Stack overflow: cannot push beyond maximum size.");
        }
        stackData.push_back(value);
    }

    void pop() {
        if (size() == 0) {
            throw std::runtime_error("Stack underflow: cannot pop from an empty stack.");
        }
        stackData.pop_back();
    }

    int top() const {
        if (size() == 0) {
            throw std::runtime_error("Cannot access top of an empty stack.");
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
        for (int value : stackData) {
            while (!result.empty() && value <= result.back()) {
                result.pop_back();
            }
            result.push_back(value);
        }
        return result;
    }
};
