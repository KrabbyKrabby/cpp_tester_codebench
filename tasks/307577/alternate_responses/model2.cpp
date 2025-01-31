#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <cmath>
#include <limits>

class Matrix {
private:
    std::vector<std::vector<int>> data;
    size_t rows, cols;

    // Helper function to check for integer overflow in multiplication
    static bool willMultiplicationOverflow(int a, int b) {
        if (a == 0 || b == 0) return false;
        if (a > 0 && b > 0) {
            return a > std::numeric_limits<int>::max() / b;
        }
        if (a < 0 && b < 0) {
            return a < std::numeric_limits<int>::max() / b;
        }
        if (a > 0 && b < 0) {
            return b < std::numeric_limits<int>::min() / a;
        }
        return a < std::numeric_limits<int>::min() / b;
    }

    // Helper function for determinant calculation using Gaussian elimination
    double determinantGaussian() const {
        std::vector<std::vector<double>> temp(rows, std::vector<double>(cols));
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                temp[i][j] = static_cast<double>(data[i][j]);
            }
        }

        double det = 1;
        for (size_t i = 0; i < rows; i++) {
            size_t pivot = i;
            for (size_t j = i + 1; j < rows; j++) {
                if (std::abs(temp[j][i]) > std::abs(temp[pivot][i])) {
                    pivot = j;
                }
            }

            if (std::abs(temp[pivot][i]) < 1e-10) return 0;

            if (pivot != i) {
                std::swap(temp[i], temp[pivot]);
                det = -det;
            }

            det *= temp[i][i];
            for (size_t j = i + 1; j < rows; j++) {
                double factor = temp[j][i] / temp[i][i];
                for (size_t k = i + 1; k < cols; k++) {
                    temp[j][k] -= factor * temp[i][k];
                }
            }
        }
        return det;
    }

public:
    // Constructor
    Matrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
        if (rows == 0 || cols == 0) {
            throw std::invalid_argument("Matrix dimensions cannot be zero");
        }
        data = std::vector<std::vector<int>>(rows, std::vector<int>(cols, 0));
    }

    // Set value at a specific position
    void setValue(size_t row, size_t col, int value) {
        if (row >= rows || col >= cols) {
            throw std::out_of_range("Invalid index");
        }
        data[row][col] = value;
    }

    // Get value at a specific position
    int getValue(size_t row, size_t col) const {
        if (row >= rows || col >= cols) {
            throw std::out_of_range("Invalid index");
        }
        return data[row][col];
    }

    // Multiply two matrices
    Matrix multiply(const Matrix& other) const {
        if (cols != other.rows) {
            throw std::invalid_argument("Matrix dimensions do not align for multiplication");
        }

        if (cols * other.rows > 10000) {
            throw std::overflow_error("Matrix multiplication exceeds size limit");
        }

        Matrix result(rows, other.cols);
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < other.cols; j++) {
                long long sum = 0;
                for (size_t k = 0; k < cols; k++) {
                    if (willMultiplicationOverflow(data[i][k], other.data[k][j])) {
                        throw std::overflow_error("Integer overflow in multiplication");
                    }
                    sum += static_cast<long long>(data[i][k]) * other.data[k][j];
                }
                if (sum > std::numeric_limits<int>::max() || 
                    sum < std::numeric_limits<int>::min()) {
                    throw std::overflow_error("Integer overflow in multiplication");
                }
                result.data[i][j] = static_cast<int>(sum);
            }
        }
        return result;
    }

    // Calculate determinant
    int determinant() const {
        if (rows != cols) {
            throw std::invalid_argument("Determinant is only defined for square matrices");
        }

        if (rows == 1) {
            return data[0][0];
        }

        if (rows == 2) {
            long long det = static_cast<long long>(data[0][0]) * data[1][1] - 
                           static_cast<long long>(data[0][1]) * data[1][0];
            if (det > std::numeric_limits<int>::max() || 
                det < std::numeric_limits<int>::min()) {
                throw std::overflow_error("Determinant value exceeds integer limits");
            }
            return static_cast<int>(det);
        }

        if (rows == 3) {
            // Using Sarrus' rule for 3x3
            long long det = 
                static_cast<long long>(data[0][0]) * data[1][1] * data[2][2] +
                static_cast<long long>(data[0][1]) * data[1][2] * data[2][0] +
                static_cast<long long>(data[0][2]) * data[1][0] * data[2][1] -
                static_cast<long long>(data[0][2]) * data[1][1] * data[2][0] -
                static_cast<long long>(data[0][0]) * data[1][2] * data[2][1] -
                static_cast<long long>(data[0][1]) * data[1][0] * data[2][2];

            if (det > std::numeric_limits<int>::max() || 
                det < std::numeric_limits<int>::min()) {
                throw std::overflow_error("Determinant value exceeds integer limits");
            }
            return static_cast<int>(det);
        }

        // For larger matrices, use Gaussian elimination
        double det = determinantGaussian();
        if (std::abs(det) > std::numeric_limits<int>::max()) {
            throw std::overflow_error("Determinant value exceeds integer limits");
        }
        return static_cast<int>(std::round(det));
    }

    // Display the matrix
    void display() const {
        for (const auto& row : data) {
            for (const auto& val : row) {
                std::cout << std::setw(5) << val << " ";
            }
            std::cout << "\n";
        }
    }
};