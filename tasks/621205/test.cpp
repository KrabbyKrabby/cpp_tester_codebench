#include <gtest/gtest.h>
#include <unordered_map>
#include <string>
#include "solution.cpp"

using namespace std;

TEST(LongestUniqueWeightedSubstrTest, SimpleUniqueCharacters) {
    string input = "abc";
    unordered_map<char, int> weights = {{'a', 1}, {'b', 2}, {'c', 3}};
    int expected = 6;
    EXPECT_EQ(longestUniqueSubstr(input, weights), expected);
}

TEST(LongestUniqueWeightedSubstrTest, AllCharactersSame) {
    string input = "aaaa";
    unordered_map<char, int> weights = {{'a', 4}};
    int expected = 4;
    EXPECT_EQ(longestUniqueSubstr(input, weights), expected);
}

TEST(LongestUniqueWeightedSubstrTest, MixedCaseCharacters) {
    string input = "AaBbCc";
    unordered_map<char, int> weights = {{'a', 1}, {'b', 2}, {'c', 3}};
    int expected = 9;
    EXPECT_EQ(longestUniqueSubstr(input, weights), expected);
}

TEST(LongestUniqueWeightedSubstrTest, NonAlphanumericIgnored) {
    string input = "a!@#b$c";
    unordered_map<char, int> weights = {{'a', 4}, {'b', 2}, {'c', 1}};
    int expected = 7;
    EXPECT_EQ(longestUniqueSubstr(input, weights), expected);
}

TEST(LongestUniqueWeightedSubstrTest, AllUniqueAlphabet) {
    string input = "abcdefghijklmnopqrstuvwxyz";
    unordered_map<char, int> weights = {
        {'a', 1}, {'b', 2}, {'c', 3}, {'d', 4}, {'e', 5}, {'f', 6}, {'g', 7},
        {'h', 8}, {'i', 9}, {'j', 10}, {'k', 11}, {'l', 12}, {'m', 13}, {'n', 14},
        {'o', 15}, {'p', 16}, {'q', 17}, {'r', 18}, {'s', 19}, {'t', 20},
        {'u', 21}, {'v', 22}, {'w', 23}, {'x', 24}, {'y', 25}, {'z', 26}
    };
    int expected = 351;
    EXPECT_EQ(longestUniqueSubstr(input, weights), expected);
}

TEST(LongestUniqueWeightedSubstrTest, RepeatedBlocksOfCharacters) {
    string input = "aaaabbbbcccc";
    unordered_map<char, int> weights = {{'a', 1}, {'b', 1}, {'c', 1}};
    int expected = 2;
    EXPECT_EQ(longestUniqueSubstr(input, weights), expected);
}

TEST(LongestUniqueWeightedSubstrTest, IncreasingWeightsWithUniqueCharacters) {
    string input = "abcde";
    unordered_map<char, int> weights = {{'a', 10}, {'b', 20}, {'c', 30}, {'d', 40}, {'e', 50}};
    int expected = 150;
    EXPECT_EQ(longestUniqueSubstr(input, weights), expected);
}

TEST(LongestUniqueWeightedSubstrTest, AlphanumericCharactersWithWeights) {
    string input = "a1b2c3";
    unordered_map<char, int> weights = {{'a', 1}, {'1', 2}, {'b', 3}, {'2', 4}, {'c', 5}, {'3', 6}};
    int expected = 21;
    EXPECT_EQ(longestUniqueSubstr(input, weights), expected);
}

TEST(LongestUniqueWeightedSubstrTest, MixedCharactersWithWeights) {
    string input = "a@b!a$c%d^";
    unordered_map<char, int> weights = {{'a', 5}, {'b', 3}, {'c', 2}, {'d', 4}};
    int expected = 14;
    EXPECT_EQ(longestUniqueSubstr(input, weights), expected);
}

TEST(LongestUniqueWeightedSubstrTest, RepeatedCharactersIgnored) {
    string input = "a@a!b#b$c%c^";
    unordered_map<char, int> weights = {{'a', 4}, {'b', 2}, {'c', 1}};
    int expected = 6;
    EXPECT_EQ(longestUniqueSubstr(input, weights), expected);
}

TEST(LongestUniqueWeightedSubstrTest, EmptyString) {
    string input = "";
    unordered_map<char, int> weights = {{'a', 4}, {'b', 2}, {'c', 1}};
    int expected = 0;
    EXPECT_EQ(longestUniqueSubstr(input, weights), expected);
}

TEST(LongestUniqueWeightedSubstrTest, UniqueCharactersEqualWeights) {
    string input = "abcdefg";
    unordered_map<char, int> weights = {{'a', 1}, {'b', 1}, {'c', 1}, {'d', 1}, {'e', 1}, {'f', 1}, {'g', 1}};
    int expected = 7;
    EXPECT_EQ(longestUniqueSubstr(input, weights), expected);
}

TEST(LongestUniqueWeightedSubstrTest, RepeatingPattern) {
    string input = "abacabadabacaba";
    unordered_map<char, int> weights = {{'a', 1}, {'b', 2}, {'c', 3}, {'d', 4}};
    int expected = 7;
    EXPECT_EQ(longestUniqueSubstr(input, weights), expected);
}

TEST(LongestUniqueWeightedSubstrTest, SingleCharacterZeroWeight) {
    string input = "a";
    unordered_map<char, int> weights = {{'a', 0}};
    int expected = 0;
    EXPECT_EQ(longestUniqueSubstr(input, weights), expected);
}

TEST(LongestUniqueWeightedSubstrTest, SingleCharacterNonZeroWeight) {
    string input = "a";
    unordered_map<char, int> weights = {{'a', 4}};
    int expected = 4;
    EXPECT_EQ(longestUniqueSubstr(input, weights), expected);
}

TEST(LongestUniqueWeightedSubstrTest, ExtraWeightsNonExistentCharacters) {
    string input = "aabbcc";
    unordered_map<char, int> weights = {{'a', 1}, {'b', 1}, {'c', 1}, {'d', 1}};
    int expected = 2;
    EXPECT_EQ(longestUniqueSubstr(input, weights), expected);
}

TEST(LongestUniqueWeightedSubstrTest, InvalidWeightNonAlphanumeric) {
    string input = "&";
    unordered_map<char, int> weights = {{'&', 1}, {'b', 1}, {'c', 1}, {'d', 1}};
    int expected = 0;
    EXPECT_EQ(longestUniqueSubstr(input, weights), expected);
}