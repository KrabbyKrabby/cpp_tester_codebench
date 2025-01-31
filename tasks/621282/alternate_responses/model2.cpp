#include <string>
#include <stack>
using namespace std;

bool isBalanced(const string& s) {
    stack<char> st;
    for(char c : s) {
        if(c == '(') {
            st.push(c);
        } else if(!st.empty() && c == ')') {
            st.pop();
        } else {
            return false;
        }
    }
    return st.empty();
}

int calculateScore(const string& s) {
    // Check constraints
    if(s.length() > 50 || (s.length() > 0 && s.length() < 2)) return 0;
    
    // Check if string is balanced
    if(!isBalanced(s)) return 0;
    
    // Empty string case
    if(s.empty()) return 0;
    
    // Base case: simple parentheses
    if(s == "()") return 1;
    
    int n = s.length();
    
    // Find split point for concatenated strings
    int balance = 0;
    for(int i = 0; i < n; i++) {
        if(s[i] == '(') balance++;
        else balance--;
        
        // Found a concatenation point
        if(balance == 0 && i < n-1) {
            return calculateScore(s.substr(0, i+1)) + 
                   calculateScore(s.substr(i+1));
        }
    }
    
    // Handle nested case (A)
    if(s[0] == '(' && s[n-1] == ')') {
        return 2 * calculateScore(s.substr(1, n-2));
    }
    
    return 0;
}