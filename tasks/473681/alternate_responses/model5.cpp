#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>

std::pair<int, std::vector<int>> findMissingBandwidthPackages(std::vector<int>& bandwidthPackages, int maxBandwidth) {
    // Sort the input packages for efficient processing
    std::sort(bandwidthPackages.begin(), bandwidthPackages.end());
    
    // Vector to track achievable sums
    std::vector<bool> achievableSums(maxBandwidth + 1, false);
    achievableSums[0] = true;
    
    // Calculate all possible sums using initial packages
    for (int package : bandwidthPackages) {
        if (package > maxBandwidth) continue;
        
        // Update achievable sums from right to left to avoid counting same package multiple times
        for (int j = maxBandwidth; j >= package; j--) {
            if (achievableSums[j - package]) {
                achievableSums[j] = true;
            }
        }
    }
    
    std::vector<int> addedPackages;
    
    // Find missing values and add necessary packages
    for (int target = 1; target <= maxBandwidth; target++) {
        if (!achievableSums[target]) {
            // Add this value as a new package
            addedPackages.push_back(target);
            
            // Update achievable sums with the new package
            for (int j = maxBandwidth; j >= target; j--) {
                if (achievableSums[j - target]) {
                    achievableSums[j] = true;
                }
            }
        }
    }
    
    return {addedPackages.size(), addedPackages};
}