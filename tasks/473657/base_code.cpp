#include <iostream>
#include <string>
#include <vector>

// Function to find the Longest Common Substring (LCS)
std::string SequenceAllignment(const std::string& dna1, const std::string& dna2) {
    int m = dna1.length();
    int n = dna2.length();
    
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
    int maxLength = 0;  
    int endIndex = 0;  

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (dna1[i - 1] == dna2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                if (dp[i][j] > maxLength) {
                    maxLength = dp[i][j];
                    endIndex = i;                 }
            }
        }
    }

    // Extract the longest common subsequence
    return dna1.substr(endIndex - maxLength, maxLength);
}