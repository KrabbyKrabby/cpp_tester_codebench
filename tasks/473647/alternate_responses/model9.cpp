#include <iostream>
#include <vector>
#include <algorithm>

class LandUsageManager {
private:
    // Data structure to store allocated ranges
    std::vector<std::pair<int, int>> allocatedRanges;

public:
    // Constructor
    LandUsageManager() {}

    // Allocate a range [start, end)
    void allocateLand(int start, int end) {
        if (start >= end) {
            throw std::invalid_argument("Invalid range: start must be less than end");
        }

        // Release any overlapping ranges
        releaseLand(start, end);

        // Add the new range to the allocated ranges
        allocatedRanges.emplace_back(start, end);
        std::sort(allocatedRanges.begin(), allocatedRanges.end());
    }

    // Check if a range [start, end) is fully allocated
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

    // Release a range [start, end)
    void releaseLand(int start, int end) {
        if (start >= end) {
            throw std::invalid_argument("Invalid range: start must be less than end");
        }

        // Find and remove overlapping ranges
        auto it = allocatedRanges.begin();
        while (it != allocatedRanges.end()) {
            if (start < it->second && end > it->first) {
                // Overlap found, remove the range
                it = allocatedRanges.erase(it);
            } else {
                ++it;
            }
        }
    }

    // Return all individual allocated ranges
    std::vector<std::pair<int, int>> getIndividualAllocations() {
        return allocatedRanges;
    }

    // Return merged allocation view
    std::vector<std::pair<int, int>> getMergedAllocations() {
        if (allocatedRanges.empty()) {
            return {};
        }

        std::vector<std::pair<int, int>> mergedRanges;
        mergedRanges.push_back(allocatedRanges[0]);

        for (size_t i = 1; i < allocatedRanges.size(); ++i) {
            const auto& currentRange = allocatedRanges[i];
            auto& lastMergedRange = mergedRanges.back();

            if (currentRange.first <= lastMergedRange.second) {
                // Merge the current range with the last merged range
                lastMergedRange.second = std::max(lastMergedRange.second, currentRange.second);
            } else {
                // Add the current range as a new merged range
                mergedRanges.push_back(currentRange);
            }
        }
        return mergedRanges;
    }
};