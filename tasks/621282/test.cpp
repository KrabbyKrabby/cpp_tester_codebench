#include "solution.cpp"

#include <vector>
#include <gtest/gtest.h>
#include <stack>

using namespace std;

TEST(ParenthesesScoreTests, TestMinimumLength) {
    EXPECT_EQ(calculateScore("()"), 1);
}

TEST(ParenthesesScoreTests, TestSimpleNonNested) {
    EXPECT_EQ(calculateScore("()()"), 2);
}

TEST(ParenthesesScoreTests, TestNestedParentheses) {
    EXPECT_EQ(calculateScore("(())"), 2);
}

TEST(ParenthesesScoreTests, TestMultipleNestedParentheses) {
    EXPECT_EQ(calculateScore("(()(()))"), 6);
}

TEST(ParenthesesScoreTests, TestDeeplyNested) {
    EXPECT_EQ(calculateScore("((((()))))"), 16);
}

TEST(ParenthesesScoreTests, TestLongSequenceOfSimpleParentheses) {
    EXPECT_EQ(calculateScore("()()()()()"), 5);
}

TEST(ParenthesesScoreTests, TestNonBalanced) {
    EXPECT_EQ(calculateScore("((()())"), 0);
}

TEST(ParenthesesScoreTests, TestLongNestedStructure) {
    EXPECT_EQ(calculateScore("((((())())))"), 24);
}

TEST(ParenthesesScoreTests, TestEdgeCaseNoInnerNested) {
    EXPECT_EQ(calculateScore("()()()()()()()()()()"), 10);
}

TEST(ParenthesesScoreTests, TestMaximumLength) {
    EXPECT_EQ(calculateScore("(((((((((((((((((())))))))))))))))))"), 131072);
}

TEST(ParenthesesScoreTests, TestEmpty) {
    EXPECT_EQ(calculateScore(""), 0);
}
// Main function for running tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
