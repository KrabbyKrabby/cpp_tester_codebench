#include <iostream>
#include <cmath>
#include <vector>
#include <thread>
#include <mutex>

// Function to apply Bilateral filter on the image
void applyBilateralFilter(int** image, int width, int height, int kernelSize, float sigmaSpatial, float sigmaIntensity) {
    int halfSize = kernelSize / 2;

    // Handle edge cases
    if (width == 1 || height == 1) {
        return;  // Single-pixel image, no changes needed
    }
    if (kernelSize > width || kernelSize > height) {
        return;  // Kernel size larger than image dimensions, no changes needed
    }

    // Create a vector to store the filtered image
    std::vector<std::vector<int>> result(height, std::vector<int>(width));

    // Define the number of threads to use
    int numThreads = std::thread::hardware_concurrency();

    // Function to apply the bilateral filter to a range of rows
    auto applyFilter = [&](int startRow, int endRow) {
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

                result[i][j] = static_cast<int>(sumFiltered / sumWeights);
            }
        }
    };

    // Calculate the number of rows to process per thread
    int rowsPerThread = height / numThreads;

    // Create and start the threads
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        int startRow = i * rowsPerThread;
        int endRow = (i == numThreads - 1) ? height : (i + 1) * rowsPerThread;
        threads.emplace_back(applyFilter, startRow, endRow);
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
