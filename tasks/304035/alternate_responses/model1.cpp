#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <memory>

void applyBilateralFilter(int** inputImage, int width, int height, int kernelSize, 
                         float sigmaSpatial, float sigmaIntensity) {
    // Convert input to vector for safer memory management
    std::vector<std::vector<int>> image(height, std::vector<int>(width));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            image[i][j] = inputImage[i][j];
        }
    }

    // Handle special cases
    if (width <= 1 || height <= 1 || kernelSize <= 1) {
        return;
    }

    // Adjust kernel size if larger than image dimensions
    kernelSize = std::min(kernelSize, std::min(width, height));
    int halfSize = kernelSize / 2;

    // Pre-compute spatial weights for optimization
    std::vector<std::vector<float>> spatialWeights(kernelSize, std::vector<float>(kernelSize));
    for (int i = -halfSize; i <= halfSize; ++i) {
        for (int j = -halfSize; j <= halfSize; ++j) {
            spatialWeights[i + halfSize][j + halfSize] = 
                std::exp(-(i * i + j * j) / (2.0f * sigmaSpatial * sigmaSpatial));
        }
    }

    // Create result buffer
    std::vector<std::vector<int>> result(height, std::vector<int>(width));

    // Determine number of threads based on hardware
    unsigned int threadCount = std::thread::hardware_concurrency();
    threadCount = std::max(1u, std::min(threadCount, static_cast<unsigned int>(height)));

    std::vector<std::thread> threads;
    std::mutex mutex;

    // Thread worker function
    auto processRows = [&](int startRow, int endRow) {
        for (int i = startRow; i < endRow; ++i) {
            for (int j = 0; j < width; ++j) {
                if (i < halfSize || i >= height - halfSize || 
                    j < halfSize || j >= width - halfSize) {
                    result[i][j] = image[i][j];
                    continue;
                }

                float sumWeights = 0.0f;
                float sumFiltered = 0.0f;
                const int centerPixel = image[i][j];

                for (int ki = -halfSize; ki <= halfSize; ++ki) {
                    for (int kj = -halfSize; kj <= halfSize; ++kj) {
                        int neighborX = i + ki;
                        int neighborY = j + kj;
                        
                        const float spatialWeight = spatialWeights[ki + halfSize][kj + halfSize];
                        const int neighborPixel = image[neighborX][neighborY];
                        
                        // Intensity weight calculation
                        const float intensityDiff = centerPixel - neighborPixel;
                        const float intensityWeight = 
                            std::exp(-(intensityDiff * intensityDiff) / 
                                   (2.0f * sigmaIntensity * sigmaIntensity));

                        const float weight = spatialWeight * intensityWeight;
                        sumWeights += weight;
                        sumFiltered += weight * neighborPixel;
                    }
                }

                result[i][j] = static_cast<int>(std::round(sumFiltered / sumWeights));
            }
        }
    };

    // Distribute work among threads
    int rowsPerThread = height / threadCount;
    for (unsigned int t = 0; t < threadCount; ++t) {
        int startRow = t * rowsPerThread;
        int endRow = (t == threadCount - 1) ? height : (t + 1) * rowsPerThread;
        threads.emplace_back(processRows, startRow, endRow);
    }

    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }

    // Copy result back to input array
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            inputImage[i][j] = result[i][j];
        }
    }
}
