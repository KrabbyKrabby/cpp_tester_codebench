#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

class LandUsageManager {
private:
    std::vector<std::pair<int, int>> allocatedRanges;

public:
    LandUsageManager() {}

    void allocateLand(int start, int end) {
        if (start >= end) {
            throw std::invalid_argument("Invalid range: start must be less than end");
        }

        // Release any overlapping ranges
        releaseLand(start, end);

        // Add the new range
        allocatedRanges.push_back({start, end});
        std::sort(allocatedRanges.begin(), allocatedRanges.end());
    }

    bool checkAvailability(int start, int end) {
        if (start >= end) {
            throw std::invalid_argument("Invalid range: start must be less than end");
        }

        for (const auto& range : allocatedRanges) {
            if (start < range.second && end > range.first) {
                return false; // Overlap found
            }
        }
        return true; // No overlap found
    }

    void releaseLand(int start, int end) {
        if (start >= end) {
            throw std::invalid_argument("Invalid range: start must be less than end");
        }

        // Remove overlapping ranges
        allocatedRanges.erase(std::remove_if(allocatedRanges.begin(), allocatedRanges.end(),
            [start, end](const auto& range) {
                return start < range.second && end > range.first;
            }), allocatedRanges.end());
    }

    std::vector<std::pair<int, int>> getIndividualAllocations() {
        return allocatedRanges;
    }

    std::vector<std::pair<int, int>> getMergedAllocations() {
        std::vector<std::pair<int, int>> mergedRanges;

        if (allocatedRanges.empty()) {
            return mergedRanges;
        }

        // Sort the ranges by start value
        std::sort(allocatedRanges.begin(), allocatedRanges.end());

        mergedRanges.push_back(allocatedRanges[0]);
        for (size_t i = 1; i < allocatedRanges.size(); ++i) {
            const auto& currentRange = allocatedRanges[i];
            auto& lastMergedRange = mergedRanges.back();

            // Check for overlap or adjacency
            if (currentRange.first <= lastMergedRange.second) {
                // Merge the ranges
                lastMergedRange.second = std::max(lastMergedRange.second, currentRange.second);
            } else {
                // Add the new range
                mergedRanges.push_back(currentRange);
            }
        }

        return mergedRanges;
    }
};
