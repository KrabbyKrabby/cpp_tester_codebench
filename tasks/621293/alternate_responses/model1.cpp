#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <iostream>

int calculateMaxChannelQuality(std::vector<int> packetSizes, int numChannels) {
    // If no packets, return 0
    if (packetSizes.empty()) {
        return 0;
    }

    // Sort packet sizes in ascending order
    std::sort(packetSizes.begin(), packetSizes.end());

    // If packets are fewer than channels, return sum of packets
    if (packetSizes.size() <= numChannels) {
        return std::accumulate(packetSizes.begin(), packetSizes.end(), 0);
    }

    int totalPackets = packetSizes.size();
    int packetsPerChannel = totalPackets / numChannels;
    float totalQuality = 0;

    // Distribute packets and calculate median for each channel
    for (int i = 0; i < numChannels; i++) {
        std::vector<int> channelPackets;
        int startIdx = i * packetsPerChannel;
        int endIdx = (i == numChannels - 1) ? totalPackets : (i + 1) * packetsPerChannel;

        // Collect packets for this channel
        for (int j = startIdx; j < endIdx; j++) {
            channelPackets.push_back(packetSizes[j]);
        }

        // Calculate median for this channel
        int medianIdx = channelPackets.size() / 2;
        if (channelPackets.size() % 2 == 0) {
            // Even number of packets, average the middle two
            totalQuality += (channelPackets[medianIdx - 1] + channelPackets[medianIdx]) / 2.0;
        } else {
            // Odd number of packets, take the middle one
            totalQuality += channelPackets[medianIdx];
        }
    }

    // Round the total quality to nearest integer
    return std::round(totalQuality);
}