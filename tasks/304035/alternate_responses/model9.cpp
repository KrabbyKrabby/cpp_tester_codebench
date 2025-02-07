#include <iostream>
#include <cmath>
#include <vector>
#include <thread>
#include <mutex>

// Function to apply Bilateral filter on the image
void applyBilateralFilter(int** image, int width, int height, int kernelSize, float sigmaSpatial, float sigmaIntensity) {
    int halfSize = kernelSize / 2;
    std::vector<std::vector<int>> result(height, std::vector<int>(width));

    // Handle single-pixel images (1x1)
    if (width == 1 && height == 1) {
        result[0][0] = image[0][0];
    } else {
        // Multithreading for parallel computation
        int numThreads = std::thread::hardware_concurrency();
        int chunkSize = height / numThreads;

        std::mutex mtx;

        auto processChunk = [&](int start, int end) {
            for (int i = start; i < end; ++i) {
                for (int j = halfSize; j < width - halfSize; ++j) {
                    float sumWeights = 0.0f;
                    float sumFiltered = 0.0f;

                    for (int ki = -halfSize; ki <= halfSize; ++ki) {
                        for (int kj = -halfSize; kj <= halfSize; ++kj) {
                            int neighborX = i + ki;
                            int neighborY = j + kj;

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

                    // Update result array with thread safety
                    std::lock_guard<std::mutex> lock(mtx);
                    result[i][j] = static_cast<int>(sumFiltered / sumWeights);
                }
            }
        };

        std::vector<std::thread> threads;
        for (int i = 0; i < numThreads; ++i) {
            int start = i * chunkSize;
            int end = (i == numThreads - 1) ? height : (start + chunkSize);
            threads.emplace_back(processChunk, start, end);
        }

        for (auto& thread : threads) {
            thread.join();
        }
    }

    // Copy the result back into the original image
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            image[i][j] = result[i][j];
        }
    }
}
