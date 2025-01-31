#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>
#include <limits>

using namespace std;

mutex mtx;

void eliminateRows(vector<vector<double>>& augmentedMatrix, int startRow, int endRow, int pivotRow, int n) {
    for (int i = startRow; i <= endRow; i++) {
        double factor = augmentedMatrix[i][pivotRow] / augmentedMatrix[pivotRow][pivotRow];
        for (int j = pivotRow; j <= n; j++) {
            augmentedMatrix[i][j] -= factor * augmentedMatrix[pivotRow][j];
        }
    }
}

void gaussianElimination(vector<vector<double>>& augmentedMatrix, int startRow, int endRow, int n) {
    if (startRow >= endRow) return;

    int maxRow = startRow;
    for (int k = startRow + 1; k <= endRow; k++) {
        if (fabs(augmentedMatrix[k][startRow]) > fabs(augmentedMatrix[maxRow][startRow])) {
            maxRow = k;
        }
    }

    swap(augmentedMatrix[startRow], augmentedMatrix[maxRow]);

    int threadCount = 4;
    int rowsPerThread = (endRow - startRow) / threadCount;

    vector<thread> threads;
    for (int t = 0; t < threadCount; t++) {
        int start = startRow + 1 + t * rowsPerThread;
        int end = (t == threadCount - 1) ? endRow : start + rowsPerThread - 1;

        threads.emplace_back(eliminateRows, ref(augmentedMatrix), start, end, startRow, n);
    }

    for (auto& t : threads) {
        t.join();
    }

    gaussianElimination(augmentedMatrix, startRow + 1, endRow, n);
}

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

    gaussianElimination(augmentedMatrix, 0, n - 1, n);

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