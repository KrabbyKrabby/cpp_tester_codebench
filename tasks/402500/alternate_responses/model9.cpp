#include <bits/stdc++.h>
#include <algorithm>
#include <string>

string decipherMessage(string str){
    string word = "";
    string result = "";

    for (int i = 0; i < str.length(); i++) {
        if (str[i] == ' ') {
            sort(word.begin(), word.end());
            result += word + " ";
            word = "";
        } else {
            if (word.find(str[i]) == string::npos) {
                word += str[i];
            }
        }
    }

    // handle the last word
    sort(word.begin(), word.end());
    result += word;

    return result;
}