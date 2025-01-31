#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>

char complement(char nucleotide) {
    std::unordered_map<char, char> complement_map = {
        {'A', 'T'},
        {'T', 'A'},
        {'C', 'G'},
        {'G', 'C'}
    };
    return complement_map[nucleotide];
}

// Helper function to get reverse complement of a DNA sequence
std::string reverseComplement(const std::string& seq) {
    std::string result = seq;
    std::reverse(result.begin(), result.end());
    for (char& c : result) {
        c = complement(c);
    }
    return result;
}

// Helper function to check if a substring is a complementary palindrome
bool isComplementaryPalindrome(const std::string& seq) {
    return seq == reverseComplement(seq);
}

std::string longestPalindromeSubstring(const std::string& dna) {
    std::string longest_palindrome = "";
    int n = dna.length();
    
    // Check all possible substrings
    for (int i = 0; i < n; i++) {
        for (int len = 2; i + len <= n; len += 2) {  // Only even lengths are valid
            std::string substr = dna.substr(i, len);
            if (isComplementaryPalindrome(substr)) {
                if (substr.length() > longest_palindrome.length()) {
                    longest_palindrome = substr;
                }
            }
        }
    }
    
    return longest_palindrome;
}