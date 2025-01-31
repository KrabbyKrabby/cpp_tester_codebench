#include <string>
#include <vector>
#include <unordered_map>
#include <cctype>
using namespace std;

int longestUniqueSubstr(const string& s, unordered_map<char, int>& weights) {
    // if string length is 0
    if (s.length() == 0)
        return 0;

    // if string length 1
    if (s.length() == 1) {
        auto it = weights.find(s[0]);
        return it != weights.end() ? it->second : 1;
    }

    // if string length is more than 1
    int maxLength = 0;
    unordered_map<char, int> visited;

    // left and right pointer of sliding window
    int left = 0, right = 0;
    int currentWeight = 0;
    while (right < s.length()) {
        if (!std::isalnum(s[right])) {
            right++;
            continue;
        }

        // If the character is repeated, move left pointer 
        // to the right and mark visited characters as false 
        // until the repeating character is no longer part
        // of the current window.
        while (visited.find(s[right]) != visited.end()) {
            currentWeight -= visited[s[left]];
            visited.erase(s[left]);
            left++;
        }

        // Get the weight of the current character
        auto it = weights.find(s[right]);
        int weight = it != weights.end() ? it->second : 1;
        visited[s[right]] = weight;
        currentWeight += weight;

        // The length of the current window (right - left + 1)
        // is calculated and answer is updated accordingly.
        maxLength = max(maxLength, currentWeight);
        right++;
    }
    return maxLength;
}
