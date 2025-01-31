#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <iostream>
#include <stdexcept>

int calculateMaxChannelQuality(std::vector<int> packetSizes, int numChannels) {
    if (packetSizes.empty() || numChannels == 0) {
        return 0;
    }

    // If the number of packets is less than the number of channels, throw an exception
    if (packetSizes.size() < numChannels) {
        throw std::invalid_argument("Number of packets cannot be less than the number of channels.");
    }

    // Sort packets in descending order
    std::sort(packetSizes.rbegin(), packetSizes.rend());

    // If packets are fewer or equal to channels, return the sum of all packets
    if (packetSizes.size() <= numChannels) {
        return std::accumulate(packetSizes.begin(), packetSizes.end(), 0);
    }

    float totalChannelQuality = 0;
    int startIdx = packetSizes.size() - numChannels;

    // Allocate the largest packets to channels as initial medians
    for (int i = 0; i < numChannels - 1; i++) {
        totalChannelQuality += packetSizes[i];
    }

    int remainingStartIdx = numChannels - 1;
    int remainingCount = packetSizes.size() - remainingStartIdx;

    // Add smaller remaining packets to the last channel
    if (remainingCount % 2 == 0) {
        int mid1 = remainingStartIdx + (remainingCount / 2) - 1;
        int mid2 = remainingStartIdx + (remainingCount / 2);
        totalChannelQuality += (packetSizes[mid1] + packetSizes[mid2]) / 2.0;
    } else {
        int mid = remainingStartIdx + (remainingCount / 2);
        totalChannelQuality += packetSizes[mid];
    }

    // Return the total quality rounded to the nearest integer
    return std::round(totalChannelQuality);
}