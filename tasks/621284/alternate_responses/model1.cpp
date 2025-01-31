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

// Helper function to expand around center and find palindrome
std::string expandAroundCenter(const std::string& dna, int left, int right) {
    while (left >= 0 && right < dna.length() && 
           dna[left] == complement(dna[right])) {
        left--;
        right++;
    }
    return dna.substr(left + 1, right - left - 1);
}

// Function to find the longest complementary palindromic substring
std::string longestPalindromeSubstring(const std::string& dna) {
    if (dna.empty() || dna.length() < 2) return dna;
    
    std::string longest = "";
    
    // Check for all possible centers
    for (int i = 0; i < dna.length(); i++) {
        // Check for odd length palindromes
        std::string odd = expandAroundCenter(dna, i, i);
        if (odd.length() > longest.length()) {
            longest = odd;
        }
        
        // Check for even length palindromes
        if (i < dna.length() - 1) {
            std::string even = expandAroundCenter(dna, i, i + 1);
            if (even.length() > longest.length()) {
                longest = even;
            }
        }
    }
    
    return longest;
}