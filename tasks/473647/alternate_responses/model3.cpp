#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

class LandUsageManager {
private:
    std::vector<std::pair<int, int>> allocatedRanges;

    void validateRange(int start, int end) {
        if (start >= end) {
            throw std::invalid_argument("Invalid range: start must be less than end");
        }
    }

    bool isOverlapping(const std::pair<int, int>& range1, const std::pair<int, int>& range2) {
        return range1.first < range2.second && range2.first < range1.second;
    }

    bool isAdjacent(const std::pair<int, int>& range1, const std::pair<int, int>& range2) {
        return range1.second == range2.first || range2.second == range1.first;
    }

public:
    LandUsageManager() {}

    void allocateLand(int start, int end) {
        validateRange(start, end);
        
        // Remove any overlapping ranges
        std::vector<std::pair<int, int>> newRanges;
        for (const auto& range : allocatedRanges) {
            if (!isOverlapping({start, end}, range)) {
                newRanges.push_back(range);
            }
        }
        
        // Add the new range
        newRanges.push_back({start, end});
        
        // Sort ranges by start position
        std::sort(newRanges.begin(), newRanges.end());
        
        allocatedRanges = newRanges;
    }

    bool checkAvailability(int start, int end) {
        validateRange(start, end);
        
        for (const auto& range : allocatedRanges) {
            if (isOverlapping({start, end}, range)) {
                return false;
            }
        }
        return true;
    }

    void releaseLand(int start, int end) {
        validateRange(start, end);
        
        std::vector<std::pair<int, int>> newRanges;
        for (const auto& range : allocatedRanges) {
            if (!isOverlapping({start, end}, range)) {
                newRanges.push_back(range);
            } else {
                // Handle partial overlaps
                if (range.first < start) {
                    newRanges.push_back({range.first, start});
                }
                if (range.second > end) {
                    newRanges.push_back({end, range.second});
                }
            }
        }
        
        std::sort(newRanges.begin(), newRanges.end());
        allocatedRanges = newRanges;
    }

    std::vector<std::pair<int, int>> getIndividualAllocations() {
        return allocatedRanges;
    }

    std::vector<std::pair<int, int>> getMergedAllocations() {
        if (allocatedRanges.empty()) {
            return {};
        }

        std::vector<std::pair<int, int>> merged;
        auto current = allocatedRanges[0];

        for (size_t i = 1; i < allocatedRanges.size(); i++) {
            if (isOverlapping(current, allocatedRanges[i]) || 
                isAdjacent(current, allocatedRanges[i])) {
                current.second = std::max(current.second, allocatedRanges[i].second);
            } else {
                merged.push_back(current);
                current = allocatedRanges[i];
            }
        }
        merged.push_back(current);

        return merged;
    }
};
