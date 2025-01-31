#include <vector>
#include <iostream>
#include <algorithm>
#include <stdexcept>

int calculateMaxChannelQuality(std::vector<int> packetSizes, int numChannels) {
    // Check if the packet vector is empty or the number of channels is 0
    if (packetSizes.empty() || numChannels == 0) {
        return 0;
    }

    // Check if the number of packets is less than the number of channels
    if (packetSizes.size() < numChannels) {
        throw std::invalid_argument("Number of packets is less than the number of channels");
    }

    // Sort the packet sizes in ascending order
    std::sort(packetSizes.begin(), packetSizes.end());

    // Calculate the total quality
    double totalQuality = 0.0;
    for (int i = 0; i < numChannels; ++i) {
        int start = i * packetSizes.size() / numChannels;
        int end = (i + 1) * packetSizes.size() / numChannels;

        // Calculate the median for the current channel
        double median;
        int count = end - start;
        if (count % 2 == 0) {
            median = (packetSizes[start + count / 2 - 1] + packetSizes[start + count / 2]) / 2.0;
        } else {
            median = packetSizes[start + count / 2];
        }

        totalQuality += median;
    }

    // Round the total quality to the nearest integer
    return static_cast<int>(totalQuality + 0.5);
}
