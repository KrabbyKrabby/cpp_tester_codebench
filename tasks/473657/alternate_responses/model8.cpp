#include <iostream>
#include <string>
#include <vector>

// Function to find the Longest Common Subsequence (LCS)
std::string SequenceAllignment(const std::string& dna1, const std::string& dna2) {
    // Convert to uppercase for case-insensitive comparison
    std::string dna1Upper = dna1;
    std::string dna2Upper = dna2;

    // Remove invalid characters and convert to uppercase
    dna1Upper.erase(std::remove_if(dna1Upper.begin(), dna1Upper.end(), [](char c){ return c != 'A' && c != 'C' && c != 'G' && c != 'T' && c != 'a' && c != 'c' && c != 'g' && c != 't'; }), dna1Upper.end());
    dna2Upper.erase(std::remove_if(dna2Upper.begin(), dna2Upper.end(), [](char c){ return c != 'A' && c != 'C' && c != 'G' && c != 'T' && c != 'a' && c != 'c' && c != 'g' && c != 't'; }), dna2Upper.end());

    for (char& c : dna1Upper) {
        c = std::toupper(c);
    }

    for (char& c : dna2Upper) {
        c = std::toupper(c);
    }

    int m = dna1Upper.length();
    int n = dna2Upper.length();
    
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (dna1Upper[i - 1] == dna2Upper[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    std::string lcs;
    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (dna1Upper[i - 1] == dna2Upper[j - 1]) {
            lcs.push_back(dna1Upper[i - 1]);
            i--;
            j--;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }

    std::reverse(lcs.begin(), lcs.end());
    return lcs;
}
