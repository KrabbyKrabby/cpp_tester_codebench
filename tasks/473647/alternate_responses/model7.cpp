#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

class LandUsageManager {
private:
    std::vector<std::pair<int, int>> allocatedRanges;

public:
    // Constructor
    LandUsageManager() {}

    // Allocate a range [start, end)
    void allocateLand(int start, int end) {
        if (start >= end) {
            throw std::invalid_argument("Invalid range: start must be less than end");
        }

        // Release overlapping ranges
        releaseLand(start, end);

        // Insert the new range
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

        // Remove overlapping ranges
        allocatedRanges.erase(std::remove_if(allocatedRanges.begin(), allocatedRanges.end(),
            [start, end](const auto& range) {
                return start < range.second && end > range.first;
            }), allocatedRanges.end());
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

        std::vector<std::pair<int, int>> merged;
        merged.push_back(allocatedRanges[0]);

        for (size_t i = 1; i < allocatedRanges.size(); ++i) {
            auto& lastMerged = merged.back();
            const auto& current = allocatedRanges[i];

            if (current.first <= lastMerged.second) {
                // Merge adjacent or overlapping ranges
                lastMerged.second = std::max(lastMerged.second, current.second);
            } else {
                // Add a new range
                merged.push_back(current);
            }
        }

        return merged;
    }
};
