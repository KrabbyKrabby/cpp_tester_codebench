#include <iostream>
#include <string>
#include <vector>

std::string SequenceAllignment(const std::string& dna1, const std::string& dna2) {
    // Helper function to check if character is valid DNA nucleotide
    auto isValidNucleotide = [](char c) {
        c = std::toupper(c);
        return c == 'A' || c == 'C' || c == 'G' || c == 'T';
    };

    // Clean and convert sequences to uppercase
    std::string cleanDna1, cleanDna2;
    for (char c : dna1) {
        if (isValidNucleotide(c)) {
            cleanDna1 += std::toupper(c);
        }
    }
    for (char c : dna2) {
        if (isValidNucleotide(c)) {
            cleanDna2 += std::toupper(c);
        }
    }

    int m = cleanDna1.length();
    int n = cleanDna2.length();
    
    // Create DP table
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    // Fill DP table
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
    std::string lcs;
    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (cleanDna1[i - 1] == cleanDna2[j - 1]) {
            lcs = cleanDna1[i - 1] + lcs;
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