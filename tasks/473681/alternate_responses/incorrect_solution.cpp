#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>

std::pair<int, std::vector<int>> findMissingBandwidthPackages(std::vector<int>& bandwidthPackages, int maxBandwidth) {
    // Sort the input packages
    std::sort(bandwidthPackages.begin(), bandwidthPackages.end());
    
    // Vector to track achievable sums
    std::vector<bool> achievableSums(maxBandwidth + 1, false);
    achievableSums[0] = true;  // Base case
    
    // Process each bandwidth package
    for (int package : bandwidthPackages) {
        if (package > maxBandwidth) continue;
        // Update achievable sums from right to left to avoid counting package multiple times
        for (int j = maxBandwidth; j >= 0; j--) {
            if (achievableSums[j] && j + package <= maxBandwidth) {
                achievableSums[j + package] = true;
            }
        }
    }
    
    std::vector<int> addedPackages;
    int missingPackageCount = 0;
    
    // Find gaps and add necessary packages
    for (int target = 1; target <= maxBandwidth; target++) {
        if (!achievableSums[target]) {
            // Add this number as a new package
            addedPackages.push_back(target);
            missingPackageCount++;
            
            // Update achievable sums with the new package
            for (int j = maxBandwidth; j >= 0; j--) {
                if (achievableSums[j] && j + target <= maxBandwidth) {
                    achievableSums[j + target] = true;
                }
            }
        }
    }
    
    return {missingPackageCount, addedPackages};
}