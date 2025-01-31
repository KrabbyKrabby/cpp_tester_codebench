#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <iostream>

int calculateMaxChannelQuality(std::vector<int> packetSizes, int numChannels) {
    // Sort packet sizes in descending order
    std::sort(packetSizes.rbegin(), packetSizes.rend());

    // If packets are fewer or equal to channels, return the sum of all packets
    if (packetSizes.size() <= numChannels) {
        return std::accumulate(packetSizes.begin(), packetSizes.end(), 0);
    }

    // Divide the packet sizes into groups
    int groupSize = packetSizes.size() / numChannels;
    int extraPackets = packetSizes.size() % numChannels;

    float totalChannelQuality = 0;

    // Process the groups with an extra packet
    for (int i = 0; i < extraPackets; i++) {
        int startIdx = i * (groupSize + 1);
        int endIdx = startIdx + groupSize + 1;
        std::vector<int> group(packetSizes.begin() + startIdx, packetSizes.begin() + endIdx);
        std::sort(group.begin(), group.end());
        totalChannelQuality += group[group.size() / 2];
    }

    // Process the groups without an extra packet
    for (int i = extraPackets; i < numChannels; i++) {
        int startIdx = extraPackets * (groupSize + 1) + (i - extraPackets) * groupSize;
        int endIdx = startIdx + groupSize;
        std::vector<int> group(packetSizes.begin() + startIdx, packetSizes.begin() + endIdx);
        std::sort(group.begin(), group.end());
        totalChannelQuality += (group[group.size() / 2 - 1] + group[group.size() / 2]) / 2.0;
    }

    // Return the total quality rounded to the nearest integer
    return std::round(totalChannelQuality);
}
