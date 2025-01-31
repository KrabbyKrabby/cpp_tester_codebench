#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>
#include <limits>
#include <future>
#include <algorithm>

using namespace std;

mutex mtx;
const double EPSILON = 1e-10;

void parallelRowElimination(vector<vector<double>>& matrix, int pivotRow, int startRow, 
                          int endRow, int n, double pivotVal) {
    for (int i = startRow; i <= endRow; i++) {
        if (i != pivotRow && fabs(matrix[i][pivotRow]) > EPSILON) {
            double factor = matrix[i][pivotRow] / pivotVal;
            for (int j = pivotRow; j <= n; j++) {
                matrix[i][j] -= factor * matrix[pivotRow][j];
                if (fabs(matrix[i][j]) < EPSILON) matrix[i][j] = 0;
            }
        }
    }
}

bool recursiveElimination(vector<vector<double>>& matrix, int currentRow, int n, 
                         int threadCount) {
    if (currentRow >= n - 1) return true;

    int maxRow = currentRow;
    double maxVal = fabs(matrix[currentRow][currentRow]);

    for (int i = currentRow + 1; i < n; i++) {
        if (fabs(matrix[i][currentRow]) > maxVal) {
            maxVal = fabs(matrix[i][currentRow]);
            maxRow = i;
        }
    }

    if (maxVal < EPSILON) return false;

    if (maxRow != currentRow) {
        lock_guard<mutex> lock(mtx);
        swap(matrix[currentRow], matrix[maxRow]);
    }

    vector<future<void>> futures;
    int rowsPerThread = max(1, (n - currentRow - 1) / threadCount);

    for (int t = 0; t < threadCount; t++) {
        int startRow = currentRow + 1 + t * rowsPerThread;
        int endRow = (t == threadCount - 1) ? n - 1 : 
                     min(startRow + rowsPerThread - 1, n - 1);

        if (startRow <= endRow) {
            futures.push_back(async(launch::async, parallelRowElimination,
                ref(matrix), currentRow, startRow, endRow, n,
                matrix[currentRow][currentRow]));
        }
    }

    for (auto& future : futures) {
        future.wait();
    }

    return recursiveElimination(matrix, currentRow + 1, n, threadCount);
}

vector<double> gaussianEliminationSolver(const vector<vector<double>>& matrix, 
                                       const vector<double>& constants) {
    if (matrix.empty() || constants.empty() || 
        matrix.size() != constants.size() || 
        matrix[0].size() != matrix.size()) {
        return vector<double>();
    }

    int n = matrix.size();
    vector<vector<double>> augmentedMatrix(n, vector<double>(n + 1));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            augmentedMatrix[i][j] = matrix[i][j];
        }
        augmentedMatrix[i][n] = constants[i];
    }

    int threadCount = min(4, n / 2);
    if (!recursiveElimination(augmentedMatrix, 0, n, threadCount)) {
        return vector<double>();
    }

    vector<double> solution(n);
    for (int i = n - 1; i >= 0; i--) {
        double sum = 0;
        for (int j = i + 1; j < n; j++) {
            sum += augmentedMatrix[i][j] * solution[j];
        }
        
        if (fabs(augmentedMatrix[i][i]) < EPSILON) {
            if (fabs(augmentedMatrix[i][n] - sum) > EPSILON) {
                return vector<double>();
            }
            solution[i] = 0;
        } else {
            solution[i] = (augmentedMatrix[i][n] - sum) / augmentedMatrix[i][i];
        }
    }

    return solution;
}