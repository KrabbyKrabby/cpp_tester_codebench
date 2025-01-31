#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>

class Matrix {
private:
    std::vector<std::vector<int>> data;
    size_t rows, cols;

public:
    // Constructor
    Matrix(size_t rows, size_t cols) : rows(rows), cols(cols), data(rows, std::vector<int>(cols, 0)) {}

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

    // Multiply two matrices (Incomplete)
    Matrix multiply(const Matrix& other) const {
        if (cols != other.rows) {
            throw std::invalid_argument("Matrix dimensions do not align for multiplication.");
        }

        // TODO: Implement matrix multiplication
        return Matrix(rows, other.cols); // Placeholder
    }

    // Calculate determinant (Incomplete)
    int determinant() const {
        if (rows != cols) {
            throw std::invalid_argument("Determinant is only defined for square matrices.");
        }

        // TODO: Implement determinant calculation
        return 0; // Placeholder
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