#include <iostream>
#include <vector>
#include <algorithm>

std::pair<int, std::vector<int>> findMissingBandwidthPackages(std::vector<int>& bandwidthPackages, int maxBandwidth) {
    std::sort(bandwidthPackages.begin(), bandwidthPackages.end()); // Sort the bandwidth packages in ascending order
    std::vector<bool> covered(maxBandwidth + 1, false); // Tracks the covered bandwidths
    covered[0] = true; // Base case: 0 bandwidth is always covered

    // Mark the bandwidths that can be covered using the initial packages
    for (int package : bandwidthPackages) {
        for (int i = maxBandwidth; i >= package; i--) {
            if (covered[i - package]) {
                covered[i] = true;
            }
        }
    }

    int missingPackageCount = 0; // Counts the number of missing packages added
    std::vector<int> addedPackages; // Stores the missing packages added

    // Find the minimum number of packages required to cover the entire range
    for (int i = 1; i <= maxBandwidth; i++) {
        if (!covered[i]) {
            addedPackages.push_back(i);
            missingPackageCount += 1;

            // Update the covered bandwidths
            for (int j = i; j <= maxBandwidth; j++) {
                if (covered[j - i]) {
                    covered[j] = true;
                }
            }
        }
    }

    return {missingPackageCount, addedPackages};
}
