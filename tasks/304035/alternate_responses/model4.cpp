#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <cmath>
#include <algorithm>
#include <numeric>

void applyBilateralFilter(int** inputImage, int width, int height, int kernelSize, 
                         float sigmaSpatial, float sigmaIntensity) {
    // Convert input to vector for safer memory management
    std::vector<std::vector<int>> image(height, std::vector<int>(width));
    std::vector<std::vector<int>> result(height, std::vector<int>(width));
    
    // Copy input to vector
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            image[i][j] = inputImage[i][j];
        }
    }

    // Handle special cases
    if (width <= 1 || height <= 1 || kernelSize <= 1) {
        return;  // Return original image for tiny images
    }

    // Adjust kernel size if larger than image
    kernelSize = std::min(kernelSize, std::min(width, height));
    int halfSize = kernelSize / 2;

    // Pre-compute spatial Gaussian weights
    std::vector<std::vector<float>> spatialKernel(kernelSize, std::vector<float>(kernelSize));
    float spatialNorm = 2.0f * sigmaSpatial * sigmaSpatial;
    for (int i = -halfSize; i <= halfSize; ++i) {
        for (int j = -halfSize; j <= halfSize; ++j) {
            spatialKernel[i + halfSize][j + halfSize] = 
                std::exp(-(i * i + j * j) / spatialNorm);
        }
    }

    // Determine number of threads based on hardware
    unsigned int threadCount = std::thread::hardware_concurrency();
    threadCount = std::max(1u, std::min(threadCount, static_cast<unsigned int>(height)));

    std::vector<std::thread> threads;
    std::mutex resultMutex;

    // Thread worker function
    auto processRows = [&](int startRow, int endRow) {
        for (int i = startRow; i < endRow; ++i) {
            for (int j = 0; j < width; ++j) {
                float sumWeights = 0.0f;
                float sumFiltered = 0.0f;

                // Process kernel
                for (int ki = -halfSize; ki <= halfSize; ++ki) {
                    for (int kj = -halfSize; kj <= halfSize; ++kj) {
                        int ni = std::clamp(i + ki, 0, height - 1);
                        int nj = std::clamp(j + kj, 0, width - 1);

                        float spatialWeight = spatialKernel[ki + halfSize][kj + halfSize];
                        
                        // Intensity weight
                        float intensityDiff = static_cast<float>(image[i][j] - image[ni][nj]);
                        float intensityWeight = std::exp(
                            -(intensityDiff * intensityDiff) / 
                            (2.0f * sigmaIntensity * sigmaIntensity)
                        );

                        float weight = spatialWeight * intensityWeight;
                        sumWeights += weight;
                        sumFiltered += weight * image[ni][nj];
                    }
                }

                // Compute filtered value
                result[i][j] = static_cast<int>(
                    std::round(sumWeights > 0.0f ? sumFiltered / sumWeights : image[i][j])
                );
            }
        }
    };

    // Launch threads
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
