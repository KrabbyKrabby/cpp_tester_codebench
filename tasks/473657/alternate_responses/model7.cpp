#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Function to find the Longest Common Subsequence (LCS)
std::string SequenceAllignment(const std::string& dna1, const std::string& dna2) {
    // Convert DNA sequences to uppercase and remove invalid characters
    std::string validChars = "ACGT";
    std::string dna1Cleaned, dna2Cleaned;
    for (char c : dna1) {
        if (validChars.find(toupper(c)) != std::string::npos) {
            dna1Cleaned += toupper(c);
        }
    }
    for (char c : dna2) {
        if (validChars.find(toupper(c)) != std::string::npos) {
            dna2Cleaned += toupper(c);
        }
    }

    int m = dna1Cleaned.length();
    int n = dna2Cleaned.length();
    
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (dna1Cleaned[i - 1] == dna2Cleaned[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    // Backtrack to construct the longest common subsequence
    std::string lcs;
    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (dna1Cleaned[i - 1] == dna2Cleaned[j - 1]) {
            lcs.push_back(dna1Cleaned[i - 1]);
            --i;
            --j;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            --i;
        } else {
            --j;
        }
    }

    // Reverse the LCS since we constructed it in reverse order
    std::reverse(lcs.begin(), lcs.end());

    return lcs;
}
