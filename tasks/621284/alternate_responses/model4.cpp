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
    if (dna.empty()) {
        return "";
    }

    int max_length = 0;
    int start_pos = 0;

    // Check all possible substring lengths and starting positions
    for (int len = 2; len <= dna.length(); len += 2) {  // Only even lengths are valid
        for (int start = 0; start <= dna.length() - len; start++) {
            if (isComplementaryPalindrome(dna, start, len) && len > max_length) {
                max_length = len;
                start_pos = start;
            }
        }
    }

    return (max_length > 0) ? dna.substr(start_pos, max_length) : "";
}