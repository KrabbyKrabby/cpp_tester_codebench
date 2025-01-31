#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <iostream>

int calculateMaxChannelQuality(std::vector<int> packetSizes, int numChannels) {
    if (packetSizes.empty() || numChannels <= 0) {
        return 0;
    }

    // Sort packets in descending order
    std::sort(packetSizes.rbegin(), packetSizes.rend());

    // If packets are fewer than channels, each packet forms its own median
    if (packetSizes.size() <= numChannels) {
        return std::accumulate(packetSizes.begin(), packetSizes.end(), 0);
    }

    // Create channels and distribute packets
    std::vector<std::vector<int>> channels(numChannels);
    
    // Distribute packets round-robin to maximize median values
    for (size_t i = 0; i < packetSizes.size(); i++) {
        channels[i % numChannels].push_back(packetSizes[i]);
    }

    // Calculate median sum
    double totalQuality = 0;
    for (const auto& channel : channels) {
        if (!channel.empty()) {
            size_t size = channel.size();
            if (size % 2 == 0) {
                // Even number of packets: average of two middle values
                totalQuality += (channel[size/2 - 1] + channel[size/2]) / 2.0;
            } else {
                // Odd number of packets: middle value
                totalQuality += channel[size/2];
            }
        }
    }

    // Round to nearest integer and return
    return std::round(totalQuality);
}
