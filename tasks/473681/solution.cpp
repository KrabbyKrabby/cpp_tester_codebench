#include <iostream>
#include <vector>
#include <algorithm>

std::pair<int, std::vector<int>> findMissingBandwidthPackages(std::vector<int>& bandwidthPackages, int maxBandwidth) {
    
    std::sort(bandwidthPackages.begin(), bandwidthPackages.end());
    
    int currentCoverage = 0; // Tracks the current maximum bandwidth that can be covered
    int missingPackageCount = 0; // Tracks the number of missing packages added
    std::vector<int> addedPackages; // Tracks the missing packages added
    int index = 0; // Tracks the index in the bandwidthPackages vector
    
    while (currentCoverage < maxBandwidth) {
        if (index < bandwidthPackages.size() && bandwidthPackages[index] <= currentCoverage + 1) {
            // Use the current bandwidth package if it can extend the range
            currentCoverage += bandwidthPackages[index];
            index++;
        } else {
            // Add a new missing package
            int newPackage = currentCoverage + 1;
            addedPackages.push_back(newPackage);
            missingPackageCount++;
            currentCoverage += newPackage;
        }
    }
    return {missingPackageCount, addedPackages};
}