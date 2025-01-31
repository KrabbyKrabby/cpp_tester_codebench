#include <iostream>
#include <string>
#include <unordered_map>

// Function to get the complementary base of a nucleotide
char complement(char nucleotide) {
    static const std::unordered_map<char, char> complement_map = {
        {'A', 'T'},
        {'T', 'A'},
        {'C', 'G'},
        {'G', 'C'}
    };
    auto it = complement_map.find(nucleotide);
    return (it != complement_map.end()) ? it->second : '\0';
}

// Helper function to check if a substring is a complementary palindrome
bool isComplementaryPalindrome(const std::string& dna, int start, int length) {
    for (int i = 0; i < length / 2; i++) {
        if (dna[start + i] != complement(dna[start + length - 1 - i])) {
            return false;
        }
    }
    return true;
}

// Function to find the longest complementary palindromic substring
std::string longestPalindromeSubstring(const std::string& dna) {
    int n = dna.length();
    int maxLength = 0;
    int startIndex = 0;
    
    // Check all possible substrings
    for (int i = 0; i < n; i++) {
        for (int len = 2; i + len <= n; len += 2) {  // Only even lengths are valid
            if (isComplementaryPalindrome(dna, i, len) && len > maxLength) {
                maxLength = len;
                startIndex = i;
            }
        }
    }
    
    return maxLength > 0 ? dna.substr(startIndex, maxLength) : "";
}
