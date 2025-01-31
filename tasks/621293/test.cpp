#include "gtest/gtest.h"
#include "solution.cpp"

TEST(CalculateMaxChannelQualityTest, BasicTests) {

    std::vector<int> packetSizes_1 = {1, 2, 3, 4, 5}; // Sorted
    int numChannels_1 = 2;
    EXPECT_EQ(calculateMaxChannelQuality(packetSizes_1, numChannels_1), 8);

    std::vector<int> packetSizes_2 = {5, 2, 2, 1, 5, 3}; // Unsorted
    int numChannels_2 = 2;
    EXPECT_EQ(calculateMaxChannelQuality(packetSizes_2, numChannels_2), 7);
}

TEST(CalculateMaxChannelQualityTest, SingleChannel) {
    
    std::vector<int> packetSizes = {10, 20, 30, 40, 50};
    int numChannels = 1;
    EXPECT_EQ(calculateMaxChannelQuality(packetSizes, numChannels), 30); 
}

TEST(CalculateMaxChannelQualityTest, EqualPacketAndChannels) {

    std::vector<int> packetSizes = {10, 20, 30, 40};
    int numChannels = 4;
    EXPECT_EQ(calculateMaxChannelQuality(packetSizes, numChannels), 100); 
}

TEST(CalculateMaxChannelQualityTest, FewPacketsAndManyChannels) {

    std::vector<int> packetSizes = {10, 20, 30, 40, 50};
    int numChannels = 10;
    EXPECT_EQ(calculateMaxChannelQuality(packetSizes, numChannels), 150); 
}

TEST(CalculateMaxChannelQualityTest, LargePacketSizes) {

    std::vector<int> packetSizes = {1000000, 2000000, 3000000, 4000000, 5000000};
    int numChannels = 3;
    EXPECT_EQ(calculateMaxChannelQuality(packetSizes, numChannels), 11000000);
}

TEST(CalculateMaxChannelQualityTest, OnePacketOneChannel) {

    std::vector<int> packetSizes = {1};
    int numChannels = 1;
    EXPECT_EQ(calculateMaxChannelQuality(packetSizes, numChannels), 1);
}

TEST(CalculateMaxChannelQualityTest, OnePacketManyChannels) {

    std::vector<int> packetSizes = {1};
    int numChannels = 10;
    EXPECT_EQ(calculateMaxChannelQuality(packetSizes, numChannels), 1);
}

TEST(CalculateMaxChannelQualityTest, ZeroChannels) {

    std::vector<int> packetSizes = {10, 20, 30};
    int numChannels = 0;
    EXPECT_EQ(calculateMaxChannelQuality(packetSizes, numChannels), 0);
}