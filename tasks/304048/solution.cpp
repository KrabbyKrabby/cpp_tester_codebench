#include <vector>
#include <iostream>
#include <stdexcept>
#include <algorithm>

class IntStack
{
private:
    std::vector<int> stackData;
    int maxSize;

public:
    explicit IntStack(int capacity) : maxSize(capacity)
    {
        if (capacity <= 0)
        {
            throw std::runtime_error("Stack capacity must be greater than 0");
        }
        stackData.reserve(capacity);
    }

    void push(int value)
    {
        if (stackData.size() >= maxSize)
        {
            throw std::runtime_error("Stack overflow");
        }
        stackData.push_back(value);
    }

    void pop()
    {
        if (stackData.empty())
        {
            throw std::runtime_error("Stack underflow");
        }
        stackData.pop_back();
    }

    int top() const
    {
        if (stackData.empty())
        {
            throw std::runtime_error("Stack is empty");
        }
        return stackData.back();
    }

    size_t size() const
    {
        return stackData.size();
    }

    bool isEmpty() const
    {
        return stackData.empty();
    }

    bool isFull() const
    {
        return stackData.size() >= maxSize;
    }

    void display() const
    {
        std::cout << "Stack: ";
        for (auto it = stackData.rbegin(); it != stackData.rend(); ++it)
        {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }

    std::vector<int> monotonicIncreasing() const
    {
        std::vector<int> result;

        // Process elements in original insertion order
        for (size_t i = 0; i < stackData.size(); ++i)
        {
            // While result is not empty and current element is less than the last element
            // pop elements that would break monotonic increasing property
            while (!result.empty() && result.back() > stackData[i])
            {
                result.pop_back();
            }
            result.push_back(stackData[i]);
        }

        return result;
    }
};