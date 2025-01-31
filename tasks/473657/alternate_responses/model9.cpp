#include <iostream>
#include <string>
#include <vector>

// Function to find the Longest Common Subsequence (LCS)
std::string SequenceAllignment(const std::string& dna1, const std::string& dna2) {
    // Convert the sequences to uppercase
    std::string dna1Upper = dna1;
    std::string dna2Upper = dna2;
    for (char& c : dna1Upper) {
        c = std::toupper(c);
    }
    for (char& c : dna2Upper) {
        c = std::toupper(c);
    }

    // Filter out invalid characters
    std::string validChars = "ACGT";
    std::string dna1Filtered;
    std::string dna2Filtered;
    for (char c : dna1Upper) {
        if (validChars.find(c) != std::string::npos) {
            dna1Filtered += c;
        }
    }
    for (char c : dna2Upper) {
        if (validChars.find(c) != std::string::npos) {
            dna2Filtered += c;
        }
    }

    int m = dna1Filtered.length();
    int n = dna2Filtered.length();

    // Initialize a 2D vector to store the lengths of common subsequences
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    // Fill the dp table
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (dna1Filtered[i - 1] == dna2Filtered[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    // Reconstruct the longest common subsequence
    std::string lcs;
    int i = m;
    int j = n;
    while (i > 0 && j > 0) {
        if (dna1Filtered[i - 1] == dna2Filtered[j - 1]) {
            lcs = dna1Filtered[i - 1] + lcs;
            --i;
            --j;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            --i;
        } else {
            --j;
        }
    }

    return lcs;
}