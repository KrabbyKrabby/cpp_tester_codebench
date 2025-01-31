#include <string>
#include <stack>
#include <vector>

using namespace std;

int calculateScore(const string& s) {
    // Check constraints and empty string
    if (s.empty()) return 0;
    if (s.length() < 2 || s.length() > 50) return 0;
    
    stack<int> scores;
    
    for (char c : s) {
        if (c == '(') {
            scores.push(0);  // Push 0 for opening bracket
        } else if (c == ')') {
            if (scores.empty()) return 0;  // Unbalanced string
            
            int currentScore = scores.top();
            scores.pop();
            
            // Calculate score based on rules
            int score = (currentScore == 0) ? 1 : (2 * currentScore);
            
            if (scores.empty()) {
                scores.push(score);
            } else {
                // Add to previous score (concatenation rule)
                int prevScore = scores.top();
                scores.pop();
                scores.push(prevScore + score);
            }
        } else {
            return 0;  // Invalid character
        }
    }
    
    // Check if string is properly balanced
    if (scores.size() != 1) return 0;
    
    return scores.top();
}