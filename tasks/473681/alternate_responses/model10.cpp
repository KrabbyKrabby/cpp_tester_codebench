#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>

std::pair<int, std::vector<int>> findMissingBandwidthPackages(std::vector<int>& bandwidthPackages, int maxBandwidth) {
    int currentCoverage = 0; // Tracks the current maximum bandwidth we can cover
    int missingPackageCount = 0; // Counts the number of missing packages added
    std::vector<int> addedPackages; // Stores the missing packages added
    std::unordered_set<int> availableBandwidths(bandwidthPackages.begin(), bandwidthPackages.end()); // Tracks available bandwidths for quick lookup
    std::sort(bandwidthPackages.begin(), bandwidthPackages.end()); // Sort bandwidth packages in ascending order
    int index = 0; // Tracks the index in the bandwidthPackages array
    
    while (currentCoverage != maxBandwidth && index < bandwidthPackages.size()) {
        if (bandwidthPackages[index] == currentCoverage + 1) {
            currentCoverage += 1;
            index++;
            continue;
        } else {
            int nextBandwidth = currentCoverage + 1;
            while (availableBandwidths.find(nextBandwidth) != availableBandwidths.end()) {
                nextBandwidth++;
            }
            addedPackages.push_back(nextBandwidth);
            missingPackageCount += 1;
            currentCoverage = nextBandwidth;
        }
    }
    if (currentCoverage != maxBandwidth) {
        int nextBandwidth = currentCoverage + 1;
        while (nextBandwidth <= maxBandwidth) {
            if (availableBandwidths.find(nextBandwidth) == availableBandwidths.end()) {
                addedPackages.push_back(nextBandwidth);
                missingPackageCount += 1;
            }
            nextBandwidth++;
        }
    }
    
    return {missingPackageCount, addedPackages};
}