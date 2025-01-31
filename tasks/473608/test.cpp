#include <gtest/gtest.h>
#include <algorithm>
#include <vector>
#include <string>

#include "solution.cpp" //

TEST(FindMatchingContactsTest, BasicMatching) {
    std::vector<std::pair<std::string, int>> contacts = {
        {"Alice", 1}, {"Alicia", 2}, {"Bob", 3}, {"Charlie", 4}
    };

    std::vector<std::pair<std::string, int>> expected = {
        {"Alice", 1}, {"Alicia", 2}
    };

    EXPECT_EQ(findMatchingContacts(contacts, "Ali"), expected);
}


TEST(FindMatchingContactsTest, ResultIsSorted) {
    std::vector<std::pair<std::string, int>> contacts = {
        {"Charlie", 3}, {"Bob", 2}, {"Alice", 1}, {"Alicia", 4}
    };

    std::vector<std::pair<std::string, int>> result = findMatchingContacts(contacts, "A");
    EXPECT_TRUE(std::is_sorted(result.begin(), result.end()));
}

TEST(FindMatchingContactsTest, EmptyQuery) {
    std::vector<std::pair<std::string, int>> contacts = {
        {"Alice", 1}, {"Bob", 2}
    };

    std::vector<std::pair<std::string, int>> expected = {};
    EXPECT_EQ(findMatchingContacts(contacts, ""), expected);
}

TEST(FindMatchingContactsTest, CaseInsensitiveMatching) {
    std::vector<std::pair<std::string, int>> contacts = {
        {"Alice", 1}, {"aLICE", 2}, {"Bob", 3}
    };

    std::vector<std::pair<std::string, int>> expected = {
        {"Alice", 1}, {"aLICE", 2}
    };

    EXPECT_EQ(findMatchingContacts(contacts, "alice"), expected);
}

TEST(FindMatchingContactsTest, LeadingAndTrailingSpacesInQuery) {
    std::vector<std::pair<std::string, int>> contacts = {
        {"Alice", 1}, {"Bob", 2}
    };

    std::vector<std::pair<std::string, int>> expected = {
        {"Alice", 1}
    };

    EXPECT_EQ(findMatchingContacts(contacts, "  Alice  "), expected);
}

TEST(FindMatchingContactsTest, SpecialCharacterSupport) {
    std::vector<std::pair<std::string, int>> contacts = {
        {"Al!ce", 1}, {"Bob#", 2}, {"Charlie@", 3}
    };

    std::vector<std::pair<std::string, int>> expected = {
        {"Al!ce", 1}
    };

    EXPECT_EQ(findMatchingContacts(contacts, "Al!c"), expected);
}

TEST(FindMatchingContactsTest, NoMatches) {
    std::vector<std::pair<std::string, int>> contacts = {
        {"Alice", 1}, {"Bob", 2}
    };

    std::vector<std::pair<std::string, int>> expected = {};
    EXPECT_EQ(findMatchingContacts(contacts, "Xyz"), expected);
}