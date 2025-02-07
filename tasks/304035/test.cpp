#include <gtest/gtest.h>

#include "solution.cpp"

// Helper function to create a 2D array
int** create2DArray(const std::vector<std::vector<int>>& data) {
    int** arr = new int* [data.size()];
    for (size_t i = 0; i < data.size(); ++i) {
        arr[i] = new int[data[i].size()];
        for (size_t j = 0; j < data[i].size(); ++j) {
            arr[i][j] = data[i][j];
        }
    }
    return arr;
}

// Helper function to delete a 2D array
void delete2DArray(int** arr, int rows) {
    for (int i = 0; i < rows; ++i) {
        delete[] arr[i];
    }
    delete[] arr;
}

// Positive Test Case: Test filter on a small 3x3 image
TEST(BilateralFilterTest, PositiveCase) {
    std::vector<std::vector<int>> inputImage = {
        {10, 20, 30},
        {40, 50, 60},
        {70, 80, 90}
    };
    int width = 3, height = 3;
    int kernelSize = 3;
    float sigmaSpatial = 1.0f;
    float sigmaIntensity = 15.0f;

    int** image = create2DArray(inputImage);
    applyBilateralFilter(image, width, height, kernelSize, sigmaSpatial, sigmaIntensity);

    // Verify that the center pixel is filtered correctly
    EXPECT_NEAR(image[1][1], 50, 10);  // Expect the central pixel to be close to its original intensity
    delete2DArray(image, height);
}

// Negative Test Case: Test with null image
TEST(BilateralFilterTest, NegativeCase_NullImage) {
    int** image = nullptr;
    EXPECT_NO_THROW(applyBilateralFilter(image, 0, 0, 3, 1.0f, 15.0f));  // Should handle gracefully
}

// Edge Case: Test with single-pixel image
TEST(BilateralFilterTest, EdgeCase_SinglePixel) {
    std::vector<std::vector<int>> inputImage = {
        {50}
    };
    int width = 1, height = 1;
    int kernelSize = 3;
    float sigmaSpatial = 1.0f;
    float sigmaIntensity = 15.0f;

    int** image = create2DArray(inputImage);
    applyBilateralFilter(image, width, height, kernelSize, sigmaSpatial, sigmaIntensity);

    // Single-pixel image should remain unchanged
    EXPECT_EQ(image[0][0], 50);
    delete2DArray(image, height);
}

// Corner Case: Test with larger kernel size than image dimensions
TEST(BilateralFilterTest, CornerCase_LargeKernel) {
    std::vector<std::vector<int>> inputImage = {
        {10, 20},
        {30, 40}
    };
    int width = 2, height = 2;
    int kernelSize = 5;  // Larger than the image dimensions
    float sigmaSpatial = 1.0f;
    float sigmaIntensity = 15.0f;

    int** image = create2DArray(inputImage);
    EXPECT_NO_THROW(applyBilateralFilter(image, width, height, kernelSize, sigmaSpatial, sigmaIntensity));  // Should handle gracefully
    delete2DArray(image, height);
}

// Stress Test: Test on a large image
TEST(BilateralFilterTest, StressCase_LargeImage) {
    int width = 1000, height = 1000;
    int kernelSize = 5;
    float sigmaSpatial = 1.0f;
    float sigmaIntensity = 15.0f;

    int** image = new int* [height];
    for (int i = 0; i < height; ++i) {
        image[i] = new int[width];
        std::fill(image[i], image[i] + width, 100);  // Initialize with constant value
    }

    EXPECT_NO_THROW(applyBilateralFilter(image, width, height, kernelSize, sigmaSpatial, sigmaIntensity));
    delete2DArray(image, height);
}

// High Intensity Variation: 3x3 with strong contrast
TEST(BilateralFilterTest, HighIntensityVariation) {
    std::vector<std::vector<int>> inputImage = {
        {0,   0, 255},
        {0,  50, 255},
        {255, 255, 255}
    };
    int width = 3, height = 3;
    int kernelSize = 3;
    float sigmaSpatial = 1.0f;
    float sigmaIntensity = 5.0f; // Smaller sigmaIntensity => edges more preserved

    int** image = create2DArray(inputImage);
    EXPECT_NO_THROW(applyBilateralFilter(image, width, height, kernelSize, sigmaSpatial, sigmaIntensity));

    // Check that the corner pixel (top-left) remains relatively small
    EXPECT_LT(image[0][0], 128); // Expect it to remain significantly lower than 255
    // Check that the corner pixel (top-right) remains relatively high
    EXPECT_GT(image[0][2], 128);
    delete2DArray(image, height);
}

// Homogeneous Image: 5x5 with constant value
TEST(BilateralFilterTest, HomogeneousImage) {
    std::vector<std::vector<int>> inputImage(5, std::vector<int>(5, 100));
    int width = 5, height = 5;
    int kernelSize = 3;
    float sigmaSpatial = 1.0f;
    float sigmaIntensity = 15.0f;

    int** image = create2DArray(inputImage);
    EXPECT_NO_THROW(applyBilateralFilter(image, width, height, kernelSize, sigmaSpatial, sigmaIntensity));

    // Since the entire image is the same value, it should remain unchanged
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            EXPECT_EQ(image[i][j], 100);
        }
    }
    delete2DArray(image, height);
}

// Boundary Check: 4x4, verifying corner pixels after filtering
TEST(BilateralFilterTest, BoundaryCheck) {
    std::vector<std::vector<int>> inputImage = {
        { 0,   0,   0,   0 },
        { 0, 100, 100,   0 },
        { 0, 100, 100,   0 },
        { 0,   0,   0,   0 }
    };
    int width = 4, height = 4;
    int kernelSize = 3;
    float sigmaSpatial = 1.0f;
    float sigmaIntensity = 10.0f;

    int** image = create2DArray(inputImage);
    EXPECT_NO_THROW(applyBilateralFilter(image, width, height, kernelSize, sigmaSpatial, sigmaIntensity));

    // Verify corner pixel (0,0) remains near 0 since it has zero-intensity neighbors
    EXPECT_NEAR(image[0][0], 0, 5);

    // Middle pixels (1,1), (1,2), (2,1), (2,2) should remain near 100
    EXPECT_NEAR(image[1][1], 100, 5);
    EXPECT_NEAR(image[2][2], 100, 5);

    delete2DArray(image, height);
}

// Very Large Sigma Values
TEST(BilateralFilterTest, VeryLargeSigma) {
    std::vector<std::vector<int>> inputImage = {
        {0,   0,   0},
        {0,  50, 100},
        {100, 100, 100}
    };
    int width = 3, height = 3;
    int kernelSize = 3;
    float sigmaSpatial = 100.0f;   // Extremely large spatial sigma
    float sigmaIntensity = 100.0f; // Extremely large intensity sigma => less edge preservation

    int** image = create2DArray(inputImage);
    EXPECT_NO_THROW(applyBilateralFilter(image, width, height, kernelSize, sigmaSpatial, sigmaIntensity));

    // Expect it to behave more like an average filter; the center pixel is roughly the mean
    int sum = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            sum += image[i][j];
        }
    }
    double average = static_cast<double>(sum) / (width * height);
    // The center pixel (1,1) should be near this average
    EXPECT_NEAR(image[1][1], average, 10);
    delete2DArray(image, height);
}

// Very Small Sigma Values
TEST(BilateralFilterTest, VerySmallSigma) {
    std::vector<std::vector<int>> inputImage = {
        {10, 20, 30},
        {40, 50, 60},
        {70, 80, 90}
    };
    int width = 3, height = 3;
    int kernelSize = 3;
    float sigmaSpatial = 0.1f;     // Very small spatial sigma => only the closest pixels get weight
    float sigmaIntensity = 0.1f;   // Very small intensity sigma => only very similar intensities get weight

    int** image = create2DArray(inputImage);
    EXPECT_NO_THROW(applyBilateralFilter(image, width, height, kernelSize, sigmaSpatial, sigmaIntensity));

    // With such small sigmas, the filter changes very little unless neighbor is identical
    // Expect minimal change, especially for center pixel
    EXPECT_NEAR(image[1][1], 50, 2);

    delete2DArray(image, height);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}