#include <iostream>
#include <cmath>

// Function to apply Bilateral filter on the image
void applyBilateralFilter(int** image, int width, int height, int kernelSize, float sigmaSpatial, float sigmaIntensity) {
    int halfSize = kernelSize / 2;
    int** result = new int*[height];
    for (int i = 0; i < height; ++i) {
        result[i] = new int[width];
    }

    for (int i = halfSize; i < height - halfSize; ++i) {
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

    // Copy the result back into the original image
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            image[i][j] = result[i][j];
        }
    }

    // Free the memory used for the result array
    for (int i = 0; i < height; ++i) {
        delete[] result[i];
    }
    delete[] result;
}