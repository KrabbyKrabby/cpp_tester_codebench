#include <string>
#include <vector>
#include <unordered_map>
#include <cctype>
using namespace std;

int longestUniqueSubstr(const string& s, unordered_map<char, int>& weights) {
    if (s.empty()) return 0;
    if (s.length() == 1) {
        return weights.count(s[0]) ? weights[s[0]] : 1;
    }

    vector<bool> visited(256, false);
    int maxWeight = 0;
    int currentWeight = 0;
    int left = 0, right = 0;

    while (right < s.length()) {
        char currentChar = s[right];

        // Skip non-alphanumeric characters
        if (!isalnum(currentChar)) {
            right++;
            continue;
        }

        // If character is already in current window
        while (visited[currentChar]) {
            char leftChar = s[left];
            if (isalnum(leftChar)) {
                visited[leftChar] = false;
                // Subtract weight of removed character
                currentWeight -= weights.count(leftChar) ? weights[leftChar] : 1;
            }
            left++;
        }

        // Add current character
        visited[currentChar] = true;
        // Add weight of current character
        currentWeight += weights.count(currentChar) ? weights[currentChar] : 1;
        maxWeight = max(maxWeight, currentWeight);
        right++;
    }

    return maxWeight;
}
