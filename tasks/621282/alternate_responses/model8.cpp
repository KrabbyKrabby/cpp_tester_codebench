#include <stack>
#include <string>

int calculateScore(const std::string& s) {
    std::stack<int> st;
    int score = 0;
    
    for (char c : s) {
        if (c == '(') {
            st.push(score);
            score = 0;
        } else if (c == ')') {
            if (st.empty()) return 0; // unbalanced string
            score = st.top() + std::max(2 * score, 1);
            st.pop();
        }
    }
    
    if (!st.empty()) return 0; // unbalanced string
    return score;
}