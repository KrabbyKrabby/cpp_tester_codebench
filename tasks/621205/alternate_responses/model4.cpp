#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

int longestUniqueSubstr(const string& s, unordered_map<char, int>& weights) {
    if (s.empty()) return 0;
    if (s.length() == 1) {
        return weights.count(s[0]) ? weights[s[0]] : 1;
    }

    int maxWeight = 0;
    vector<bool> visited(256, false);
    int left = 0, right = 0;
    int currentWeight = 0;

    while (right < s.length()) {
        // If character is already in window, shrink window from left
        while (visited[s[right]]) {
            char leftChar = s[left];
            visited[leftChar] = false;
            // Subtract weight of removed character
            currentWeight -= weights.count(leftChar) ? weights[leftChar] : 1;
            left++;
        }

        // Add new character to window
        visited[s[right]] = true;
        // Add weight of new character
        currentWeight += weights.count(s[right]) ? weights[s[right]] : 1;
        
        // Update maximum weight if current window weight is larger
        maxWeight = max(maxWeight, currentWeight);
        right++;
    }

    return maxWeight;
}
