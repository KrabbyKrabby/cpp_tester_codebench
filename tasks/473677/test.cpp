#include <gtest/gtest.h>
#include <vector>

#include "solution.cpp" //

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
    // Invalid case: packets < channels
    std::vector<int> packetSizes = {10, 20, 30};
    int numChannels = 5;
    EXPECT_THROW(calculateMaxChannelQuality(packetSizes, numChannels), std::invalid_argument);
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
    // Invalid case: packets < channels
    std::vector<int> packetSizes = {1};
    int numChannels = 2;
    EXPECT_THROW(calculateMaxChannelQuality(packetSizes, numChannels), std::invalid_argument);
}

TEST(CalculateMaxChannelQualityTest, ZeroChannels) {
    // Edge case: numChannels = 0
    std::vector<int> packetSizes = {10, 20, 30};
    int numChannels = 0;
    EXPECT_EQ(calculateMaxChannelQuality(packetSizes, numChannels), 0);
}

TEST(CalculateMaxChannelQualityTest, EmptyPacketSizes) {
    // Edge case: empty vector
    std::vector<int> packetSizes = {};
    int numChannels = 3;
    EXPECT_EQ(calculateMaxChannelQuality(packetSizes, numChannels), 0);
}

TEST(CalculateMaxChannelQualityTest, PacketsLessThanChannels) {
    // Edge case: number of packets is less than the number of channels
    std::vector<int> packetSizes = {10, 20}; // Only 2 packets
    int numChannels = 3; // More channels than packets

    EXPECT_THROW(calculateMaxChannelQuality(packetSizes, numChannels), std::invalid_argument);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}