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

    for (int i = dna.size() / 2 - 1; i >= 0; --i) {
        if (dna[i] == complement(dna[dna.length() - i - 1])) {  
            longest_palindrome_substring = dna.substr(i, dna.size() - i); 
        } else {
            break;
        }
    }
    return longest_palindrome_substring;
}