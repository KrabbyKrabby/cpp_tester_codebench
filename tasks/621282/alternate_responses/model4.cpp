#include <string>
#include <stack>
using namespace std;

int calculateScore(const string& s) {
    // Handle empty string case
    if (s.empty()) return 0;
    
    // If length is odd, string can't be balanced
    if (s.length() % 2 != 0) return 0;
    
    stack<int> scores;
    
    for (char c : s) {
        if (c == '(') {
            scores.push(0);  // Push 0 for opening bracket
        } 
        else if (c == ')') {
            if (scores.empty()) return 0;  // Unbalanced case
            
            int curr = scores.top();
            scores.pop();
            
            int score = (curr == 0) ? 1 : curr * 2;
            
            if (scores.empty()) {
                scores.push(score);
            } else {
                int prev = scores.top();
                scores.pop();
                scores.push(prev + score);
            }
        }
    }
    
    // If stack has more than one element or is empty, string is unbalanced
    if (scores.size() != 1) return 0;
    
    return scores.top();
}
