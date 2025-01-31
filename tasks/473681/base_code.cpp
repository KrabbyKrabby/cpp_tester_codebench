#include <iostream>
#include <vector>

std::pair<int, std::vector<int>> findMissingBandwidthPackages(std::vector<int>& bandwidthPackages, int maxBandwidth) {
    int currentCoverage = 0; // Tracks the current maximum bandwidth we can cover
    int missingPackageCount = 0; // Counts the number of missing packages added
    std::vector<int> addedPackages; // Stores the missing packages added
    int index = 0; // Tracks the index in the bandwidthPackages array
    
    while (currentCoverage != maxBandwidth && index < bandwidthPackages.size()) {
        if (bandwidthPackages[index] == currentCoverage + 1) {
            currentCoverage += 1;
            index++;
            continue;
        } else {
            addedPackages.push_back(currentCoverage + 1);
            missingPackageCount += 1;
            currentCoverage += 1;
        }
    }
    if (currentCoverage != maxBandwidth) {
        for (int i = currentCoverage + 1; i <= maxBandwidth; i++) {
            addedPackages.push_back(i);
            missingPackageCount += 1;
        }
    }    
    
    return {missingPackageCount, addedPackages};
}