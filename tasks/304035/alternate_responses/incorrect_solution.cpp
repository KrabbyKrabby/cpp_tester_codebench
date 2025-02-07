#include <iostream>
#include <cmath>
#include <vector>
#include <thread>
#include <mutex>

// Function to apply Bilateral filter on the image
void applyBilateralFilter(int** image, int width, int height, int kernelSize, float sigmaSpatial, float sigmaIntensity) {
    // Define the number of threads to use for parallelization
    const int numThreads = std::thread::hardware_concurrency();

    // Define the chunk size for each thread
    int chunkSize = height / numThreads;

    // Create a mutex for thread safety
    std::mutex mtx;

    // Create a vector to store the results of each thread
    std::vector<std::vector<int>> results(height, std::vector<int>(width, 0));

    // Define a function for each thread to execute
    auto threadFunc = [&](int start, int end) {
        int halfSize = kernelSize / 2;

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

                // Lock the mutex to ensure thread safety
                mtx.lock();
                results[i][j] = static_cast<int>(sumFiltered / sumWeights);
                mtx.unlock();
            }
        }
    };

    // Create and start the threads
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        int start = i * chunkSize;
        int end = (i == numThreads - 1) ? height : (i + 1) * chunkSize;
        threads.emplace_back(threadFunc, start, end);
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }

    // Copy the results back into the original image
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            image[i][j] = results[i][j];
        }
    }
}
