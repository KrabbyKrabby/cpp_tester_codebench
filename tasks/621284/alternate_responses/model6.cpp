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

    for (int i = 0; i < dna.size(); ++i) {
        for (int j = i + 1; j <= dna.size(); ++j) {
            std::string substring = dna.substr(i, j - i);
            std::string reversed_substring = substring;
            std::reverse(reversed_substring.begin(), reversed_substring.end());

            bool is_complementary_palindrome = true;
            for (int k = 0; k < substring.size(); ++k) {
                if (substring[k] != complement(reversed_substring[k])) {
                    is_complementary_palindrome = false;
                    break;
                }
            }

            if (is_complementary_palindrome && substring.size() > longest_palindrome_substring.size()) {
                longest_palindrome_substring = substring;
            }
        }
    }
    return longest_palindrome_substring;
}
