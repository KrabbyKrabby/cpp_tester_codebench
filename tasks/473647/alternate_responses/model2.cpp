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

public:
    LandUsageManager() {}

    void allocateLand(int start, int end) {
        validateRange(start, end);
        
        // Release any overlapping ranges first
        releaseLand(start, end);
        
        // Insert new range at correct position
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
                // No overlap, keep range as is
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
        std::vector<std::pair<int, int>> result = allocatedRanges;
        std::sort(result.begin(), result.end());
        return result;
    }

    std::vector<std::pair<int, int>> getMergedAllocations() {
        if (allocatedRanges.empty()) {
            return {};
        }

        std::vector<std::pair<int, int>> merged;
        std::vector<std::pair<int, int>> sorted = allocatedRanges;
        std::sort(sorted.begin(), sorted.end());

        merged.push_back(sorted[0]);
        
        for (size_t i = 1; i < sorted.size(); i++) {
            if (merged.back().second >= sorted[i].first) {
                // Merge overlapping or adjacent ranges
                merged.back().second = std::max(merged.back().second, sorted[i].second);
            } else {
                // Add new non-overlapping range
                merged.push_back(sorted[i]);
            }
        }
        
        return merged;
    }
};
