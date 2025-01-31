#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <iostream>

int calculateMaxChannelQuality(std::vector<int> packetSizes, int numChannels) {
    // Sort packet sizes in ascending order
    std::sort(packetSizes.begin(), packetSizes.end());

    // If packets are fewer or equal to channels, return the sum of all packets
    if (packetSizes.size() <= numChannels) {
        return std::accumulate(packetSizes.begin(), packetSizes.end(), 0);
    }

    float totalChannelQuality = 0;
    int startIdx = packetSizes.size() - numChannels; 

    // Allocate the largest packets to channels as initial medians
    for (int i = startIdx; i < packetSizes.size(); i++) {
        totalChannelQuality += packetSizes[i];
    }

    int remainingPackets = startIdx;
    int extraPairs = remainingPackets / numChannels;

    // Add smaller packets to channels while maintaining quality
    for (int i = 1; i <= extraPairs; i++) {
        for (int j = 0; j < numChannels; j++) {
            int leftIdx = (i - 1) * numChannels + j;
            int rightIdx = remainingPackets - i * numChannels + j;
            totalChannelQuality += (float)(packetSizes[leftIdx] + packetSizes[rightIdx]) / 2.0;
        }
    }

    // Return the total quality rounded to the nearest integer
    return std::round(totalChannelQuality);
}
