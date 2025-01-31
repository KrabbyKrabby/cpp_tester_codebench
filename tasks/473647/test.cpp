#include <gtest/gtest.h>
#include <vector>
#include <utility>

#include "solution.cpp"


// Test constructor initializes with no allocations
TEST(LandUsageManagerTest, Constructor) {
    LandUsageManager manager;
    EXPECT_TRUE(manager.getIndividualAllocations().empty());
    EXPECT_TRUE(manager.getMergedAllocations().empty());
}

// Test land availability: Initially available
TEST(LandUsageManagerTest, LandInitiallyAvailable) {
    LandUsageManager manager;
    EXPECT_TRUE(manager.checkAvailability(10, 20));
}

// Test simple allocation
TEST(LandUsageManagerTest, SimpleAllocation) {
    LandUsageManager manager;
    manager.allocateLand(10, 20);

    std::vector<std::pair<int, int>> expectedIndividual = {{10, 20}};
    EXPECT_EQ(manager.getIndividualAllocations(), expectedIndividual);
    EXPECT_EQ(manager.getMergedAllocations(), expectedIndividual);
}


// Test land availability: Fully allocated range
TEST(LandUsageManagerTest, FullyAllocatedRange) {
    LandUsageManager manager;
    manager.allocateLand(10, 20);
    EXPECT_FALSE(manager.checkAvailability(10, 20));
}

// Test overlapping allocation behavior
TEST(LandUsageManagerTest, OverlappingAllocation) {
    LandUsageManager manager;
    manager.allocateLand(10, 20);
    manager.allocateLand(15, 25);

    // Individual allocations should keep both ranges and release the overlapping portion from the old range
    std::vector<std::pair<int, int>> expectedIndividual = {{10, 15}, {15, 25}};
    EXPECT_EQ(manager.getIndividualAllocations(), expectedIndividual);

    // Merged view should combine them
    std::vector<std::pair<int, int>> expectedMerged = {{10, 25}};
    EXPECT_EQ(manager.getMergedAllocations(), expectedMerged);
}

// Test allocating a range that partially overlaps multiple ranges
TEST(LandUsageManagerTest, OverlappingAllocationMultipleRanges) {
    LandUsageManager manager;

    // Allocate multiple ranges
    manager.allocateLand(10, 20);
    manager.allocateLand(30, 40);

    // Allocate a range that partially overlaps the existing ranges
    manager.allocateLand(15, 35);

    // Individual allocations should show new range taking precedence
    std::vector<std::pair<int, int>> expectedIndividual = {{10, 15}, {15, 35}, {35, 40}};
    EXPECT_EQ(manager.getIndividualAllocations(), expectedIndividual);

    // Merged view should reflect the continuous allocation
    std::vector<std::pair<int, int>> expectedMerged = {{10, 40}};
    EXPECT_EQ(manager.getMergedAllocations(), expectedMerged);
}

// Test land availability: Partial overlap
TEST(LandUsageManagerTest, PartialOverlapUnavailable) {
    LandUsageManager manager;
    manager.allocateLand(10, 20);
    EXPECT_FALSE(manager.checkAvailability(15, 25));
}

// Test releasing a portion of a range
TEST(LandUsageManagerTest, PartialRelease) {
    LandUsageManager manager;
    manager.allocateLand(10, 30);
    manager.releaseLand(15, 20);

    // Individual allocations should split the range
    std::vector<std::pair<int, int>> expectedIndividual = {{10, 15}, {20, 30}};
    EXPECT_EQ(manager.getIndividualAllocations(), expectedIndividual);

    // Merged view should still combine the ranges
    std::vector<std::pair<int, int>> expectedMerged = {{10, 15}, {20, 30}};
    EXPECT_EQ(manager.getMergedAllocations(), expectedMerged);
}

// Test releasing a range that spans multiple allocated ranges
TEST(LandUsageManagerTest, ReleaseAcrossMultipleRanges) {
    LandUsageManager manager;

    // Allocate multiple ranges
    manager.allocateLand(10, 20);
    manager.allocateLand(30, 40);
    manager.allocateLand(50, 60);

    // Release a range that spans across multiple allocated ranges
    manager.releaseLand(15, 55);

    // Individual allocations should retain non-overlapping portions
    std::vector<std::pair<int, int>> expectedIndividual = {{10, 15}, {55, 60}};
    EXPECT_EQ(manager.getIndividualAllocations(), expectedIndividual);

    // Merged view should reflect the same non-overlapping portions
    std::vector<std::pair<int, int>> expectedMerged = {{10, 15}, {55, 60}};
    EXPECT_EQ(manager.getMergedAllocations(), expectedMerged);
}

// Test complete release
TEST(LandUsageManagerTest, CompleteReleaseIndividualView) {
    LandUsageManager manager;
    manager.allocateLand(10, 20);
    manager.allocateLand(30, 40);
    manager.releaseLand(10, 40);
    EXPECT_TRUE(manager.getIndividualAllocations().empty());
    EXPECT_TRUE(manager.getMergedAllocations().empty());

}

// Test complete release: Availability check
TEST(LandUsageManagerTest, CompleteReleaseAvailabilityCheck) {
    LandUsageManager manager;
    manager.allocateLand(10, 20);
    manager.allocateLand(30, 40);
    manager.releaseLand(10, 40);
    EXPECT_TRUE(manager.checkAvailability(10, 40));
}

// Test back-to-back allocations
TEST(LandUsageManagerTest, BackToBackAllocations) {
    LandUsageManager manager;
    manager.allocateLand(10, 20);
    manager.allocateLand(20, 30);

    // Individual view should have separate allocations
    std::vector<std::pair<int, int>> expectedIndividual = {{10, 20}, {20, 30}};
    EXPECT_EQ(manager.getIndividualAllocations(), expectedIndividual);

    // Merged view should combine them
    std::vector<std::pair<int, int>> expectedMerged = {{10, 30}};
    EXPECT_EQ(manager.getMergedAllocations(), expectedMerged);
}

// Test land availability: Adjacent range
TEST(LandUsageManagerTest, AdjacentRangeAvailable) {
    LandUsageManager manager;
    manager.allocateLand(10, 20);
    EXPECT_TRUE(manager.checkAvailability(20, 30));
}

// Test with large allocations
TEST(LandUsageManagerTest, LargeAllocations) {
    LandUsageManager manager;
    for (int i = 0; i < 1000; i += 10) {
        manager.allocateLand(i, i + 5); // Small non-overlapping allocations
    }

    // Individual view should have all ranges
    std::vector<std::pair<int, int>> expectedIndividual;
    for (int i = 0; i < 1000; i += 10) {
        expectedIndividual.push_back({i, i + 5});
    }
    EXPECT_EQ(manager.getIndividualAllocations(), expectedIndividual);

    // Merged view should be identical since no overlaps
    EXPECT_EQ(manager.getMergedAllocations(), expectedIndividual);
}

// Test invalid ranges: Allocation
TEST(LandUsageManagerTest, InvalidAllocation) {
    LandUsageManager manager;
    manager.allocateLand(10, 20);

    EXPECT_THROW(manager.allocateLand(20, 10), std::invalid_argument);
    EXPECT_THROW(manager.allocateLand(10, 10), std::invalid_argument);
}

// Test invalid ranges: Availability check
TEST(LandUsageManagerTest, InvalidAvailabilityCheck) {
    LandUsageManager manager;
    manager.allocateLand(10, 20);

    EXPECT_THROW(manager.checkAvailability(20, 10), std::invalid_argument);
    EXPECT_THROW(manager.checkAvailability(10, 10), std::invalid_argument);
}

// Test invalid ranges: Release
TEST(LandUsageManagerTest, InvalidRelease) {
    LandUsageManager manager;
    manager.allocateLand(10, 20);

    EXPECT_THROW(manager.releaseLand(20, 10), std::invalid_argument);
    EXPECT_THROW(manager.releaseLand(10, 10), std::invalid_argument);
}