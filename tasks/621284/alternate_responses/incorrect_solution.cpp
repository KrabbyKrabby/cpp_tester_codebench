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

// Function to check if a substring is a complementary palindrome
bool isComplementaryPalindrome(const std::string& dna, int start, int end) {
    int left = start;
    int right = end;

    while (left < right) {
        if (dna[left] != complement(dna[right])) {
            return false;
        }
        left++;
        right--;
    }
    return true;
}

// Function to find the longest complementary palindromic substring
std::string longestPalindromeSubstring(const std::string& dna) {
    std::string longest_palindrome_substring = "";
    int max_length = 0;

    for (int i = 0; i < dna.size(); i++) {
        for (int j = i; j < dna.size(); j++) {
            if (isComplementaryPalindrome(dna, i, j) && (j - i + 1) > max_length) {
                max_length = j - i + 1;
                longest_palindrome_substring = dna.substr(i, j - i + 1);
            }
        }
    }
    return longest_palindrome_substring;
}