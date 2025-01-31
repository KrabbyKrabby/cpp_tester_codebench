#include <gtest/gtest.h>
#include <string>

#include "solution.cpp" //

// Test case 1: Both sequences are valid and have a common LCS
TEST(SequenceAlignmentTest, ValidSequencesWithCommonLCS) {
    std::string dna1 = "ACGTACGT";
    std::string dna2 = "ATTGT";
    EXPECT_EQ(SequenceAllignment(dna1, dna2), "ATGT");
}

// Test case 2: Multiple Sequences Possible
TEST(SequenceAlignmentTest, MultiplePossibleSequences) {
    std::string dna1 = "ACGTACGT";
    std::string dna2 = "GACGTG";
    EXPECT_EQ(SequenceAllignment(dna1, dna2), "ACGTG");
}

// Test case 3: One sequence is empty
TEST(SequenceAlignmentTest, OneEmptySequence) {
    std::string dna1 = "ACGT";
    std::string dna2 = "";
    EXPECT_EQ(SequenceAllignment(dna1, dna2), "");
}

// Test case 4: Both sequences are empty
TEST(SequenceAlignmentTest, BothEmptySequences) {
    std::string dna1 = "";
    std::string dna2 = "";
    EXPECT_EQ(SequenceAllignment(dna1, dna2), "");
}

// Test case 5: Case insensitivity
TEST(SequenceAlignmentTest, CaseInsensitiveAlignment) {
    std::string dna1 = "acgtACGT";
    std::string dna2 = "gAcgTG";
    EXPECT_EQ(SequenceAllignment(dna1, dna2), "ACGTG");
}

// Test case 6: Sequences with invalid characters
TEST(SequenceAlignmentTest, InvalidCharactersIgnored) {
    std::string dna1 = "ACGTTTXX123G";
    std::string dna2 = "GAC!GT?GXX123";
    EXPECT_EQ(SequenceAllignment(dna1, dna2), "ACGTG");
}

// Test case 7: No common subsequence
TEST(SequenceAlignmentTest, NoCommonSubsequence) {
    std::string dna1 = "AAAA";
    std::string dna2 = "TTTT";
    EXPECT_EQ(SequenceAllignment(dna1, dna2), "");
}

// Test case 8: Identical sequences
TEST(SequenceAlignmentTest, IdenticalSequences) {
    std::string dna1 = "ACGTACGT";
    std::string dna2 = "ACGTACGT";
    EXPECT_EQ(SequenceAllignment(dna1, dna2), "ACGTACGT");
}

// Test case 9: One sequence is a subsequence of the other
TEST(SequenceAlignmentTest, SubsequenceCase) {
    std::string dna1 = "ACGTACGT";
    std::string dna2 = "ACGT";
    EXPECT_EQ(SequenceAllignment(dna1, dna2), "ACGT");
}

// Test case 10: MULTIPLE POSSIBLE SEQUENCES STARTING AT SAME INDEX
TEST(SequenceAlignmentTest, MultiplePossibleSubsequencesAtSameIndex) {
    std::string dna1 = "ACGTCGTA";
    std::string dna2 = "ACGTAC";
    EXPECT_EQ(SequenceAllignment(dna1, dna2), "ACGTC");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}