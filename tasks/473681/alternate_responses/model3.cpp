#include <iostream>
#include <vector>
#include <algorithm>

std::pair<int, std::vector<int>> findMissingBandwidthPackages(std::vector<int>& bandwidthPackages, int maxBandwidth) {
    // Sort the input packages
    std::sort(bandwidthPackages.begin(), bandwidthPackages.end());
    
    // Vector to track achievable sums
    std::vector<bool> achievableSums(maxBandwidth + 1, false);
    achievableSums[0] = true;
    
    // Calculate achievable sums with initial packages
    for (int package : bandwidthPackages) {
        if (package > maxBandwidth) continue;
        // Need to go backwards to avoid using the same package multiple times
        for (int j = maxBandwidth; j >= package; j--) {
            if (achievableSums[j - package]) {
                achievableSums[j] = true;
            }
        }
    }
    
    std::vector<int> missingPackages;
    
    // Check each value from 1 to maxBandwidth
    for (int i = 1; i <= maxBandwidth; i++) {
        if (!achievableSums[i]) {
            missingPackages.push_back(i);
            // Update achievable sums with the new package
            for (int j = maxBandwidth; j >= i; j--) {
                if (achievableSums[j - i]) {
                    achievableSums[j] = true;
                }
            }
        }
    }
    
    return {missingPackages.size(), missingPackages};
}
