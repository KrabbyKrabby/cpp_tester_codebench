#include <iostream>
#include <string>
#include <set>
#include <algorithm>

std::string decipherMessage(std::string str) {
    std::string result;
    std::string word;

    for (char c : str) {
        if (c == ' ') {
            std::set<char> s(word.begin(), word.end());
            std::string sorted_word(s.begin(), s.end());
            std::sort(sorted_word.begin(), sorted_word.end());
            result += sorted_word + " ";
            word.clear();
        } else {
            word += c;
        }
    }

    // Process the last word
    if (!word.empty()) {
        std::set<char> s(word.begin(), word.end());
        std::string sorted_word(s.begin(), s.end());
        std::sort(sorted_word.begin(), sorted_word.end());
        result += sorted_word;
    }

    return result;
}