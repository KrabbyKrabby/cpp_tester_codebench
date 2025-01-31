#include <vector>
#include <iostream>
#include <stdexcept>
#include <algorithm>

int calculateMaxChannelQuality(std::vector<int> packetSizes, int numChannels) {
    if (packetSizes.empty() || numChannels == 0) {
        return 0;
    }

    if (packetSizes.size() < numChannels) {
        throw std::invalid_argument("Number of packets is less than the number of channels");
    }

    std::sort(packetSizes.begin(), packetSizes.end());

    int totalQuality = 0;
    for (int i = 0; i < numChannels; ++i) {
        int start = i * packetSizes.size() / numChannels;
        int end = (i + 1) * packetSizes.size() / numChannels;

        if (end - start == 1) {
            totalQuality += packetSizes[start];
        } else {
            int mid1 = packetSizes[start + (end - start) / 2 - 1];
            int mid2 = packetSizes[start + (end - start) / 2];
            totalQuality += (mid1 + mid2) / 2.0;
        }
    }

    return std::round(totalQuality);
}
