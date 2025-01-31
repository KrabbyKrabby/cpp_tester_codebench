#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_set>

std::string decipherMessage(std::string str) {
    std::string word, result;
    for (char c : str) {
        if (c != ' ') {
            word += c;
        } else {
            std::sort(word.begin(), word.end());
            word.erase(std::unique(word.begin(), word.end()), word.end());
            result += word + " ";
            word.clear();
        }
    }

    // Process the last word
    if (!word.empty()) {
        std::sort(word.begin(), word.end());
        word.erase(std::unique(word.begin(), word.end()), word.end());
        result += word;
    }

    return result;
}