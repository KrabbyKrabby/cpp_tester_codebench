#include <iostream>
#include <string>
#include <stack>

int calculateScore(const std::string& s) {
    std::stack<int> stack;
    int score = 0;
    int multiplier = 1;

    for (char c : s) {
        if (c == '(') {
            stack.push(multiplier);
            multiplier *= 2;
        } else if (c == ')') {
            if (stack.empty()) {
                return 0; // string is not balanced
            }
            multiplier = stack.top();
            stack.pop();
            score += multiplier;
        }
    }

    if (!stack.empty()) {
        return 0; // string is not balanced
    }

    return score;
}

int main() {
    std::string s = "(()(()))";
    std::cout << calculateScore(s) << std::endl; // output: 6
    return 0;
}