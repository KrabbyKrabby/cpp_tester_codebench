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
    bool willMultiplicationOverflow(int a, int b) const {
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

    // Helper function for determinant of 2x2 matrix
    int det2x2() const {
        int a = data[0][0], b = data[0][1];
        int c = data[1][0], d = data[1][1];
        
        if (willMultiplicationOverflow(a, d) || willMultiplicationOverflow(b, c)) {
            throw std::overflow_error("Integer overflow in determinant calculation");
        }
        return a * d - b * c;
    }

    // Helper function for determinant of 3x3 matrix using Sarrus' rule
    int det3x3() const {
        int det = 0;
        // Positive terms
        for (int i = 0; i < 3; i++) {
            int product = 1;
            for (int j = 0; j < 3; j++) {
                if (willMultiplicationOverflow(product, data[j][(i + j) % 3])) {
                    throw std::overflow_error("Integer overflow in determinant calculation");
                }
                product *= data[j][(i + j) % 3];
            }
            det += product;
        }
        // Negative terms
        for (int i = 0; i < 3; i++) {
            int product = 1;
            for (int j = 0; j < 3; j++) {
                if (willMultiplicationOverflow(product, data[2 - j][(i + j) % 3])) {
                    throw std::overflow_error("Integer overflow in determinant calculation");
                }
                product *= data[2 - j][(i + j) % 3];
            }
            det -= product;
        }
        return det;
    }

    // Helper function for larger matrices using Gaussian elimination
    int detLarge() const {
        std::vector<std::vector<double>> temp(data.begin(), data.end());
        double det = 1.0;
        
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
                for (size_t k = i; k < cols; k++) {
                    temp[j][k] -= factor * temp[i][k];
                }
            }
        }
        
        if (std::abs(det) > std::numeric_limits<int>::max()) {
            throw std::overflow_error("Determinant exceeds integer limits");
        }
        
        return static_cast<int>(std::round(det));
    }

public:
    // Constructor
    Matrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
        if (rows == 0 || cols == 0) {
            throw std::invalid_argument("Matrix dimensions cannot be zero");
        }
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
            throw std::overflow_error("Matrix multiplication size exceeds limit");
        }

        Matrix result(rows, other.cols);
        
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < other.cols; j++) {
                int sum = 0;
                for (size_t k = 0; k < cols; k++) {
                    if (willMultiplicationOverflow(data[i][k], other.data[k][j])) {
                        throw std::overflow_error("Integer overflow in multiplication");
                    }
                    int product = data[i][k] * other.data[k][j];
                    
                    if (sum > 0 && product > std::numeric_limits<int>::max() - sum) {
                        throw std::overflow_error("Integer overflow in addition");
                    }
                    if (sum < 0 && product < std::numeric_limits<int>::min() - sum) {
                        throw std::overflow_error("Integer overflow in addition");
                    }
                    
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
        return detLarge();
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
