#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <iostream>

int calculateMaxChannelQuality(std::vector<int> packetSizes, int numChannels) {
    // Sort packet sizes in ascending order
    std::sort(packetSizes.begin(), packetSizes.end());

    int totalChannelQuality = 0;
    int startIdx = packetSizes.size() - numChannels; 

    // Allocate the largest packets to channels as initial medians
    for (int i = startIdx; i < packetSizes.size(); i++) {
        totalChannelQuality += packetSizes[i];
    }

    int remainingPackets = startIdx;

    // Distribute remaining packets to channels while maintaining quality
    for (int i = 0; i < remainingPackets; i++) {
        totalChannelQuality += packetSizes[i];
    }

    // Calculate the median of each channel
    float median = (float)totalChannelQuality / numChannels;

    // Return the total quality rounded to the nearest integer
    return std::round(median * numChannels);
}
