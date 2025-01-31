#include <stack>
#include <string>

int calculateScore(const std::string& s) {
    std::stack<int> stack;
    int score = 0;
    for (char c : s) {
        if (c == '(') {
            stack.push(score);
            score = 0;
        } else if (c == ')') {
            if (stack.empty()) {
                return 0; // unbalanced string
            }
            score = stack.top() + std::max(2 * score, 1);
            stack.pop();
        }
    }
    return stack.empty() ? score : 0; // return 0 if string is not balanced
}

// Test the function
#include <iostream>

int main() {
    std::string input = "(()(()))";
    std::cout << "Score: " << calculateScore(input) << std::endl;
    return 0;
}