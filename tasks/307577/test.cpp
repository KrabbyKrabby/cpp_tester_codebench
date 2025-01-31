#include <gtest/gtest.h>
#include <climits>
#include "solution.cpp"


// Define a test fixture
class MatrixTest : public ::testing::Test {};

// Positive Test Case: Regular multiplication
TEST_F(MatrixTest, MatrixMultiplication) {
    Matrix mat1(2, 3);
    mat1.setValue(0, 0, 1);
    mat1.setValue(0, 1, 2);
    mat1.setValue(0, 2, 3);
    mat1.setValue(1, 0, 4);
    mat1.setValue(1, 1, 5);
    mat1.setValue(1, 2, 6);

    Matrix mat2(3, 2);
    mat2.setValue(0, 0, 7);
    mat2.setValue(0, 1, 8);
    mat2.setValue(1, 0, 9);
    mat2.setValue(1, 1, 10);
    mat2.setValue(2, 0, 11);
    mat2.setValue(2, 1, 12);

    Matrix result = mat1.multiply(mat2);

    EXPECT_EQ(result.getValue(0, 0), 58);
    EXPECT_EQ(result.getValue(0, 1), 64);
    EXPECT_EQ(result.getValue(1, 0), 139);
    EXPECT_EQ(result.getValue(1, 1), 154);
}

// Positive Test Case: Determinant of a 2x2 matrix
TEST_F(MatrixTest, Determinant2x2) {
    Matrix mat(2, 2);
    mat.setValue(0, 0, 4);
    mat.setValue(0, 1, 7);
    mat.setValue(1, 0, 2);
    mat.setValue(1, 1, 6);

    EXPECT_EQ(mat.determinant(), 10);
}

// Positive Test Case: Determinant of a 3x3 matrix
TEST_F(MatrixTest, Determinant3x3) {
    Matrix mat(3, 3);
    mat.setValue(0, 0, 1);
    mat.setValue(0, 1, 2);
    mat.setValue(0, 2, 3);
    mat.setValue(1, 0, 0);
    mat.setValue(1, 1, 1);
    mat.setValue(1, 2, 4);
    mat.setValue(2, 0, 5);
    mat.setValue(2, 1, 6);
    mat.setValue(2, 2, 0);

    EXPECT_EQ(mat.determinant(), 1);
}

TEST_F(MatrixTest, DeterminantMatrixSizeMoreThan3) {
    Matrix mat(4, 4);
    mat.setValue(0, 0, 1);
    mat.setValue(0, 1, 2);
    mat.setValue(0, 2, 3);
    mat.setValue(0, 3, 4);
    mat.setValue(1, 0, 5);
    mat.setValue(1, 1, 6);
    mat.setValue(1, 2, 7);
    mat.setValue(1, 3, 8);
    mat.setValue(2, 0, 9);
    mat.setValue(2, 1, 10);
    mat.setValue(2, 2, 11);
    mat.setValue(2, 3, 12);
    mat.setValue(3, 0, 13);
    mat.setValue(3, 1, 14);
    mat.setValue(3, 2, 15);
    mat.setValue(3, 3, 16);
    EXPECT_EQ(mat.determinant(), 0);
}

// Negative Test Case: Multiplication with incompatible dimensions
TEST_F(MatrixTest, IncompatibleMultiplication) {
    Matrix mat1(2, 3);
    Matrix mat2(2, 3);
    EXPECT_THROW(mat1.multiply(mat2), std::invalid_argument);
}

// Negative Test Case: Determinant of a non-square matrix
TEST_F(MatrixTest, NonSquareMatrixDeterminant) {
    Matrix mat(2, 3);
    EXPECT_THROW(mat.determinant(), std::invalid_argument);
}

// Edge Case: Zero-sized matrix
TEST_F(MatrixTest, ZeroSizedMatrix) {
    EXPECT_THROW(Matrix(0, 0), std::invalid_argument);
}

// Edge Case: Determinant of a 1x1 matrix
TEST_F(MatrixTest, Determinant1x1) {
    Matrix mat(1, 1);
    mat.setValue(0, 0, 5);
    EXPECT_EQ(mat.determinant(), 5);
}

// Edge Case: Large matrix multiplication
TEST_F(MatrixTest, LargeMatrixMultiplication) {
    Matrix mat1(100, 101);
    Matrix mat2(101, 100);
    EXPECT_THROW(mat1.multiply(mat2), std::overflow_error);
}

TEST_F(MatrixTest, MultiplicationOverflow) {
    Matrix mat1(2, 2);
    mat1.setValue(0, 0, INT_MAX / 2);
    mat1.setValue(0, 1, 1);
    mat1.setValue(1, 0, 1);
    mat1.setValue(1, 1, INT_MAX / 2);

    Matrix mat2(2, 2);
    mat2.setValue(0, 0, 2);
    mat2.setValue(0, 1, 3);
    mat2.setValue(1, 0, 4);
    mat2.setValue(1, 1, 5);

    EXPECT_THROW(mat1.multiply(mat2), std::overflow_error);
}

TEST_F(MatrixTest, DeterminantOverflow) {
    Matrix mat(3, 3);
    mat.setValue(0, 0, INT_MAX / 2);
    mat.setValue(0, 1, 0);
    mat.setValue(0, 2, 0);
    mat.setValue(1, 0, 0);
    mat.setValue(1, 1, INT_MAX / 2);
    mat.setValue(1, 2, 0);
    mat.setValue(2, 0, 0);
    mat.setValue(2, 1, 0);
    mat.setValue(2, 2, INT_MAX / 2);

    EXPECT_THROW(mat.determinant(), std::overflow_error);
}

// Test case for 4x4 determinant overflow
TEST_F(MatrixTest, Determinant4x4Overflow) {
    Matrix mat(4, 4);
    mat.setValue(0, 0, INT_MAX / 2);
    mat.setValue(0, 1, 1);
    mat.setValue(0, 2, 1);
    mat.setValue(0, 3, 1);
    
    mat.setValue(1, 0, 1);
    mat.setValue(1, 1, INT_MAX / 2);
    mat.setValue(1, 2, 1);
    mat.setValue(1, 3, 1);
    
    mat.setValue(2, 0, 1);
    mat.setValue(2, 1, 1);
    mat.setValue(2, 2, INT_MAX / 2);
    mat.setValue(2, 3, 1);
    
    mat.setValue(3, 0, 1);
    mat.setValue(3, 1, 1);
    mat.setValue(3, 2, 1);
    mat.setValue(3, 3, INT_MAX / 2);

    // Expect determinant calculation to throw an overflow error
    EXPECT_THROW(mat.determinant(), std::overflow_error);
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}