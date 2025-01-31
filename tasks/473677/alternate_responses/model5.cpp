#include <vector>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <cmath>

int calculateMaxChannelQuality(std::vector<int> packetSizes, int numChannels) {
    // Handle edge cases
    if (packetSizes.empty() || numChannels == 0) {
        return 0;
    }
    if (packetSizes.size() < numChannels) {
        throw std::invalid_argument("Number of packets must be greater than or equal to number of channels");
    }

    // Sort packets in descending order
    std::sort(packetSizes.begin(), packetSizes.end(), std::greater<int>());

    // Create vectors for each channel
    std::vector<std::vector<int>> channels(numChannels);

    // Distribute packets using greedy approach
    for (size_t i = 0; i < packetSizes.size(); ++i) {
        // Add packet to channel with least number of packets
        auto minChannel = std::min_element(channels.begin(), channels.end(),
            [](const std::vector<int>& a, const std::vector<int>& b) {
                return a.size() < b.size();
            });
        minChannel->push_back(packetSizes[i]);
    }

    // Calculate sum of medians
    double totalQuality = 0;
    for (auto& channel : channels) {
        std::sort(channel.begin(), channel.end());
        size_t size = channel.size();
        if (size % 2 == 0) {
            totalQuality += (channel[size/2 - 1] + channel[size/2]) / 2.0;
        } else {
            totalQuality += channel[size/2];
        }
    }

    // Round to nearest integer and return
    return static_cast<int>(std::round(totalQuality));
}