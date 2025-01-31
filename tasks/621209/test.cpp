#include <gtest/gtest.h>
#include <vector>
#include "solution.cpp"
using namespace std;

TEST(SolutionTest, TestSingleDay) {
    int days = 1;
    vector<vector<int>> meetings = {};
    EXPECT_EQ(countDays(days, meetings), 1);
}

TEST(SolutionTest, TestSingleMeetingExactDays) {
    int days = 5;
    vector<vector<int>> meetings = { {1, 5} };
    EXPECT_EQ(countDays(days, meetings), 0);
}

TEST(SolutionTest, TestMeetingsCoverPartialDays) {
    int days = 10;
    vector<vector<int>> meetings = { {2, 4}, {6, 8} };
    EXPECT_EQ(countDays(days, meetings), 4); 
}

TEST(SolutionTest, TestOverlappingMeetings) {
    int days = 10;
    vector<vector<int>> meetings = { {1, 5}, {4, 8} };
    EXPECT_EQ(countDays(days, meetings), 2);  
}

TEST(SolutionTest, TestNonOverlappingMeetings) {
    int days = 10;
    vector<vector<int>> meetings = { {1, 3}, {5, 7}, {9, 10} };
    EXPECT_EQ(countDays(days, meetings), 2);  
}

TEST(SolutionTest, TestAllDaysFree) {
    int days = 10;
    vector<vector<int>> meetings = {};
    EXPECT_EQ(countDays(days, meetings), 10);
}

TEST(SolutionTest, TestAllDaysMeeting) {
    int days = 10;
    vector<vector<int>> meetings = { {1, 10} };
    EXPECT_EQ(countDays(days, meetings), 0);
}

TEST(SolutionTest, TestSingleDayMeeting) {
    int days = 10;
    vector<vector<int>> meetings = { {5, 5} };
    EXPECT_EQ(countDays(days, meetings), 9);  
}

TEST(SolutionTest, TestMultipleOverlappingMeetings) {
    int days = 15;
    vector<vector<int>> meetings = { {1, 5}, {3, 7}, {6, 10} };
    EXPECT_EQ(countDays(days, meetings), 5);  
}

TEST(SolutionTest, TestEdgeDaysMeetings) {
    int days = 15;
    vector<vector<int>> meetings = { {1, 1}, {15, 15} };
    EXPECT_EQ(countDays(days, meetings), 13);  
}

TEST(SolutionTest, TestEmptyInputZeroDays) {
    int days = 0;
    vector<vector<int>> meetings = {};
    EXPECT_EQ(countDays(days, meetings), 0);
}

TEST(SolutionTest, TestLargeDaysSingleMeeting) {
    int days = 100000;
    vector<vector<int>> meetings = { {1, 99999} };
    EXPECT_EQ(countDays(days, meetings), 1);  
}