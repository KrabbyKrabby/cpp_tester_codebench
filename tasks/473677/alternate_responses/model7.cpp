#include <vector>
#include <iostream>
#include <algorithm>
#include <stdexcept>

int calculateMaxChannelQuality(std::vector<int> packetSizes, int numChannels) {
    // Check for invalid arguments
    if (packetSizes.size() < numChannels) {
        throw std::invalid_argument("Number of packets is less than the number of channels");
    }
    if (packetSizes.empty() || numChannels == 0) {
        return 0;
    }

    // Sort the packet sizes in descending order
    std::sort(packetSizes.rbegin(), packetSizes.rend());

    // Calculate the total quality
    double totalQuality = 0.0;
    for (int i = 0; i < numChannels; ++i) {
        int start = i * (packetSizes.size() / numChannels);
        int end = (i + 1) * (packetSizes.size() / numChannels);
        if (i == numChannels - 1) {
            end = packetSizes.size();
        }

        std::vector<int> channelPackets(packetSizes.begin() + start, packetSizes.begin() + end);
        std::sort(channelPackets.begin(), channelPackets.end());

        double median;
        if (channelPackets.size() % 2 == 0) {
            median = (channelPackets[channelPackets.size() / 2 - 1] + channelPackets[channelPackets.size() / 2]) / 2.0;
        } else {
            median = channelPackets[channelPackets.size() / 2];
        }

        totalQuality += median;
    }

    // Round the total quality to the nearest integer
    return static_cast<int>(totalQuality + 0.5);
}
