#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <cmath>
#include <algorithm>
#include <numeric>

void applyBilateralFilter(int** image, int width, int height, int kernelSize, float sigmaSpatial, float sigmaIntensity) {
    // Convert input image to vector for safer memory management
    std::vector<std::vector<int>> img(height, std::vector<int>(width));
    std::vector<std::vector<int>> result(height, std::vector<int>(width));
    
    // Copy input image to vector
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            img[i][j] = image[i][j];
        }
    }

    // Handle single pixel or very small images
    if (width <= 1 || height <= 1 || kernelSize <= 1) {
        return;
    }

    // Adjust kernel size if larger than image dimensions
    kernelSize = std::min(kernelSize, std::min(width, height));
    int halfSize = kernelSize / 2;

    // Precompute spatial weights for optimization
    std::vector<std::vector<float>> spatialWeights(kernelSize, std::vector<float>(kernelSize));
    for (int i = -halfSize; i <= halfSize; ++i) {
        for (int j = -halfSize; j <= halfSize; ++j) {
            spatialWeights[i + halfSize][j + halfSize] = 
                std::exp(-(i * i + j * j) / (2.0f * sigmaSpatial * sigmaSpatial));
        }
    }

    // Determine number of threads based on hardware
    unsigned int threadCount = std::thread::hardware_concurrency();
    threadCount = std::min(threadCount, static_cast<unsigned int>(height - 2 * halfSize));
    std::vector<std::thread> threads;
    std::mutex mutex;

    // Lambda function for processing image chunks
    auto processChunk = [&](int startRow, int endRow) {
        for (int i = startRow; i < endRow; ++i) {
            for (int j = halfSize; j < width - halfSize; ++j) {
                float sumWeights = 0.0f;
                float sumFiltered = 0.0f;
                const int centerPixel = img[i][j];

                // Process kernel
                for (int ki = -halfSize; ki <= halfSize; ++ki) {
                    for (int kj = -halfSize; kj <= halfSize; ++kj) {
                        int neighborX = i + ki;
                        int neighborY = j + kj;

                        // Boundary check
                        if (neighborX < 0 || neighborX >= height || neighborY < 0 || neighborY >= width) {
                            continue;
                        }

                        float spatialWeight = spatialWeights[ki + halfSize][kj + halfSize];
                        
                        // Intensity weight calculation
                        int intensityDiff = centerPixel - img[neighborX][neighborY];
                        float intensityWeight = std::exp(
                            -(intensityDiff * intensityDiff) / 
                            (2.0f * sigmaIntensity * sigmaIntensity)
                        );

                        float weight = spatialWeight * intensityWeight;
                        sumWeights += weight;
                        sumFiltered += weight * img[neighborX][neighborY];
                    }
                }

                // Compute filtered value
                result[i][j] = static_cast<int>(
                    sumWeights > 0.0f ? sumFiltered / sumWeights : centerPixel
                );
            }
        }
    };

    // Create and launch threads
    int rowsPerThread = (height - 2 * halfSize) / threadCount;
    for (unsigned int t = 0; t < threadCount; ++t) {
        int startRow = halfSize + t * rowsPerThread;
        int endRow = (t == threadCount - 1) ? 
                     height - halfSize : 
                     halfSize + (t + 1) * rowsPerThread;
        threads.emplace_back(processChunk, startRow, endRow);
    }

    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }

    // Copy border pixels
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (i < halfSize || i >= height - halfSize || 
                j < halfSize || j >= width - halfSize) {
                result[i][j] = img[i][j];
            }
        }
    }

    // Copy result back to input image
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            image[i][j] = result[i][j];
        }
    }
}
