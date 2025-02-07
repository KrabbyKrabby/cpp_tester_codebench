class IntStack {
private:
    std::vector<int> stackData;

public:
    void push(int value) {
        stackData.push_back(value);
    }
    void pop() {
        stackData.pop_back();
    }
    int top() const {
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
};