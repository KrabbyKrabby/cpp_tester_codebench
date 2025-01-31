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
                long long sum = 0; // Use long long to handle potential overflows
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

    // Calculate determinant
    int determinant() const {
        if (rows != cols) {
            throw std::invalid_argument("Determinant is only defined for square matrices.");
        }

        if (rows == 1) {
            return data[0][0];
        } else if (rows == 2) {
            return data[0][0] * data[1][1] - data[0][1] * data[1][0];
        } else if (rows == 3) {
            return data[0][0] * (data[1][1] * data[2][2] - data[1][2] * data[2][1]) -
                   data[0][1] * (data[1][0] * data[2][2] - data[1][2] * data[2][0]) +
                   data[0][2] * (data[1][0] * data[2][1] - data[1][1] * data[2][0]);
        } else {
            int det = 0;
            for (size_t i = 0; i < rows; ++i) {
                Matrix subMatrix(rows - 1, cols - 1);
                for (size_t j = 1; j < rows; ++j) {
                    size_t colIndex = 0;
                    for (size_t k = 0; k < cols; ++k) {
                        if (k == i) {
                            continue;
                        }
                        subMatrix.data[j - 1][colIndex++] = data[j][k];
                    }
                }
                det += (i % 2 == 0 ? 1 : -1) * data[0][i] * subMatrix.determinant();
            }
            return det;
        }
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

