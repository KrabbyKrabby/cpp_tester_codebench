#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

class LandUsageManager {
private:
    std::vector<std::pair<int, int>> allocatedRanges;

    // Helper function to validate range
    void validateRange(int start, int end) {
        if (start >= end) {
            throw std::invalid_argument("Invalid range: start must be less than end");
        }
    }

public:
    LandUsageManager() {}

    void allocateLand(int start, int end) {
        validateRange(start, end);
        
        // First release any overlapping land
        releaseLand(start, end);
        
        // Insert new range at the correct position
        auto insertPos = std::lower_bound(allocatedRanges.begin(), allocatedRanges.end(),
            std::make_pair(start, end),
            [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                return a.first < b.first;
            });
            
        allocatedRanges.insert(insertPos, std::make_pair(start, end));
    }

    bool checkAvailability(int start, int end) {
        validateRange(start, end);
        
        for (const auto& range : allocatedRanges) {
            // Check if there's any overlap
            if (!(end <= range.first || start >= range.second)) {
                return false;
            }
        }
        return true;
    }

    void releaseLand(int start, int end) {
        validateRange(start, end);
        
        std::vector<std::pair<int, int>> newRanges;
        
        for (const auto& range : allocatedRanges) {
            if (range.second <= start || range.first >= end) {
                // No overlap, keep the range as is
                newRanges.push_back(range);
            } else {
                // Handle partial overlaps
                if (range.first < start) {
                    newRanges.push_back(std::make_pair(range.first, start));
                }
                if (range.second > end) {
                    newRanges.push_back(std::make_pair(end, range.second));
                }
            }
        }
        
        allocatedRanges = std::move(newRanges);
        std::sort(allocatedRanges.begin(), allocatedRanges.end());
    }

    std::vector<std::pair<int, int>> getIndividualAllocations() {
        // Return a copy of the current allocations (already sorted)
        return allocatedRanges;
    }

    std::vector<std::pair<int, int>> getMergedAllocations() {
        if (allocatedRanges.empty()) {
            return {};
        }

        std::vector<std::pair<int, int>> mergedRanges;
        std::pair<int, int> currentRange = allocatedRanges[0];

        for (size_t i = 1; i < allocatedRanges.size(); i++) {
            if (currentRange.second >= allocatedRanges[i].first) {
                // Merge overlapping or adjacent ranges
                currentRange.second = std::max(currentRange.second, allocatedRanges[i].second);
            } else {
                mergedRanges.push_back(currentRange);
                currentRange = allocatedRanges[i];
            }
        }
        mergedRanges.push_back(currentRange);

        return mergedRanges;
    }
};
