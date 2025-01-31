#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>

std::pair<int, std::vector<int>> findMissingBandwidthPackages(std::vector<int>& bandwidthPackages, int maxBandwidth) {
    std::sort(bandwidthPackages.begin(), bandwidthPackages.end());
    std::unordered_set<int> existingPackages(bandwidthPackages.begin(), bandwidthPackages.end());
    int currentCoverage = 0; // Tracks the current maximum bandwidth we can cover
    int missingPackageCount = 0; // Counts the number of missing packages added
    std::vector<int> addedPackages; // Stores the missing packages added
    
    for (int package : bandwidthPackages) {
        if (package > currentCoverage + 1) {
            for (int i = currentCoverage + 1; i < package; i++) {
                if (existingPackages.find(i) == existingPackages.end()) {
                    addedPackages.push_back(i);
                    missingPackageCount += 1;
                }
            }
        }
        currentCoverage = std::max(currentCoverage, package);
    }
    
    if (currentCoverage < maxBandwidth) {
        for (int i = currentCoverage + 1; i <= maxBandwidth; i++) {
            if (existingPackages.find(i) == existingPackages.end()) {
                addedPackages.push_back(i);
                missingPackageCount += 1;
            }
        }
    }
    
    return {missingPackageCount, addedPackages};
}
