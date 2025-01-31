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
        return weights[s[0]];

    // if string length is more than 1
    int maxLength = 0;
    vector<bool>visited(256, false);

    // left and right pointer of sliding window
    int left = 0, right = 0;
    int currentWindowWeight = 0;
    while (right < s.length()) {
        // skip non-alphanumeric characters
        if(!isalnum(s[right])) {
            right++;
            continue;
        }

        // If the character is repeated, move left pointer 
        // to the right and mark visited characters as false 
        // until the repeating character is no longer part
        // of the current window.
        while (visited[s[right]] == true) {
            currentWindowWeight -= weights[s[left]];
            visited[s[left]] = false;
            left++;
        }

        // add weight of new character
        currentWindowWeight += weights[s[right]];
        visited[s[right]] = true;

        // The weight of the current window is calculated 
        // and answer is updated accordingly.
        maxLength = max(maxLength, currentWindowWeight);
        right++;
    }
    return maxLength;
}