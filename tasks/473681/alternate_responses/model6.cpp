#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

std::pair<int, std::vector<int>> findMissingBandwidthPackages(std::vector<int>& bandwidthPackages, int maxBandwidth) {
    std::set<int> uniquePackages(bandwidthPackages.begin(), bandwidthPackages.end()); // remove duplicates
    std::vector<int> sortedPackages(uniquePackages.begin(), uniquePackages.end()); // sort packages
    std::sort(sortedPackages.begin(), sortedPackages.end());

    int currentCoverage = 0; // Tracks the current maximum bandwidth we can cover
    std::vector<int> addedPackages; // Stores the missing packages added

    for (int package : sortedPackages) {
        if (package > currentCoverage + 1) {
            // add missing packages
            for (int i = currentCoverage + 1; i < package; i++) {
                addedPackages.push_back(i);
            }
        }
        currentCoverage = std::max(currentCoverage, package);
    }

    // add remaining missing packages
    for (int i = currentCoverage + 1; i <= maxBandwidth; i++) {
        addedPackages.push_back(i);
    }

    return {addedPackages.size(), addedPackages};
}
