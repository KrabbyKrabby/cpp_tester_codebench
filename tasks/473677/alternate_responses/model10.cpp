#include <vector>
#include <iostream>
#include <algorithm>
#include <stdexcept>

int calculateMaxChannelQuality(std::vector<int> packetSizes, int numChannels) {
    if (numChannels == 0 || packetSizes.empty()) {
        return 0;
    }

    if (packetSizes.size() < numChannels) {
        throw std::invalid_argument("Number of packets is less than the number of channels.");
    }

    std::sort(packetSizes.begin(), packetSizes.end(), std::greater<int>());

    double totalQuality = 0;

    for (int i = 0; i < numChannels; i++) {
        int channelSize = packetSizes.size() / numChannels;
        if (i < packetSizes.size() % numChannels) {
            channelSize++;
        }

        int start = i * (packetSizes.size() / numChannels) + std::min(i, packetSizes.size() % numChannels);
        int end = start + channelSize;

        if (channelSize % 2 == 0) {
            totalQuality += (packetSizes[start + channelSize / 2 - 1] + packetSizes[start + channelSize / 2]) / 2.0;
        } else {
            totalQuality += packetSizes[start + channelSize / 2];
        }
    }

    return static_cast<int>(totalQuality + 0.5);
}
