#include <string>
#include <algorithm>
#include <unordered_set>

std::string decipherMessage(std::string str) {
    std::string result;
    std::string word;
    for (char c : str) {
        if (c == ' ') {
            // Sort the word and remove duplicates
            std::sort(word.begin(), word.end());
            std::unordered_set<char> unique_chars(word.begin(), word.end());
            word.assign(unique_chars.begin(), unique_chars.end());
            result += word + " ";
            word.clear();
        } else {
            word += c;
        }
    }
    // Handle the last word
    if (!word.empty()) {
        std::sort(word.begin(), word.end());
        std::unordered_set<char> unique_chars(word.begin(), word.end());
        word.assign(unique_chars.begin(), unique_chars.end());
        result += word;
    }
    return result;
}