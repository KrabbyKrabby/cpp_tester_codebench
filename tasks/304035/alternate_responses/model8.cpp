#include <iostream>
#include <cmath>
#include <vector>
#include <thread>
#include <mutex>

// Function to apply Bilateral filter on the image
void applyBilateralFilter(int** image, int width, int height, int kernelSize, float sigmaSpatial, float sigmaIntensity) {
    int halfSize = kernelSize / 2;

    // Check if the kernel size is larger than the image dimensions
    if (kernelSize > width || kernelSize > height) {
        kernelSize = std::min(width, height);
        halfSize = kernelSize / 2;
    }

    // Convert raw pointer to vector
    std::vector<std::vector<int>> imageData(height, std::vector<int>(width));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            imageData[i][j] = image[i][j];
        }
    }

    // Function to calculate bilateral filter for a single pixel
    auto calculateBilateralFilter = [&](int i, int j) {
        float sumWeights = 0.0f;
        float sumFiltered = 0.0f;

        for (int ki = -halfSize; ki <= halfSize; ++ki) {
            for (int kj = -halfSize; kj <= halfSize; ++kj) {
                int neighborX = i + ki;
                int neighborY = j + kj;

                // Check if neighbor is within image boundaries
                if (neighborX < 0 || neighborX >= height || neighborY < 0 || neighborY >= width) {
                    continue;
                }

                // Spatial Gaussian
                float spatialWeight = exp(-(ki * ki + kj * kj) / (2 * sigmaSpatial * sigmaSpatial));

                // Intensity Gaussian
                float intensityWeight = exp(-((imageData[i][j] - imageData[neighborX][neighborY]) * (imageData[i][j] - imageData[neighborX][neighborY])) / (2 * sigmaIntensity * sigmaIntensity));

                // Combined weight
                float weight = spatialWeight * intensityWeight;

                sumWeights += weight;
                sumFiltered += weight * imageData[neighborX][neighborY];
            }
        }

        // Update pixel value
        imageData[i][j] = static_cast<int>(sumFiltered / sumWeights);
    };

    // Apply bilateral filter in parallel
    int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    int chunkSize = height / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int start = i * chunkSize;
        int end = (i == numThreads - 1) ? height : (i + 1) * chunkSize;

        threads.emplace_back([&, start, end]() {
            for (int i = start; i < end; ++i) {
                for (int j = 0; j < width; ++j) {
                    calculateBilateralFilter(i, j);
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
            image[i][j] = imageData[i][j];
        }
    }
}
