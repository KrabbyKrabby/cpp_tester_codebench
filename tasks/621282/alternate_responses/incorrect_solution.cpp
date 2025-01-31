#include <stack>
#include <string>

int calculateScore(const std::string& s) {
    std::stack<int> scores;
    int currentScore = 0;

    for (char c : s) {
        if (c == '(') {
            scores.push(currentScore);
            currentScore = 0;
        } else if (c == ')') {
            if (scores.empty()) {
                return 0; // Unbalanced string
            }
            int prevScore = scores.top();
            scores.pop();
            currentScore = 2 * currentScore + prevScore;
        }
    }

    if (!scores.empty()) {
        return 0; // Unbalanced string
    }

    return currentScore;
}
