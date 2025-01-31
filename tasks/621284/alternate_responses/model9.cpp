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

// Helper function to check if a substring is a complementary palindrome
bool isComplementaryPalindrome(const std::string& dna, int start, int end) {
    for (int i = start; i <= (start + end) / 2; ++i) {
        if (dna[i] != complement(dna[end - i + start])) {
            return false;
        }
    }
    return true;
}

// Function to find the longest complementary palindromic substring
std::string longestPalindromeSubstring(const std::string& dna) {
    int n = dna.size();
    std::string longest_palindrome_substring = "";

    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            if (isComplementaryPalindrome(dna, i, j) && j - i + 1 > longest_palindrome_substring.size()) {
                longest_palindrome_substring = dna.substr(i, j - i + 1);
            }
        }
    }
    return longest_palindrome_substring;
}
