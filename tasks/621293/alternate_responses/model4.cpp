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

    // If packets are fewer than channels, return sum of all packets
    if (packetSizes.size() <= numChannels) {
        return std::accumulate(packetSizes.begin(), packetSizes.end(), 0);
    }

    // Calculate packets per channel
    int packetsPerChannel = packetSizes.size() / numChannels;
    float totalQuality = 0;

    // Distribute packets and calculate median for each channel
    for (int i = 0; i < numChannels; i++) {
        std::vector<int> channelPackets;
        int startIdx = i * packetsPerChannel;
        
        // Get packets for current channel
        for (int j = 0; j < packetsPerChannel; j++) {
            if (startIdx + j < packetSizes.size()) {
                channelPackets.push_back(packetSizes[startIdx + j]);
            }
        }

        // Handle remaining packets
        if (i < packetSizes.size() % numChannels) {
            channelPackets.push_back(packetSizes[numChannels * packetsPerChannel + i]);
        }

        // Calculate median for current channel
        float median;
        int size = channelPackets.size();
        if (size % 2 == 0) {
            median = (channelPackets[size/2 - 1] + channelPackets[size/2]) / 2.0f;
        } else {
            median = channelPackets[size/2];
        }
        
        totalQuality += median;
    }

    return std::round(totalQuality);
}
