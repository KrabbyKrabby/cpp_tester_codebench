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

// Function to find the longest complementary palindromic substring
std::string longestPalindromeSubstring(const std::string& dna) {
    std::string longest_palindrome_substring = "";
    int max_length = 0;

    for (int i = 0; i < dna.size(); ++i) {
        for (int j = i + 1; j <= dna.size(); ++j) {
            std::string substring = dna.substr(i, j - i);
            std::string reverse_substring = substring;
            std::reverse(reverse_substring.begin(), reverse_substring.end());
            std::string complementary_substring = "";
            for (char nucleotide : reverse_substring) {
                complementary_substring += complement(nucleotide);
            }
            if (substring == complementary_substring && substring.size() > max_length) {
                longest_palindrome_substring = substring;
                max_length = substring.size();
            }
        }
    }
    return longest_palindrome_substring;
}