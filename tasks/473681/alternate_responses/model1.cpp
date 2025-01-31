#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>

const int MAX_BANDWIDTH = 1000000;  // Adjust based on problem constraints

std::pair<int, std::vector<int>> findMissingBandwidthPackages(std::vector<int>& bandwidthPackages, int maxBandwidth) {
    // Sort the input packages
    std::sort(bandwidthPackages.begin(), bandwidthPackages.end());
    
    // Use bitset for memory efficiency
    std::bitset<MAX_BANDWIDTH + 1> achievableSums;
    achievableSums[0] = true;
    
    // Calculate achievable sums with current packages
    for (int package : bandwidthPackages) {
        // Skip if package is greater than maxBandwidth
        if (package > maxBandwidth) continue;
        
        // Update achievable sums from right to left to avoid counting package multiple times
        for (int j = maxBandwidth; j >= 0; j--) {
            if (achievableSums[j] && j + package <= maxBandwidth) {
                achievableSums[j + package] = true;
            }
        }
    }
    
    std::vector<int> missingPackages;
    
    // Check each required sum and add missing packages
    for (int target = 1; target <= maxBandwidth; target++) {
        if (!achievableSums[target]) {
            missingPackages.push_back(target);
            
            // Update achievable sums with the new package
            for (int j = maxBandwidth - target; j >= 0; j--) {
                if (achievableSums[j]) {
                    achievableSums[j + target] = true;
                }
            }
        }
    }
    
    return {missingPackages.size(), missingPackages};
}