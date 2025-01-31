#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

class LandUsageManager {
private:
    // Data structure to store allocated ranges
    std::vector<std::pair<int, int>> allocatedRanges;

public:
    // Constructor
    LandUsageManager() = default;

    // Allocate a range [start, end)
    void allocateLand(int start, int end) {
        if (start >= end) {
            throw std::invalid_argument("Invalid range: start >= end");
        }

        // Find overlapping ranges and release them
        releaseOverlappingRanges(start, end);

        // Add the new allocation
        allocatedRanges.push_back({start, end});

        // Sort allocations by start value
        std::sort(allocatedRanges.begin(), allocatedRanges.end());
    }

    // Check if a range [start, end) is fully allocated
    bool checkAvailability(int start, int end) {
        if (start >= end) {
            throw std::invalid_argument("Invalid range: start >= end");
        }

        for (const auto& allocation : allocatedRanges) {
            if (start < allocation.second && end > allocation.first) {
                return false; // Overlap found
            }
        }

        return true; // No overlap found
    }

    // Release a range [start, end)
    void releaseLand(int start, int end) {
        if (start >= end) {
            throw std::invalid_argument("Invalid range: start >= end");
        }

        // Remove overlapping ranges
        allocatedRanges.erase(std::remove_if(allocatedRanges.begin(), allocatedRanges.end(),
            [start, end](const auto& allocation) {
                return start < allocation.second && end > allocation.first;
            }), allocatedRanges.end());
    }

    // Return all individual allocated ranges
    std::vector<std::pair<int, int>> getIndividualAllocations() {
        return allocatedRanges;
    }

    // Return merged allocation view
    std::vector<std::pair<int, int>> getMergedAllocations() {
        std::vector<std::pair<int, int>> mergedAllocations;

        if (allocatedRanges.empty()) {
            return mergedAllocations;
        }

        // Sort allocations by start value
        std::sort(allocatedRanges.begin(), allocatedRanges.end());

        mergedAllocations.push_back(allocatedRanges[0]);

        for (size_t i = 1; i < allocatedRanges.size(); ++i) {
            auto& lastMerged = mergedAllocations.back();
            const auto& current = allocatedRanges[i];

            if (current.first <= lastMerged.second) {
                // Merge overlapping or adjacent ranges
                lastMerged.second = std::max(lastMerged.second, current.second);
            } else {
                // Add non-overlapping range
                mergedAllocations.push_back(current);
            }
        }

        return mergedAllocations;
    }

private:
    // Release overlapping ranges for a given interval [start, end)
    void releaseOverlappingRanges(int start, int end) {
        allocatedRanges.erase(std::remove_if(allocatedRanges.begin(), allocatedRanges.end(),
            [start, end](const auto& allocation) {
                return start < allocation.second && end > allocation.first;
            }), allocatedRanges.end());
    }
};