#include <string>
#include <vector>
#include <unordered_map>

// Function to find the maximum weight of the longest substring without repeating characters.
double longestUniqueSubstr(const string& s, unordered_map<char, int> weights) {
    // If the string is empty, return 0 as there are no characters.
    if (s.length() == 0)
        return 0;

    // If the string has only one character, return its weight.
    if (s.length() == 1)
        return weights[s[0]];

    // Variable to store the maximum weight of the substring.
    double maxLength = 0;
    // Vector to keep track of visited characters (ASCII values range from 0-255).
    vector<bool> visited(256, false);

    // Sliding window pointers to define the current window.
    int left = 0, right = 0;
    // Variable to keep track of the weight of the current window.
    double current_weight = 0;

    // Iterate through the string using the right pointer.
    while (right < s.length()) {
        // If the current character is not alphanumeric, skip it and move to the next.
        if (!iswalnum(s[right])) {
            right++;
            continue;
        }
        if (!iswalnum(s[left])) {
            left++;
            continue;
        }

        // If the current character is already visited, adjust the left pointer
        // to the next alphanumeric character to remove the repeated character 
        // from the window and update the weight.
        while (visited[s[right]]) {
            visited[s[left]] = false;               // Mark the character as not visited.
            current_weight -= weights[s[left]];     // Subtract its weight from the current weight.
            left++;                                 // Move the left pointer forward.
        }

        // Mark the current character as visited.
        visited[s[right]] = true;
        // Add the weight of the current character to the current window weight.
        current_weight += weights[s[right]];

        // Update the maximum weight of a valid substring.
        maxLength = max(maxLength, current_weight);
        right++; // Move the right pointer to expand the window.
    }

    // Return the maximum weight of the longest unique substring.
    return maxLength;
}