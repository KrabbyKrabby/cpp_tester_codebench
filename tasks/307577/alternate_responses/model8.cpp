#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <climits>

class Matrix {
private:
    std::vector<std::vector<int>> data;
    size_t rows, cols;

public:
    // Constructor
    Matrix(size_t rows, size_t cols) : rows(rows), cols(cols), data(rows, std::vector<int>(cols, 0)) {
        if (rows == 0 || cols == 0) {
            throw std::invalid_argument("Matrix dimensions cannot be zero.");
        }
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
            throw std::invalid_argument("Matrix dimensions do not align for multiplication.");
        }

        if (cols * other.rows > 10000) {
            throw std::overflow_error("Matrix multiplication exceeds computation limit.");
        }

        Matrix result(rows, other.cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < other.cols; ++j) {
                long long sum = 0; // Use long long to handle potential overflow
                for (size_t k = 0; k < cols; ++k) {
                    sum += static_cast<long long>(data[i][k]) * other.data[k][j];
                    if (sum > INT_MAX || sum < INT_MIN) {
                        throw std::overflow_error("Matrix multiplication result exceeds integer limits.");
                    }
                }
                result.data[i][j] = static_cast<int>(sum);
            }
        }
        return result;
    }

    // Calculate determinant using recursive Laplace expansion for smaller matrices and iterative approach for larger ones
    int determinant() const {
        if (rows != cols) {
            throw std::invalid_argument("Determinant is only defined for square matrices.");
        }

        if (rows == 1) {
            return data[0][0];
        }

        if (rows == 2) {
            return data[0][0] * data[1][1] - data[0][1] * data[1][0];
        }

        if (rows == 3) {
            return data[0][0] * (data[1][1] * data[2][2] - data[1][2] * data[2][1]) -
                   data[0][1] * (data[1][0] * data[2][2] - data[1][2] * data[2][0]) +
                   data[0][2] * (data[1][0] * data[2][1] - data[1][1] * data[2][0]);
        }

        // For larger matrices, use an iterative approach (e.g., LU decomposition or Gauss elimination)
        // For simplicity, this example uses a naive iterative approach which is not efficient for large matrices
        int det = 1;
        std::vector<std::vector<int>> matrix = data; // Create a copy of the matrix
        for (size_t i = 0; i < rows; ++i) {
            // Search for maximum in this column
            size_t maxEl = i;
            for (size_t k = i + 1; k < rows; ++k) {
                if (abs(matrix[k][i]) > abs(matrix[maxEl][i])) {
                    maxEl = k;
                }
            }

            // Swap maximum row with current row
            std::swap(matrix[i], matrix[maxEl]);

            // Update determinant
            det *= matrix[i][i];

            // Make all rows below this one 0 in current column
            for (size_t k = i + 1; k < rows; ++k) {
                long long c = -static_cast<long long>(matrix[k][i]) / matrix[i][i];
                for (size_t j = i; j < cols; ++j) {
                    matrix[k][j] += c * matrix[i][j];
                }
            }
        }

        return det;
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