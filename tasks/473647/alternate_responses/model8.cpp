#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

class LandUsageManager {
private:
    std::vector<std::pair<int, int>> allocatedRanges;

public:
    // Constructor
    LandUsageManager() = default;

    // Allocate a range [start, end)
    void allocateLand(int start, int end) {
        if (start >= end) {
            throw std::invalid_argument("Invalid range: start must be less than end");
        }

        // Remove overlapping allocations
        allocatedRanges.erase(std::remove_if(allocatedRanges.begin(), allocatedRanges.end(),
            [start, end](const auto& range) {
                return start <= range.first && range.second <= end;
            }), allocatedRanges.end());

        // Add the new allocation
        allocatedRanges.push_back({start, end});

        // Sort the allocations
        std::sort(allocatedRanges.begin(), allocatedRanges.end());
    }

    // Check if a range [start, end) is fully allocated
    bool checkAvailability(int start, int end) {
        if (start >= end) {
            throw std::invalid_argument("Invalid range: start must be less than end");
        }

        // Check if the range overlaps with any allocations
        return std::none_of(allocatedRanges.begin(), allocatedRanges.end(),
            [start, end](const auto& range) {
                return start < range.second && range.first < end;
            });
    }

    // Release a range [start, end)
    void releaseLand(int start, int end) {
        if (start >= end) {
            throw std::invalid_argument("Invalid range: start must be less than end");
        }

        // Remove overlapping allocations
        allocatedRanges.erase(std::remove_if(allocatedRanges.begin(), allocatedRanges.end(),
            [start, end](const auto& range) {
                return start <= range.first && range.second <= end;
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

        mergedAllocations.push_back(allocatedRanges[0]);

        for (int i = 1; i < allocatedRanges.size(); ++i) {
            auto& lastRange = mergedAllocations.back();
            auto& currentRange = allocatedRanges[i];

            if (lastRange.second >= currentRange.first) {
                lastRange.second = std::max(lastRange.second, currentRange.second);
            } else {
                mergedAllocations.push_back(currentRange);
            }
        }

        return mergedAllocations;
    }
};