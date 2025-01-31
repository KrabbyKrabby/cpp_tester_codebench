#include <string>
#include <vector>
#include <unordered_map>
#include <cctype>
#include <algorithm>
using namespace std;

int longestUniqueSubstr(const string& s, unordered_map<char, int>& weights) {
    // Handle empty string
    if (s.empty()) return 0;
    
    // Initialize variables
    int maxWeightedLength = 0;
    int currentWeightedLength = 0;
    vector<bool> visited(256, false);
    unordered_map<int, int> windowWeights;
    
    // Sliding window pointers
    int left = 0, right = 0;
    
    while (right < s.length()) {
        char currentChar = s[right];
        
        // Skip non-alphanumeric characters
        if (!isalnum(currentChar)) {
            right++;
            continue;
        }
        
        // If character is already in window, shrink window from left
        while (visited[currentChar]) {
            char leftChar = s[left];
            if (isalnum(leftChar)) {
                visited[leftChar] = false;
                // Subtract weight of removed character
                int weightToRemove = weights.count(leftChar) ? weights[leftChar] : 1;
                currentWeightedLength -= weightToRemove;
            }
            left++;
        }
        
        // Add current character to window
        visited[currentChar] = true;
        // Add weight of current character
        int charWeight = weights.count(currentChar) ? weights[currentChar] : 1;
        currentWeightedLength += charWeight;
        
        // Update maximum weighted length
        maxWeightedLength = max(maxWeightedLength, currentWeightedLength);
        right++;
    }
    
    return maxWeightedLength;
}