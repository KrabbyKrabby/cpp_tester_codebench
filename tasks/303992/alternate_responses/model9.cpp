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

void gaussianEliminationRecursive(vector<vector<double>>& augmentedMatrix, int currentRow, int n, int threadCount) {
    if (currentRow >= n) {
        return;
    }

    int maxRow = currentRow;
    for (int k = currentRow + 1; k < n; k++) {
        if (fabs(augmentedMatrix[k][currentRow]) > fabs(augmentedMatrix[maxRow][currentRow])) {
            maxRow = k;
        }
    }

    swap(augmentedMatrix[currentRow], augmentedMatrix[maxRow]);

    vector<thread> threads;
    int rowsPerThread = (n - currentRow - 1) / threadCount;

    for (int t = 0; t < threadCount; t++) {
        int startRow = currentRow + 1 + t * rowsPerThread;
        int endRow = (t == threadCount - 1) ? n - 1 : startRow + rowsPerThread - 1;

        threads.emplace_back(eliminateRows, ref(augmentedMatrix), startRow, endRow, currentRow, n);
    }

    for (auto& t : threads) {
        t.join();
    }

    gaussianEliminationRecursive(augmentedMatrix, currentRow + 1, n, threadCount);
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

    int threadCount = 4;
    gaussianEliminationRecursive(augmentedMatrix, 0, n, threadCount);

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