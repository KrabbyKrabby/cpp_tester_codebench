#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

int longestUniqueSubstr(const string& s, unordered_map<char, int>& weights) {
    // Create a visited array to keep track of characters in the current window
    vector<bool> visited(256, false);

    // Initialize the maximum weight and the current weight
    int maxWeight = 0, currWeight = 0;

    // Initialize the left and right pointers of the sliding window
    int left = 0, right = 0;

    // Iterate over the string
    while (right < s.length()) {
        // If the character is alphanumeric
        if (isalnum(s[right])) {
            // If the character is already in the window, move the left pointer to the right
            while (visited[s[right]]) {
                currWeight -= weights.count(s[left]) ? weights[s[left]] : 1;
                visited[s[left]] = false;
                left++;
            }

            // Mark the character as visited and update the current weight
            visited[s[right]] = true;
            currWeight += weights.count(s[right]) ? weights[s[right]] : 1;

            // Update the maximum weight
            maxWeight = max(maxWeight, currWeight);
        }
        right++;
    }
    return maxWeight;
}