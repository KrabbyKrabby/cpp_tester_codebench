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
            throw std::overflow_error("Matrix multiplication exceeds maximum allowed size.");
        }

        Matrix result(rows, other.cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < other.cols; ++j) {
                int sum = 0;
                for (size_t k = 0; k < cols; ++k) {
                    sum += data[i][k] * other.data[k][j];
                    if (sum > INT_MAX || sum < INT_MIN) {
                        throw std::overflow_error("Matrix multiplication result exceeds integer limits.");
                    }
                }
                result.data[i][j] = sum;
            }
        }
        return result;
    }

    // Calculate determinant
    int determinant() const {
        if (rows != cols) {
            throw std::invalid_argument("Determinant is only defined for square matrices.");
        }

        if (rows <= 3) {
            // Use recursive Laplace expansion for smaller matrices
            return laplaceDeterminant(*this);
        } else {
            // Use iterative approach for larger matrices
            return iterativeDeterminant(*this);
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

    // Recursive Laplace expansion for determinant calculation
    int laplaceDeterminant(const Matrix& matrix) const {
        if (matrix.rows == 1) {
            return matrix.data[0][0];
        }

        int det = 0;
        for (size_t i = 0; i < matrix.cols; ++i) {
            Matrix subMatrix(matrix.rows - 1, matrix.cols - 1);
            for (size_t j = 1; j < matrix.rows; ++j) {
                size_t k = 0;
                for (size_t col = 0; col < matrix.cols; ++col) {
                    if (col != i) {
                        subMatrix.data[j - 1][k] = matrix.data[j][col];
                        ++k;
                    }
                }
            }
            det += ((i % 2 == 0) ? 1 : -1) * matrix.data[0][i] * laplaceDeterminant(subMatrix);
            if (det > INT_MAX || det < INT_MIN) {
                throw std::overflow_error("Determinant calculation result exceeds integer limits.");
            }
        }
        return det;
    }

    // Iterative approach for determinant calculation
    int iterativeDeterminant(const Matrix& matrix) const {
        int det = 1;
        Matrix luMatrix = matrix;
        for (size_t i = 0; i < matrix.rows; ++i) {
            // Partial pivoting
            size_t maxIndex = i;
            for (size_t j = i + 1; j < matrix.rows; ++j) {
                if (abs(luMatrix.data[j][i]) > abs(luMatrix.data[maxIndex][i])) {
                    maxIndex = j;
                }
            }
            if (maxIndex != i) {
                std::swap(luMatrix.data[i], luMatrix.data[maxIndex]);
                det = -det;
            }

            // LU decomposition
            for (size_t j = i + 1; j < matrix.rows; ++j) {
                if (luMatrix.data[i][i] == 0) {
                    throw std::invalid_argument("Matrix is singular.");
                }
                int factor = luMatrix.data[j][i] / luMatrix.data[i][i];
                for (size_t k = i; k < matrix.cols; ++k) {
                    luMatrix.data[j][k] -= factor * luMatrix.data[i][k];
                }
            }
        }

        // Calculate determinant from LU decomposition
        for (size_t i = 0; i < matrix.rows; ++i) {
            det *= luMatrix.data[i][i];
            if (det > INT_MAX || det < INT_MIN) {
                throw std::overflow_error("Determinant calculation result exceeds integer limits.");
            }
        }
        return det;
    }
};
