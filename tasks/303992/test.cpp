#include <gtest/gtest.h>
#include <vector>
using namespace std;

#include "solution.cpp"

// Test Case 1: Simple 2x2 matrix with unique solution
TEST(GaussianEliminationTests, Simple2x2) {
    vector<vector<double>> matrix = {{2, 3}, {4, 5}};
    vector<double> constants = {6, 7};
    vector<double> expected = {-4.5, 5.0};
    vector<double> result = gaussianEliminationSolver(matrix, constants);
    ASSERT_EQ(result.size(), expected.size());
    for (size_t i = 0; i < result.size(); ++i) {
        EXPECT_NEAR(result[i], expected[i], 1e-6);
    }
}

// Test Case 2: 3x3 matrix with unique solution
TEST(GaussianEliminationTests, Simple3x3) {
    vector<vector<double>> matrix = {{3, -2, 5}, {2, 4, 7}, {1, -1, 1}};
    vector<double> constants = {6, 19, 3};
    vector<double> expected = {13.285714285714292, 5.85714285714286, -4.4285714285714315};
    vector<double> result = gaussianEliminationSolver(matrix, constants);
    ASSERT_EQ(result.size(), expected.size());
    for (size_t i = 0; i < result.size(); ++i) {
        EXPECT_NEAR(result[i], expected[i], 1e-6);
    }
}

// Test Case 3: Singular matrix (no solution or infinite solutions)
TEST(GaussianEliminationTests, SingularMatrix) {
    vector<vector<double>> matrix = {{1, 2}, {2, 4}};
    vector<double> constants = {3, 6};
    vector<double> result = gaussianEliminationSolver(matrix, constants);
    EXPECT_TRUE(result.empty());
}

// Test Case 4: Inconsistent system (no solution)
TEST(GaussianEliminationTests, InconsistentSystem) {
    vector<vector<double>> matrix = {{1, 1}, {2, 2}};
    vector<double> constants = {3, 5};
    vector<double> result = gaussianEliminationSolver(matrix, constants);
    EXPECT_TRUE(result.empty());
}

// Test Case 5: 1x1 matrix
TEST(GaussianEliminationTests, OneByOneMatrix) {
    vector<vector<double>> matrix = {{7}};
    vector<double> constants = {14};
    vector<double> expected = {2.0};
    vector<double> result = gaussianEliminationSolver(matrix, constants);
    ASSERT_EQ(result.size(), expected.size());
    for (size_t i = 0; i < result.size(); ++i) {
        EXPECT_NEAR(result[i], expected[i], 1e-6);
    }
}

// Test Case 6: Zero matrix (no unique solution)
TEST(GaussianEliminationTests, ZeroMatrix) {
    vector<vector<double>> matrix = {{0, 0}, {0, 0}};
    vector<double> constants = {0, 0};
    vector<double> result = gaussianEliminationSolver(matrix, constants);
    EXPECT_TRUE(result.empty());
}

// Test Case 7: Matrix with small values (close to zero but non-zero pivots)
TEST(GaussianEliminationTests, SmallValues) {
    vector<vector<double>> matrix = {{1e-9, 2}, {2, 1e-9}};
    vector<double> constants = {2, 3};
    vector<double> expected = {1.4999999995, 0.99999999925};
    vector<double> result = gaussianEliminationSolver(matrix, constants);
    ASSERT_EQ(result.size(), expected.size());
    for (size_t i = 0; i < result.size(); ++i) {
        EXPECT_NEAR(result[i], expected[i], 1e-6);
    }
}

// Test Case 8: Matrix with large values
TEST(GaussianEliminationTests, LargeValues) {
    vector<vector<double>> matrix = {{1e9, 2e9}, {3e9, 4e9}};
    vector<double> constants = {5e9, 6e9};
    vector<double> expected = {-3.9999999999999987, 4.499999999999999};
    vector<double> result = gaussianEliminationSolver(matrix, constants);
    ASSERT_EQ(result.size(), expected.size());
    for (size_t i = 0; i < result.size(); ++i) {
        EXPECT_NEAR(result[i], expected[i], 1e-6);
    }
}

// Test Case 9: Empty matrix (invalid case)
TEST(GaussianEliminationTests, EmptyMatrix) {
    vector<vector<double>> matrix = {};
    vector<double> constants = {};
    vector<double> result = gaussianEliminationSolver(matrix, constants);
    EXPECT_TRUE(result.empty());
}

// Test Case 10: Non-square matrix (invalid case)
TEST(GaussianEliminationTests, NonSquareMatrix) {
    vector<vector<double>> matrix = {{1, 2, 3}, {4, 5, 6}};
    vector<double> constants = {7, 8};
    vector<double> result = gaussianEliminationSolver(matrix, constants);
    EXPECT_TRUE(result.empty());
}
