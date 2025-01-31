#include <string>
#include <stack>
#include <vector>

using namespace std;

int calculateScore(const string& s) {
    if (s.empty()) return 0;
    
    stack<int> scores;
    
    for (char c : s) {
        if (c == '(') {
            scores.push(0);  // Push 0 for opening bracket
        } else if (c == ')') {
            if (scores.empty()) return 0;  // Unbalanced string
            
            int currentScore = scores.top();
            scores.pop();
            
            // If currentScore is 0, it means empty pair ()
            int value = (currentScore == 0) ? 1 : (2 * currentScore);
            
            if (scores.empty()) {
                scores.push(value);
            } else {
                // Add to previous score (for concatenation)
                int prevScore = scores.top();
                scores.pop();
                scores.push(prevScore + value);
            }
        }
    }
    
    // Check if string was balanced and has final score
    return (scores.size() == 1) ? scores.top() : 0;
}
