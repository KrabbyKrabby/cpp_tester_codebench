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

// Function to check if a string is a complementary palindrome
bool isComplementaryPalindrome(const std::string& str) {
    int left = 0;
    int right = str.size() - 1;
    while (left < right) {
        if (str[left] != complement(str[right])) {
            return false;
        }
        ++left;
        --right;
    }
    return true;
}

// Function to find the longest complementary palindromic substring
std::string longestPalindromeSubstring(const std::string& dna) {
    std::string longest_palindrome_substring = "";

    for (int i = 0; i < dna.size(); ++i) {
        for (int j = i + 1; j <= dna.size(); ++j) {
            std::string substring = dna.substr(i, j - i);
            if (isComplementaryPalindrome(substring) && substring.size() > longest_palindrome_substring.size()) {
                longest_palindrome_substring = substring;
            }
        }
    }
    return longest_palindrome_substring;
}
