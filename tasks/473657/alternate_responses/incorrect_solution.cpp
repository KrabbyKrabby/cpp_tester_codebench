#include <iostream>
#include <string>
#include <vector>

// Function to find the Longest Common Subsequence (LCS)
std::string SequenceAllignment(const std::string& dna1, const std::string& dna2) {
    // Convert to uppercase and remove invalid characters
    std::string cleanDna1, cleanDna2;
    for (char c : dna1) {
        if (c == 'A' || c == 'a' || c == 'C' || c == 'c' || c == 'G' || c == 'g' || c == 'T' || c == 't') {
            cleanDna1 += toupper(c);
        }
    }
    for (char c : dna2) {
        if (c == 'A' || c == 'a' || c == 'C' || c == 'c' || c == 'G' || c == 'g' || c == 'T' || c == 't') {
            cleanDna2 += toupper(c);
        }
    }

    int m = cleanDna1.length();
    int n = cleanDna2.length();

    // Initialize dp table
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    // Fill dp table
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (cleanDna1[i - 1] == cleanDna2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    // Backtrack to find the LCS
    int i = m, j = n;
    std::string lcs;
    while (i > 0 && j > 0) {
        if (cleanDna1[i - 1] == cleanDna2[j - 1]) {
            lcs = cleanDna1[i - 1] + lcs;
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
