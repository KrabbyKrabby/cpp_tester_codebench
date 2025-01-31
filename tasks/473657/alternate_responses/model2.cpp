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
    // Filter valid DNA characters and convert to uppercase
    std::string filtered1, filtered2;
    std::vector<int> positions1;
    
    for (size_t i = 0; i < dna1.length(); ++i) {
        if (isValidDNAChar(dna1[i])) {
            filtered1 += toupper(dna1[i]);
            positions1.push_back(i);
        }
    }
    
    for (const char& c : dna2) {
        if (isValidDNAChar(c)) {
            filtered2 += toupper(c);
        }
    }

    int m = filtered1.length();
    int n = filtered2.length();
    
    // Create DP table with an extra row and column for empty string case
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
    
    // Fill the DP table
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (filtered1[i - 1] == filtered2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    
    // Backtrack to find the LCS
    std::string lcs;
    int i = m, j = n;
    
    while (i > 0 && j > 0) {
        if (filtered1[i - 1] == filtered2[j - 1]) {
            lcs = filtered1[i - 1] + lcs;
            --i;
            --j;
        } else if (dp[i - 1][j] >= dp[i][j - 1]) {
            --i;
        } else {
            --j;
        }
    }
    
    return lcs;
}