#include <string>
#include <stack>

using namespace std;

int calculateScore(const string& s) {
    // Handle empty string or odd length string
    if (s.empty() || s.length() % 2 != 0 || s.length() > 50 || s.length() < 2) {
        return 0;
    }
    
    stack<int> scores;
    
    for (char c : s) {
        if (c == '(') {
            scores.push(0);  // Push 0 for opening bracket
        } 
        else if (c == ')') {
            if (scores.empty()) {
                return 0;  // Unbalanced string
            }
            
            int currentScore = scores.top();
            scores.pop();
            
            // Calculate score based on rules
            int finalScore = (currentScore == 0) ? 1 : (currentScore * 2);
            
            if (scores.empty()) {
                scores.push(finalScore);
            } else {
                // Add to previous score (concatenation rule)
                int prevScore = scores.top();
                scores.pop();
                scores.push(prevScore + finalScore);
            }
        }
        else {
            return 0;  // Invalid character
        }
    }
    
    // Check if we have exactly one final score
    if (scores.size() != 1) {
        return 0;
    }
    
    return scores.top();
}