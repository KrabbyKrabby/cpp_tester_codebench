#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <cmath>
#include <algorithm>
#include <memory>

void applyBilateralFilter(int** image, int width, int height, int kernelSize, float sigmaSpatial, float sigmaIntensity) {
    // Handle invalid input parameters
    if (width <= 0 || height <= 0 || kernelSize <= 0 || sigmaSpatial <= 0 || sigmaIntensity <= 0) {
        return;
    }

    // Convert input image to vector for safer memory handling
    std::vector<std::vector<int>> img(height, std::vector<int>(width));
    std::vector<std::vector<int>> result(height, std::vector<int>(width));
    
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            img[i][j] = image[i][j];
        }
    }

    // Handle single pixel image
    if (width == 1 && height == 1) {
        return;
    }

    const int halfSize = kernelSize / 2;
    const float spatialCoeff = -1.0f / (2.0f * sigmaSpatial * sigmaSpatial);
    const float intensityCoeff = -1.0f / (2.0f * sigmaIntensity * sigmaIntensity);

    // Pre-compute spatial weights
    std::vector<std::vector<float>> spatialWeights(kernelSize, std::vector<float>(kernelSize));
    for (int i = -halfSize; i <= halfSize; ++i) {
        for (int j = -halfSize; j <= halfSize; ++j) {
            spatialWeights[i + halfSize][j + halfSize] = 
                std::exp((i * i + j * j) * spatialCoeff);
        }
    }

    // Determine number of threads based on hardware
    const unsigned int threadCount = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    std::mutex mutex;

    // Thread worker function
    auto processRows = [&](int startRow, int endRow) {
        for (int i = startRow; i < endRow; ++i) {
            for (int j = 0; j < width; ++j) {
                if (i < halfSize || i >= height - halfSize || 
                    j < halfSize || j >= width - halfSize) {
                    result[i][j] = img[i][j];
                    continue;
                }

                float sumWeights = 0.0f;
                float sumFiltered = 0.0f;
                const int centerPixel = img[i][j];

                for (int ki = -halfSize; ki <= halfSize; ++ki) {
                    for (int kj = -halfSize; kj <= halfSize; ++kj) {
                        const int ni = i + ki;
                        const int nj = j + kj;
                        
                        if (ni < 0 || ni >= height || nj < 0 || nj >= width) {
                            continue;
                        }

                        const int neighborPixel = img[ni][nj];
                        const float spatialWeight = spatialWeights[ki + halfSize][kj + halfSize];
                        const float intensityDiff = centerPixel - neighborPixel;
                        const float intensityWeight = std::exp(intensityDiff * intensityDiff * intensityCoeff);
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
    const int rowsPerThread = height / threadCount;
    for (unsigned int t = 0; t < threadCount; ++t) {
        int startRow = t * rowsPerThread;
        int endRow = (t == threadCount - 1) ? height : (t + 1) * rowsPerThread;
        threads.emplace_back(processRows, startRow, endRow);
    }

    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }

    // Copy result back to input image
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            image[i][j] = result[i][j];
        }
    }
}
