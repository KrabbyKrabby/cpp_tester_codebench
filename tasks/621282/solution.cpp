#include <iostream>
#include <stack>
using namespace std;

int calculateScore(const std::string& s) {
    stack<int> stack;
    stack.push(0); // Initialize the stack with a score of 0
    
    for (char c : s) {
        if (c == '(') {
            stack.push(0); // Push a new score of 0 for the new group
        } else if (c == ')') {
            if (stack.size() == 1) {
                return 0; // Unbalanced string
            }
            int score = stack.top();
            stack.pop();
            stack.top() += std::max(2 * score, 1); // Update the score
        }
    }
    
    return stack.size() == 1 ? stack.top() : 0; // Return the final score if the string is balanced
}