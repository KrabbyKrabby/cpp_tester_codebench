#include <iostream>
#include <string>
#include <unordered_map>

// Function to get the complementary base of a nucleotide
char complement(char nucleotide) {
    std::unordered_map<char, char> complement_map = {
        {'A', 'T'},
        {'T', 'A'},
        {'C', 'G'},
        {'G', 'C'}
    };
    return complement_map[nucleotide];
}

// Helper function to check if a substring is a valid complementary palindrome
bool isComplementaryPalindrome(const std::string& dna, int start, int end) {
    while (start < end) {
        if (dna[start] != complement(dna[end])) {
            return false;
        }
        start++;
        end--;
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
        for (int j = i; j < n; j++) {
            int length = j - i + 1;
            // Only check if current length is greater than max found so far
            if (length > maxLength && isComplementaryPalindrome(dna, i, j)) {
                maxLength = length;
                startIndex = i;
            }
        }
    }
    
    return maxLength > 0 ? dna.substr(startIndex, maxLength) : "";
}