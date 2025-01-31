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

    // Helper function to check integer multiplication overflow
    bool multiplyOverflow(int a, int b) const {
        if (a == 0 || b == 0) return false;
        if (a > 0 && b > 0 && a > std::numeric_limits<int>::max() / b) return true;
        if (a > 0 && b < 0 && b < std::numeric_limits<int>::min() / a) return true;
        if (a < 0 && b > 0 && a < std::numeric_limits<int>::min() / b) return true;
        if (a < 0 && b < 0 && a < std::numeric_limits<int>::max() / b) return true;
        return false;
    }

    // Helper function to check integer addition overflow
    bool addOverflow(int a, int b) const {
        if ((b > 0 && a > std::numeric_limits<int>::max() - b) ||
            (b < 0 && a < std::numeric_limits<int>::min() - b))
            return true;
        return false;
    }

    // Helper function for determinant calculation of 2x2 matrix
    int det2x2() const {
        int a = data[0][0], b = data[0][1];
        int c = data[1][0], d = data[1][1];
        if (multiplyOverflow(a, d) || multiplyOverflow(b, c))
            throw std::overflow_error("Integer overflow in determinant calculation");
        return a * d - b * c;
    }

    // Helper function for determinant calculation of 3x3 matrix using Sarrus' rule
    int det3x3() const {
        int det = 0;
        // Positive terms
        for (int i = 0; i < 3; i++) {
            int term = data[0][i];
            for (int j = 1; j < 3; j++) {
                if (multiplyOverflow(term, data[j][(i + j) % 3]))
                    throw std::overflow_error("Integer overflow in determinant calculation");
                term *= data[j][(i + j) % 3];
            }
            if (addOverflow(det, term))
                throw std::overflow_error("Integer overflow in determinant calculation");
            det += term;
        }
        // Negative terms
        for (int i = 0; i < 3; i++) {
            int term = data[0][i];
            for (int j = 1; j < 3; j++) {
                if (multiplyOverflow(term, data[j][(i - j + 3) % 3]))
                    throw std::overflow_error("Integer overflow in determinant calculation");
                term *= data[j][(i - j + 3) % 3];
            }
            if (addOverflow(det, -term))
                throw std::overflow_error("Integer overflow in determinant calculation");
            det -= term;
        }
        return det;
    }

public:
    // Constructor
    Matrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
        if (rows == 0 || cols == 0)
            throw std::invalid_argument("Matrix dimensions cannot be zero");
        data.resize(rows, std::vector<int>(cols, 0));
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
            throw std::overflow_error("Matrix dimensions too large for multiplication");
        }

        Matrix result(rows, other.cols);
        
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < other.cols; j++) {
                int sum = 0;
                for (size_t k = 0; k < cols; k++) {
                    if (multiplyOverflow(data[i][k], other.data[k][j]))
                        throw std::overflow_error("Integer overflow in multiplication");
                    int product = data[i][k] * other.data[k][j];
                    if (addOverflow(sum, product))
                        throw std::overflow_error("Integer overflow in multiplication");
                    sum += product;
                }
                result.data[i][j] = sum;
            }
        }
        return result;
    }

    // Calculate determinant
    int determinant() const {
        if (rows != cols) {
            throw std::invalid_argument("Determinant is only defined for square matrices");
        }

        if (rows == 1) return data[0][0];
        if (rows == 2) return det2x2();
        if (rows == 3) return det3x3();

        // For larger matrices, use Gaussian elimination
        std::vector<std::vector<double>> temp(rows, std::vector<double>(cols));
        for (size_t i = 0; i < rows; i++)
            for (size_t j = 0; j < cols; j++)
                temp[i][j] = static_cast<double>(data[i][j]);

        double det = 1.0;
        for (size_t i = 0; i < rows; i++) {
            size_t pivot = i;
            for (size_t j = i + 1; j < rows; j++)
                if (std::abs(temp[j][i]) > std::abs(temp[pivot][i]))
                    pivot = j;

            if (std::abs(temp[pivot][i]) < 1e-10)
                return 0;

            if (pivot != i) {
                std::swap(temp[i], temp[pivot]);
                det = -det;
            }

            det *= temp[i][i];
            for (size_t j = i + 1; j < rows; j++) {
                double factor = temp[j][i] / temp[i][i];
                for (size_t k = i + 1; k < cols; k++)
                    temp[j][k] -= factor * temp[i][k];
            }
        }

        if (std::abs(det) > static_cast<double>(std::numeric_limits<int>::max()))
            throw std::overflow_error("Determinant exceeds integer limits");

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