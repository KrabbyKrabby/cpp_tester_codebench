#include <gtest/gtest.h>
#include <vector>
#include <utility>

#include "solution.cpp" //

// Test cases
TEST(MaxProductArrangementTest, BasicCase) {
    std::vector<int> A = {1, 2};
    std::vector<int> B = {3, 4};
    auto result = maxProductArrangement(A, B);

    // Expected arrangement
    std::pair<std::vector<int>, std::vector<int>> expected = {
        {3, 2},  
        {1, 4}   
    };

    EXPECT_EQ(result, expected);
}

TEST(MaxProductArrangementTest, NoSwapsNeeded) {
    std::vector<int> A = {7, 6};
    std::vector<int> B = {5, 8};
    auto result = maxProductArrangement(A, B);

    // Expected arrangement
    std::pair<std::vector<int>, std::vector<int>> expected = {
        {7, 6},  
        {5, 8}   
    };

    EXPECT_EQ(result, expected);
}

TEST(MaxProductArrangementTest, AllSwapsNeeded) {
    std::vector<int> A = {1, 8};
    std::vector<int> B = {9, 2};
    auto result = maxProductArrangement(A, B);

    // Expected arrangement
    std::pair<std::vector<int>, std::vector<int>> expected = {
        {9, 2},  
        {1, 8}   
    };

    EXPECT_EQ(result, expected);
}

TEST(MaxProductArrangementTest, SingleElementVectors) {
    std::vector<int> A = {5};
    std::vector<int> B = {9};
    auto result = maxProductArrangement(A, B);

    // Expected arrangement
    std::pair<std::vector<int>, std::vector<int>> expected = {
        {9},  
        {5}   
    };

    EXPECT_EQ(result, expected);
}

TEST(MaxProductArrangementTest, EdgeCaseEmptyVectors) {
    std::vector<int> A = {};
    std::vector<int> B = {};
    auto result = maxProductArrangement(A, B);

    // Expected arrangement
    std::pair<std::vector<int>, std::vector<int>> expected = {
        {},  
        {}   
    };

    EXPECT_EQ(result, expected);
}

TEST(MaxProductArrangementTest, EdgeCaseSameDigitNumbers) {
    std::vector<int> A = {1, 1, 1, 1};
    std::vector<int> B = {1, 1, 1, 1};
    auto result = maxProductArrangement(A, B);

    // Expected arrangement
    std::pair<std::vector<int>, std::vector<int>> expected = {
        {1, 1, 1, 1},  
        {1, 1, 1, 1}   
    };

    EXPECT_EQ(result, expected);
}

// Test case for 20-digit edge case
TEST(MaxProductArrangementTest, EdgeCaseMaxSize) {
    std::vector<int> A = {1, 3, 5, 7, 9, 2, 4, 6, 8, 1, 1, 3, 5, 7, 9, 2, 4, 6, 8, 1};
    std::vector<int> B = {9, 8, 6, 4, 2, 1, 7, 5, 3, 9, 8, 6, 4, 2, 1, 7, 5, 3, 9, 8};
    auto result = maxProductArrangement(A, B);

    // Example expected result
    std::pair<std::vector<int>, std::vector<int>> expected = {
        {9, 3, 5, 4, 2, 1, 4, 5, 3, 1, 1, 3, 4, 2, 1, 2, 4, 3, 8, 1},  // Example first vector (batch1)
        {1, 8, 6, 7, 9, 2, 7, 6, 8, 9, 8, 6, 5, 7, 9, 7, 5, 6, 9, 8}   // Example second vector (batch2)
    };

    EXPECT_EQ(result, expected);
}