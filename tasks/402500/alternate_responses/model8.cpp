#include <iostream>
#include <string>
#include <set>
#include <algorithm>

std::string decipherMessage(std::string str) {
    std::string word;
    std::string result;
    std::set<char> unique_chars;

    for (char c : str) {
        if (c == ' ') {
            if (!word.empty()) {
                std::sort(word.begin(), word.end());
                result += word + ' ';
                word.clear();
                unique_chars.clear();
            }
        } else {
            if (unique_chars.find(c) == unique_chars.end()) {
                word += c;
                unique_chars.insert(c);
            }
        }
    }

    if (!word.empty()) {
        std::sort(word.begin(), word.end());
        result += word;
    }

    return result;
}