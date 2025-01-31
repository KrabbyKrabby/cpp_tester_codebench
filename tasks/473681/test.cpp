#include <gtest/gtest.h>
#include <vector>
#include <climits>

#include "solution.cpp" //

// Test case: Basic functionality with gaps
TEST(FindMissingBandwidthPackagesTest, BasicFunctionality) {
    std::vector<int> bandwidthPackages = {1, 3};
    int maxBandwidth = 6;
    auto result = findMissingBandwidthPackages(bandwidthPackages, maxBandwidth);

    EXPECT_EQ(result.first, 1);  
    EXPECT_EQ(result.second, std::vector<int>({2}));  
}

// Test case: unsorted
TEST(FindMissingBandwidthPackagesTest, Unsorted) {
    std::vector<int> bandwidthPackages = {3, 1};
    int maxBandwidth = 6;
    auto result = findMissingBandwidthPackages(bandwidthPackages, maxBandwidth);

    EXPECT_EQ(result.first, 1);  
    EXPECT_EQ(result.second, std::vector<int>({2}));  
}

// Test case: Initial vector covers the whole range
TEST(FindMissingBandwidthPackagesTest, FullCoverage) {
    std::vector<int> bandwidthPackages = {1, 2, 4};
    int maxBandwidth = 7;
    auto result = findMissingBandwidthPackages(bandwidthPackages, maxBandwidth);

    EXPECT_EQ(result.first, 0);  
    EXPECT_TRUE(result.second.empty());  
}

// Test case: Multiple missing packages
TEST(FindMissingBandwidthPackagesTest, MultipleMissingPackages) {
    std::vector<int> bandwidthPackages = {50};
    int maxBandwidth = 50;
    auto result = findMissingBandwidthPackages(bandwidthPackages, maxBandwidth);

    EXPECT_EQ(result.first, 6);  
    EXPECT_EQ(result.second, std::vector<int>({1, 2, 4, 8, 16, 32})); 
}

// Test case: Duplicate bandwidth values and multiple possible answers
TEST(FindMissingBandwidthPackagesTest, DuplicateBandwidthPackages) {
    std::vector<int> bandwidthPackages = {1, 1, 2};
    int maxBandwidth = 5;
    auto result = findMissingBandwidthPackages(bandwidthPackages, maxBandwidth);

    EXPECT_EQ(result.first, 1);  
    EXPECT_EQ(result.second, std::vector<int>({5}));  
}

// Test case: Empty bandwidth packages
TEST(FindMissingBandwidthPackagesTest, EmptyBandwidthPackages) {
    std::vector<int> bandwidthPackages = {};
    int maxBandwidth = 5;
    auto result = findMissingBandwidthPackages(bandwidthPackages, maxBandwidth);

    EXPECT_EQ(result.first, 3);  
    EXPECT_EQ(result.second, std::vector<int>({1, 2, 4}));  
}


// Test case: Handling repeated bandwidths and full coverage
TEST(FindMissingBandwidthPackagesTest, RepeatedBandwidthWithUsageLimits) {
    std::vector<int> bandwidthPackages = {1, 1, 1, 2};
    int maxBandwidth = 5;
    auto result = findMissingBandwidthPackages(bandwidthPackages, maxBandwidth);

    EXPECT_EQ(result.first, 0);  
    EXPECT_EQ(result.second, std::vector<int>({}));  
}

// Test case: Maximum bandwidth limit with INT_MAX
TEST(FindMissingBandwidthPackagesTest, MaxBandwidthLimit) {
    std::vector<int> bandwidthPackages = {1, 2, 4, 8, 16};
    int maxBandwidth = INT_MAX;
    auto result = findMissingBandwidthPackages(bandwidthPackages, maxBandwidth);

    // Number of packages added should be 25
    EXPECT_EQ(result.first, 26);

    // Verify the added packages
    std::vector<int> expectedPackages = {
        32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072,
        262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432,
        67108864, 134217728, 268435456, 536870912, 1073741824
    };
    EXPECT_EQ(result.second, expectedPackages);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}