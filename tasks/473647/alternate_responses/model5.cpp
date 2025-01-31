#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

class LandUsageManager {
private:
    std::vector<std::pair<int, int>> allocatedRanges;

    // Helper function to sort and merge overlapping ranges
    void sortAndMergeRanges() {
        if (allocatedRanges.empty()) return;
        
        std::sort(allocatedRanges.begin(), allocatedRanges.end());
        
        std::vector<std::pair<int, int>> merged;
        merged.push_back(allocatedRanges[0]);
        
        for (size_t i = 1; i < allocatedRanges.size(); i++) {
            if (merged.back().second >= allocatedRanges[i].first) {
                merged.back().second = std::max(merged.back().second, allocatedRanges[i].second);
            } else {
                merged.push_back(allocatedRanges[i]);
            }
        }
        
        allocatedRanges = merged;
    }

public:
    LandUsageManager() {}

    void allocateLand(int start, int end) {
        if (start >= end) {
            throw std::invalid_argument("Invalid range: start must be less than end");
        }

        // First release any existing allocations in this range
        releaseLand(start, end);
        
        // Add the new allocation
        allocatedRanges.push_back({start, end});
        sortAndMergeRanges();
    }

    bool checkAvailability(int start, int end) {
        if (start >= end) {
            throw std::invalid_argument("Invalid range: start must be less than end");
        }

        for (const auto& range : allocatedRanges) {
            if (!(end <= range.first || start >= range.second)) {
                return false;
            }
        }
        return true;
    }

    void releaseLand(int start, int end) {
        if (start >= end) {
            throw std::invalid_argument("Invalid range: start must be less than end");
        }

        std::vector<std::pair<int, int>> newRanges;
        
        for (const auto& range : allocatedRanges) {
            if (range.second <= start || range.first >= end) {
                // Range doesn't overlap with release area
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
        
        allocatedRanges = newRanges;
        sortAndMergeRanges();
    }

    std::vector<std::pair<int, int>> getIndividualAllocations() {
        std::sort(allocatedRanges.begin(), allocatedRanges.end());
        return allocatedRanges;
    }

    std::vector<std::pair<int, int>> getMergedAllocations() {
        std::vector<std::pair<int, int>> merged = allocatedRanges;
        if (merged.empty()) return merged;
        
        std::sort(merged.begin(), merged.end());
        std::vector<std::pair<int, int>> result;
        result.push_back(merged[0]);
        
        for (size_t i = 1; i < merged.size(); i++) {
            if (result.back().second >= merged[i].first) {
                result.back().second = std::max(result.back().second, merged[i].second);
            } else {
                result.push_back(merged[i]);
            }
        }
        
        return result;
    }
};
