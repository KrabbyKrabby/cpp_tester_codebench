#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

int longestUniqueSubstr(const string& s, unordered_map<char, int>& weights) {
    // if string length is 0
    if (s.length() == 0)
        return 0;

    // if string length 1
    if (s.length() == 1)
        return weights.count(s[0]) ? weights[s[0]] : 1;

    // if string length is more than 1
    int maxLength = 0;
    vector<bool> visited(256, false);

    // left and right pointer of sliding window
    int left = 0, right = 0;
    int currentWeight = 0;
    while (right < s.length()) {
        // ignore non-alphanumeric characters
        if (!isalnum(s[right])) {
            right++;
            continue;
        }

        // If the character is repeated, move left pointer 
        // to the right and mark visited characters as false 
        // until the repeating character is no longer part
        // of the current window.
        while (visited[s[right]] == true) {
            currentWeight -= weights.count(s[left]) ? weights[s[left]] : 1;
            visited[s[left]] = false;
            left++;
        }

        visited[s[right]] = true;
        currentWeight += weights.count(s[right]) ? weights[s[right]] : 1;

        // The length of the current window (right - left + 1)
        // is calculated and answer is updated accordingly.
        maxLength = max(maxLength, currentWeight);
        right++;
    }
    return maxLength;
}
