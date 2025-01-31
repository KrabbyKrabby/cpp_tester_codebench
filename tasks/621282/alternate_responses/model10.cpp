#include <stack>
#include <string>
#include <iostream>

using namespace std;

int calculateScore(const string& s) {
    stack<int> st;
    int score = 0;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '(') {
            st.push(0);
        } else if (s[i] == ')') {
            if (st.empty()) return 0; // string is not balanced
            int lastScore = st.top();
            st.pop();
            score += 2 * lastScore;
            if (!st.empty()) {
                st.top() += score;
            }
            score = 0;
        }
    }
    if (!st.empty()) return 0; // string is not balanced
    return score;
}

int main() {
    string s = "(()(()))";
    cout << calculateScore(s) << endl;
    return 0;
}