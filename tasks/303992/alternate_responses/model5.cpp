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
    static const int MIN_PARALLEL_SIZE = 64;
    mutex mtx;

    struct ThreadBlock {
        int startRow;
        int endRow;
        int pivotRow;
        int n;
    };

    bool isZero(double value) const {
        return fabs(value) < EPSILON;
    }

    void parallelElimination(vector<vector<double>>& matrix, const ThreadBlock& block) {
        for (int i = block.startRow; i < block.endRow; ++i) {
            if (i == block.pivotRow) continue;
            double factor = matrix[i][block.pivotRow] / matrix[block.pivotRow][block.pivotRow];
            for (int j = block.pivotRow; j <= block.n; ++j) {
                matrix[i][j] -= factor * matrix[block.pivotRow][j];
            }
        }
    }

    int findPivot(vector<vector<double>>& matrix, int k, int n) {
        int pivotRow = k;
        double maxVal = fabs(matrix[k][k]);
        
        for (int i = k + 1; i < n; ++i) {
            if (fabs(matrix[i][k]) > maxVal) {
                maxVal = fabs(matrix[i][k]);
                pivotRow = i;
            }
        }
        return pivotRow;
    }

    bool recursiveElimination(vector<vector<double>>& matrix, int k, int n) {
        if (k >= n - 1) return true;

        int pivotRow = findPivot(matrix, k, n);
        if (isZero(matrix[pivotRow][k])) return false;

        if (pivotRow != k) {
            lock_guard<mutex> lock(mtx);
            swap(matrix[k], matrix[pivotRow]);
        }

        vector<future<void>> futures;
        int numThreads = min(thread::hardware_concurrency(), static_cast<unsigned int>(n - k));
        int rowsPerThread = (n - k) / numThreads;

        for (int t = 0; t < numThreads; ++t) {
            ThreadBlock block{
                k + t * rowsPerThread,
                (t == numThreads - 1) ? n : k + (t + 1) * rowsPerThread,
                k,
                n
            };
            futures.push_back(async(launch::async, [this, &matrix, block]() {
                parallelElimination(matrix, block);
            }));
        }

        for (auto& future : futures) {
            future.wait();
        }

        return recursiveElimination(matrix, k + 1, n);
    }

public:
    vector<double> solve(const vector<vector<double>>& inputMatrix, const vector<double>& constants) {
        if (inputMatrix.empty() || constants.empty() || 
            inputMatrix.size() != constants.size() || 
            inputMatrix[0].size() != inputMatrix.size()) {
            return vector<double>();
        }

        int n = inputMatrix.size();
        vector<vector<double>> augmentedMatrix(n, vector<double>(n + 1));
        
        for (int i = 0; i < n; ++i) {
            copy(inputMatrix[i].begin(), inputMatrix[i].end(), augmentedMatrix[i].begin());
            augmentedMatrix[i][n] = constants[i];
        }

        if (!recursiveElimination(augmentedMatrix, 0, n)) {
            return vector<double>();
        }

        vector<double> solution(n);
        for (int i = n - 1; i >= 0; --i) {
            double sum = augmentedMatrix[i][n];
            for (int j = i + 1; j < n; ++j) {
                sum -= augmentedMatrix[i][j] * solution[j];
            }
            if (isZero(augmentedMatrix[i][i])) {
                return vector<double>();
            }
            solution[i] = sum / augmentedMatrix[i][i];
        }

        return solution;
    }
};

vector<double> gaussianEliminationSolver(const vector<vector<double>>& matrix, const vector<double>& constants) {
    GaussianSolver solver;
    return solver.solve(matrix, constants);
}