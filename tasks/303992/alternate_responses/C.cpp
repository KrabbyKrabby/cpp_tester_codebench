#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>
#include <limits>
#include <future>
#include <algorithm>

using namespace std;

class GaussianSolver {
private:
    static constexpr double EPSILON = 1e-10;
    static constexpr int MIN_PARALLEL_SIZE = 64;
    mutex mtx;

    struct MatrixPartition {
        int startRow;
        int endRow;
        int pivotRow;
        int n;
    };

    bool isZero(double value) const {
        return fabs(value) < EPSILON;
    }

    void validateInput(const vector<vector<double>>& matrix, const vector<double>& constants) {
        if (matrix.empty() || constants.empty() || matrix.size() != constants.size()) {
            throw invalid_argument("Invalid matrix or constants dimensions");
        }
        for (const auto& row : matrix) {
            if (row.size() != matrix.size()) {
                throw invalid_argument("Matrix must be square");
            }
        }
    }

    pair<int, double> findPivot(vector<vector<double>>& augmentedMatrix, int startRow, int col, int n) {
        int maxRow = startRow;
        double maxVal = fabs(augmentedMatrix[startRow][col]);

        for (int i = startRow + 1; i < n; i++) {
            double absVal = fabs(augmentedMatrix[i][col]);
            if (absVal > maxVal) {
                maxVal = absVal;
                maxRow = i;
            }
        }
        return {maxRow, maxVal};
    }

    void eliminateRowsParallel(vector<vector<double>>& augmentedMatrix, const MatrixPartition& part) {
        for (int i = part.startRow; i <= part.endRow; i++) {
            if (i == part.pivotRow) continue;
            double factor = augmentedMatrix[i][part.pivotRow] / augmentedMatrix[part.pivotRow][part.pivotRow];
            for (int j = part.pivotRow; j <= part.n; j++) {
                augmentedMatrix[i][j] -= factor * augmentedMatrix[part.pivotRow][j];
            }
        }
    }

    bool recursiveElimination(vector<vector<double>>& augmentedMatrix, int currentRow, int n) {
        if (currentRow >= n - 1) return true;

        auto [maxRow, maxVal] = findPivot(augmentedMatrix, currentRow, currentRow, n);
        if (isZero(maxVal)) return false;

        if (maxRow != currentRow) {
            swap(augmentedMatrix[currentRow], augmentedMatrix[maxRow]);
        }

        vector<future<void>> futures;
        int numThreads = min(4, n - currentRow);
        int rowsPerThread = (n - currentRow - 1) / numThreads;

        for (int t = 0; t < numThreads; t++) {
            MatrixPartition part{
                currentRow + 1 + t * rowsPerThread,
                (t == numThreads - 1) ? n - 1 : currentRow + 1 + (t + 1) * rowsPerThread - 1,
                currentRow,
                n
            };
            futures.push_back(async(launch::async, [this, &augmentedMatrix, part]() {
                eliminateRowsParallel(augmentedMatrix, part);
            }));
        }

        for (auto& future : futures) {
            future.get();
        }

        return recursiveElimination(augmentedMatrix, currentRow + 1, n);
    }

public:
    vector<double> solve(const vector<vector<double>>& matrix, const vector<double>& constants) {
        try {
            validateInput(matrix, constants);
            int n = matrix.size();
            vector<vector<double>> augmentedMatrix(n, vector<double>(n + 1));

            // Initialize augmented matrix
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    augmentedMatrix[i][j] = matrix[i][j];
                }
                augmentedMatrix[i][n] = constants[i];
            }

            // Perform elimination
            if (!recursiveElimination(augmentedMatrix, 0, n)) {
                return vector<double>(); // Return empty vector for singular matrix
            }

            // Back substitution
            vector<double> solution(n);
            for (int i = n - 1; i >= 0; i--) {
                if (isZero(augmentedMatrix[i][i])) {
                    return vector<double>();
                }
                solution[i] = augmentedMatrix[i][n];
                for (int j = i + 1; j < n; j++) {
                    solution[i] -= augmentedMatrix[i][j] * solution[j];
                }
                solution[i] /= augmentedMatrix[i][i];
            }

            return solution;
        }
        catch (const exception& e) {
            return vector<double>();
        }
    }
};

vector<double> gaussianEliminationSolver(const vector<vector<double>>& matrix, const vector<double>& constants) {
    GaussianSolver solver;
    return solver.solve(matrix, constants);
}
