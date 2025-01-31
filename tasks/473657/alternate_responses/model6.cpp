#include <iostream>
#include <string>
#include <vector>

std::string SequenceAllignment(const std::string& dna1, const std::string& dna2) {
    std::string dna1Filtered, dna2Filtered;
    for (char c : dna1) {
        switch (std::toupper(c)) {
            case 'A':
            case 'C':
            case 'G':
            case 'T':
                dna1Filtered += std::toupper(c);
                break;
        }
    }
    for (char c : dna2) {
        switch (std::toupper(c)) {
            case 'A':
            case 'C':
            case 'G':
            case 'T':
                dna2Filtered += std::toupper(c);
                break;
        }
    }
    int m = dna1Filtered.length();
    int n = dna2Filtered.length();

    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (dna1Filtered[i - 1] == dna2Filtered[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    std::string lcs;
    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (dna1Filtered[i - 1] == dna2Filtered[j - 1]) {
            lcs += dna1Filtered[i - 1];
            --i;
            --j;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            --i;
        } else {
            --j;
        }
    }
    std::reverse(lcs.begin(), lcs.end());
    return lcs;
}
