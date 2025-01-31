#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

bool isValidDNAChar(char c) {
    c = toupper(c);
    return c == 'A' || c == 'C' || c == 'G' || c == 'T';
}

std::string SequenceAllignment(const std::string& dna1, const std::string& dna2) {
    std::string validDna1, validDna2;
    
    // Filter valid DNA characters and convert to uppercase
    for (char c : dna1) {
        if (isValidDNAChar(c)) {
            validDna1 += toupper(c);
        }
    }
    for (char c : dna2) {
        if (isValidDNAChar(c)) {
            validDna2 += toupper(c);
        }
    }

    int m = validDna1.length();
    int n = validDna2.length();
    
    // Create DP table
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    // Fill DP table
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (validDna1[i - 1] == validDna2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    // Backtrack to construct LCS
    std::string lcs;
    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (validDna1[i - 1] == validDna2[j - 1]) {
            lcs = validDna1[i - 1] + lcs;
            i--; j--;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }

    return lcs;
}