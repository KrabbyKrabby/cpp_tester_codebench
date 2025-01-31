#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>
#include <limits>

using namespace std;

mutex mtx;

// Function to eliminate rows recursively
void eliminateRows(vector<vector<double>>& augmentedMatrix, int startRow, int endRow, int pivotRow, int n) {
    if (startRow > endRow) return;

    int midRow = startRow + (endRow - startRow) / 2;

    double factor = augmentedMatrix[midRow][pivotRow] / augmentedMatrix[pivotRow][pivotRow];
    for (int j = pivotRow; j <= n; j++) {
        augmentedMatrix[midRow][j] -= factor * augmentedMatrix[pivotRow][j];
    }

    // Recursively eliminate rows
    eliminateRows(augmentedMatrix, startRow, midRow - 1, pivotRow, n);
    eliminateRows(augmentedMatrix, midRow + 1, endRow, pivotRow, n);
}

// Function to perform Gaussian elimination using multithreading
vector<double> gaussianEliminationSolver(const vector<vector<double>>& matrix, const vector<double>& constants) {
    if (matrix.empty() || constants.size() != matrix.size() || matrix[0].size() != matrix.size()) {
        return {};
    }

    int n = matrix.size();
    vector<vector<double>> augmentedMatrix(n, vector<double>(n + 1));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n; j++) {
            augmentedMatrix[i][j] = (j < n) ? matrix[i][j] : constants[i];
        }
    }

    for (int i = 0; i < n; i++) {
        int maxRow = i;
        for (int k = i + 1; k < n; k++) {
            if (fabs(augmentedMatrix[k][i]) > fabs(augmentedMatrix[maxRow][i])) {
                maxRow = k;
            }
        }

        swap(augmentedMatrix[i], augmentedMatrix[maxRow]);

        if (augmentedMatrix[i][i] == 0) {
            // Singular system
            return {};
        }

        vector<thread> threads;
        int threadCount = 4;
        int rowsPerThread = (n - i - 1) / threadCount;

        for (int t = 0; t < threadCount; t++) {
            int startRow = i + 1 + t * rowsPerThread;
            int endRow = (t == threadCount - 1) ? n - 1 : startRow + rowsPerThread - 1;

            threads.emplace_back(eliminateRows, ref(augmentedMatrix), startRow, endRow, i, n);
        }

        for (auto& t : threads) {
            t.join();
        }
    }

    vector<double> solution(n, 0);
    for (int i = n - 1; i >= 0; i--) {
        solution[i] = augmentedMatrix[i][n];
        for (int j = i + 1; j < n; j++) {
            solution[i] -= augmentedMatrix[i][j] * solution[j];
        }
        solution[i] /= augmentedMatrix[i][i];
    }

    return solution;
}