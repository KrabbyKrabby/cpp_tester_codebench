#include <vector>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <cmath>

int calculateMaxChannelQuality(std::vector<int> packetSizes, int numChannels) {
    // Handle edge cases
    if (packetSizes.empty() || numChannels == 0) {
        return 0;
    }
    
    if (packetSizes.size() < numChannels) {
        throw std::invalid_argument("Number of packets must be greater than or equal to number of channels");
    }

    // Sort packets in descending order
    std::sort(packetSizes.begin(), packetSizes.end(), std::greater<int>());
    
    // Initialize channels
    std::vector<std::vector<int>> channels(numChannels);
    
    // Distribute packets to maximize median values
    for (size_t i = 0; i < packetSizes.size(); ++i) {
        channels[i % numChannels].push_back(packetSizes[i]);
    }
    
    // Calculate sum of medians
    double totalQuality = 0;
    for (const auto& channel : channels) {
        size_t size = channel.size();
        if (size % 2 == 0) {
            totalQuality += (channel[size/2 - 1] + channel[size/2]) / 2.0;
        } else {
            totalQuality += channel[size/2];
        }
    }
    
    // Round to nearest integer
    return static_cast<int>(std::round(totalQuality));
}