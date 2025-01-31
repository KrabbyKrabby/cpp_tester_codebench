#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <iostream>

int calculateMaxChannelQuality(std::vector<int> packetSizes, int numChannels) {
    // Sort packet sizes in ascending order
    std::sort(packetSizes.begin(), packetSizes.end());
    
    int totalPackets = packetSizes.size();
    
    // If packets are fewer than channels, return sum of packets
    if (totalPackets <= numChannels) {
        return std::accumulate(packetSizes.begin(), packetSizes.end(), 0);
    }
    
    // Calculate packets per channel
    int packetsPerChannel = totalPackets / numChannels;
    
    double totalQuality = 0;
    int currentIndex = 0;
    
    // Distribute packets and calculate median for each channel
    for (int i = 0; i < numChannels; i++) {
        std::vector<int> channelPackets;
        
        // Calculate number of packets for current channel
        int packetsForThisChannel = packetsPerChannel;
        if (i < totalPackets % numChannels) {
            packetsForThisChannel++;
        }
        
        // Collect packets for this channel
        for (int j = 0; j < packetsForThisChannel; j++) {
            channelPackets.push_back(packetSizes[currentIndex + j]);
        }
        
        // Calculate median for this channel
        int medianIndex = channelPackets.size() / 2;
        double median;
        if (channelPackets.size() % 2 == 0) {
            median = (channelPackets[medianIndex - 1] + channelPackets[medianIndex]) / 2.0;
        } else {
            median = channelPackets[medianIndex];
        }
        
        totalQuality += median;
        currentIndex += packetsForThisChannel;
    }
    
    // Round the final sum to nearest integer
    return std::round(totalQuality);
}
