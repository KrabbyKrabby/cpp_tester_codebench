#include <iostream>
#include <algorithm>
#include <string>
#include <unordered_set>
using namespace std;

string decipherMessage(string str) {
    string word = "";
    string result = "";
    unordered_set<char> s;
    
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == ' ') {
            if (word != "") {
                sort(word.begin(), word.end());
                result += word + " ";
                word = "";
                s.clear();
            }
        } else {
            if (s.find(str[i]) == s.end()) {
                word += str[i];
                s.insert(str[i]);
            }
        }
    }
    
    if (word != "") {
        sort(word.begin(), word.end());
        result += word;
    }
    return result;
}

