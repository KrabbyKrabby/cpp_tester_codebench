#include <gtest/gtest.h>
#include <string>
#include <sstream>
#include <set>
#include <algorithm>
#include <unordered_set>
using namespace std;
#include "solution.cpp"


TEST(DecodeMessageTest, SingleWordUniqueCharacters) {
    EXPECT_EQ(decipherMessage("abcd"), "abcd");
}

TEST(DecodeMessageTest, SingleWordWithRepeatedCharacters) {
    EXPECT_EQ(decipherMessage("aabbcc"), "abc");
}

TEST(DecodeMessageTest, MultipleWordsWithUniqueCharacters) {
    EXPECT_EQ(decipherMessage("abcd efgh ijkl"), "abcd efgh ijkl");
}

TEST(DecodeMessageTest, MultipleWordsWithRepeatedCharacters) {
    EXPECT_EQ(decipherMessage("aabbcc ddee ffgg"), "abc de fg");
}

TEST(DecodeMessageTest, AllCharactersInDescendingOrder) {
    EXPECT_EQ(decipherMessage("zyx"), "xyz");
}

TEST(DecodeMessageTest, AllCharactersInAscendingOrder) {
    EXPECT_EQ(decipherMessage("abc"), "abc");
}

TEST(DecodeMessageTest, AllCharactersOfAlphabet) {
    EXPECT_EQ(decipherMessage("zyxwvutsrqponmlkjihgfedcba"), "abcdefghijklmnopqrstuvwxyz");
}

TEST(DecodeMessageTest, MultipleWordsWithVaryingLengths) {
    EXPECT_EQ(decipherMessage("wxyz abcd pqrs"), "wxyz abcd pqrs");
}

TEST(DecodeMessageTest, WordWithRepeatedCharactersMixedWithUnique) {
    EXPECT_EQ(decipherMessage("wwoorlldd"), "dlorw");
}

TEST(DecodeMessageTest, LargeInput) {
    EXPECT_EQ(decipherMessage("aaaa bbbb cccc dddd eeee ffff gggg hhhh iiiiiiiiiii"), "a b c d e f g h i");
}
TEST(DecodeMessageTest, WhitespacesOnly) {
    EXPECT_EQ(decipherMessage("     "), "");  // Only spaces, output should be empty
}

TEST(DecodeMessageTest, LeadingAndTrailingSpaces) {
    EXPECT_EQ(decipherMessage("   abcd   efgh   "), "abcd efgh");  // Leading and trailing spaces should be ignored
}
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}