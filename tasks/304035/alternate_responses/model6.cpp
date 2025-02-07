#include <iostream>
#include <cmath>
#include <vector>
#include <thread>
#include <mutex>

// Function to apply Bilateral filter on the image
void applyBilateralFilter(std::vector<std::vector<int>>& image, int width, int height, int kernelSize, float sigmaSpatial, float sigmaIntensity) {
    int halfSize = kernelSize / 2;

    // Handle single-pixel images
    if (width == 1 && height == 1) {
        return;
    }

    // Ensure kernel size doesn't exceed image dimensions
    if (kernelSize > width || kernelSize > height) {
        kernelSize = std::min(width, height);
        halfSize = kernelSize / 2;
    }

    // Parallelize computation using std::thread
    int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    std::mutex mtx;

    // Divide the image into sections for each thread
    int sectionHeight = height / numThreads;

    auto processSection = [&](int startRow, int endRow) {
        for (int i = startRow; i < endRow; ++i) {
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

                // Update the image in place
                std::lock_guard<std::mutex> lock(mtx);
                image[i][j] = static_cast<int>(sumFiltered / sumWeights);
            }
        }
    };

    // Create and launch threads
    for (int i = 0; i < numThreads; ++i) {
        int startRow = i * sectionHeight;
        int endRow = (i == numThreads - 1) ? height : (i + 1) * sectionHeight;
        threads.emplace_back(processSection, startRow, endRow);
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }
}
