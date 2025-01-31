#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

int longestUniqueSubstr(const std::string& s, std::unordered_map<char, int>& weights) {
    if (s.empty()) return 0;

    std::unordered_set<char> visited;
    int maxLength = 0;
    int weightSum = 0;
    int left = 0;

    for (int right = 0; right < s.size(); ++right) {
        char c = s[right];
        if (!std::isalnum(c)) continue;

        while (visited.find(c) != visited.end()) {
            char leftChar = s[left];
            if (weights.find(leftChar) != weights.end()) {
                weightSum -= weights[leftChar];
            } else {
                weightSum -= 1;
            }
            visited.erase(leftChar);
            left++;
        }

        visited.insert(c);
        if (weights.find(c) != weights.end()) {
            weightSum += weights[c];
        } else {
            weightSum += 1;
        }

        maxLength = std::max(maxLength, weightSum);
    }

    return maxLength;
}

int main() {
    std::string s = "abeioub";
    std::unordered_map<char, int> weights = {{'a', 2}, {'e', 1}, {'i', 1}, {'o', 1}, {'u', 1}, {'b', 1}};
    std::cout << longestUniqueSubstr(s, weights) << std::endl;  // Output: 7
    return 0;
}