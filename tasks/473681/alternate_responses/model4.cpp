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
    
    // Process each package to find all possible sums
    for (int package : bandwidthPackages) {
        // Skip packages larger than maxBandwidth
        if (package > maxBandwidth) continue;
        
        // Update achievable sums from right to left to avoid counting package multiple times
        for (int sum = maxBandwidth; sum >= package; sum--) {
            if (achievableSums[sum - package]) {
                achievableSums[sum] = true;
            }
        }
    }
    
    std::vector<int> missingPackages;
    
    // Find missing numbers and add minimum required packages
    for (int i = 1; i <= maxBandwidth; i++) {
        if (!achievableSums[i]) {
            missingPackages.push_back(i);
            
            // Update achievable sums with the new package
            for (int sum = i; sum <= maxBandwidth; sum++) {
                if (achievableSums[sum - i]) {
                    achievableSums[sum] = true;
                }
            }
        }
    }
    
    return {missingPackages.size(), missingPackages};
}
