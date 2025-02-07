#include <iostream>
#include <cmath>
#include <vector>
#include <thread>
#include <mutex>

void applyBilateralFilter(int** image, int width, int height, int kernelSize, float sigmaSpatial, float sigmaIntensity) {
    // Handle edge cases
    if (width == 1 || height == 1) {
        return; // Single-pixel images remain unchanged
    }

    if (kernelSize > width || kernelSize > height) {
        kernelSize = std::min(width, height); // Ensure kernel size does not exceed image dimensions
    }

    int halfSize = kernelSize / 2;

    // Create a vector to store the filtered image
    std::vector<std::vector<int>> result(height, std::vector<int>(width));

    // Function to apply the bilateral filter to a single pixel
    auto applyFilterToPixel = [&](int i, int j) {
        float sumWeights = 0.0f;
        float sumFiltered = 0.0f;

        for (int ki = -halfSize; ki <= halfSize; ++ki) {
            for (int kj = -halfSize; kj <= halfSize; ++kj) {
                int neighborX = i + ki;
                int neighborY = j + kj;

                // Check if neighbor is within image bounds
                if (neighborX < 0 || neighborX >= height || neighborY < 0 || neighborY >= width) {
                    continue;
                }

                // Spatial Gaussian
                float spatialWeight = exp(-(ki * ki + kj * kj) / (2 * sigmaSpatial * sigmaSpatial));

                // Intensity Gaussian
                float intensityWeight = exp(-((image[i][j] - image[neighborX][neighborY]) * (image[i][j] - image[neighborX][neighborY])) / (2 * sigmaIntensity * sigmaIntensity));

                // Combined weight
                float weight = spatialWeight * intensityWeight;

                sumWeights += weight;
                sumFiltered += weight * image[neighborX][neighborY];
            }
        }

        result[i][j] = static_cast<int>(sumFiltered / sumWeights);
    };

    // Apply the filter to each pixel in parallel
    int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    int chunkSize = height / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int startRow = i * chunkSize;
        int endRow = (i == numThreads - 1) ? height : (startRow + chunkSize);

        threads.emplace_back([&, startRow, endRow]() {
            for (int row = startRow; row < endRow; ++row) {
                for (int col = halfSize; col < width - halfSize; ++col) {
                    applyFilterToPixel(row, col);
                }
            }
        });
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }

    // Copy the result back into the original image
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            image[i][j] = result[i][j];
        }
    }
}
