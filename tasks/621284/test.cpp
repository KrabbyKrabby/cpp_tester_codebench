#include <gtest/gtest.h>
#include "solution.cpp" 

TEST(LongestPalindromeSubstringTest, EmptyDNASequence) {
    std::string dna = "";
    std::string result = longestPalindromeSubstring(dna);
    EXPECT_EQ(result, "");
}

TEST(LongestPalindromeSubstringTest, SingleCharacterDNASequence) {
    std::string dna = "A";
    std::string result = longestPalindromeSubstring(dna);
    EXPECT_EQ(result, "");
}

TEST(LongestPalindromeSubstringTest, NoPalindromeDNASequence) {
    std::string dna = "AGGG";
    std::string result = longestPalindromeSubstring(dna);
    EXPECT_EQ(result, "");
}

TEST(LongestPalindromeSubstringTest, CompletePalindromeDNASequence) {
    std::string dna = "ATGCAT";
    std::string result = longestPalindromeSubstring(dna);
    EXPECT_EQ(result, "ATGCAT");
}

TEST(LongestPalindromeSubstringTest, MultiplePalindromesDNASequence) {
    std::string dna = "GACGCGTATCGA";
    std::string result = longestPalindromeSubstring(dna);
    EXPECT_EQ(result, "ACGCGT");
}

TEST(LongestPalindromeSubstringTest, NestedPalindromesDNASequence) {
    std::string dna = "CGCGGCATATGCATGC";
    std::string result = longestPalindromeSubstring(dna);
    EXPECT_EQ(result, "GCATATGC");
}

TEST(LongestPalindromeSubstringTest, PalindromeAtStart) {
    std::string dna = "AGCGCTATCCCC";
    std::string result = longestPalindromeSubstring(dna);
    EXPECT_EQ(result, "AGCGCT");
}

TEST(LongestPalindromeSubstringTest, ComplementaryPalindromeAtEnd) {
    std::string dna = "AAAACCCGGGGAGCTATAGCT";
    std::string result = longestPalindromeSubstring(dna);
    EXPECT_EQ(result, "AGCTATAGCT");
}