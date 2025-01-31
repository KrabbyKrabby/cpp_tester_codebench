#include <iostream>
#include <string>
#include <stack>

int calculateScore(const std::string& s) {
    std::stack<int> stack;
    int score = 0;
    for (char c : s) {
        if (c == '(') {
            stack.push(score);
            score = 0;
        } else if (c == ')') {
            if (stack.empty()) {
                return 0;  // Unbalanced string
            }
            score = stack.top() + std::max(2 * score, 1);
            stack.pop();
        }
    }
    return stack.empty() ? score : 0;  // Return score only if string is balanced
}

int main() {
    std::string s = "(()(()))";
    std::cout << calculateScore(s) << std::endl;
    return 0;
}