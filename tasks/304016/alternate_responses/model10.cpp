#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

const int N = 9; // Size of the Sudoku grid

// Function to check if it's safe to place num at board[row][col]
bool isSafe(const vector<vector<int>>& board, int row, int col, int num) {
    for (int i = 0; i < N; i++) {
        // Check row and column
        if (board[row][i] == num || board[i][col] == num) {
            return false;
        }
        // Check 3x3 subgrid
        if (board[(row / 3) * 3 + i / 3][(col / 3) * 3 + i % 3] == num) {
            return false;
        }
    }
    return true;
}

// Recursive helper function for solving Sudoku using backtracking
bool solveSudokuHelper(vector<vector<int>>& board, int startRow) {
    for (int row = startRow; row < N; row++) {
        for (int col = 0; col < N; col++) {
            if (board[row][col] == 0) {
                for (int num = 1; num <= N; num++) {
                    if (isSafe(board, row, col, num)) {
                        board[row][col] = num;
                        if (solveSudokuHelper(board, row)) {
                            return true;
                        }
                        board[row][col] = 0; // Backtrack
                    }
                }
                return false; // No valid number found for this cell
            }
        }
    }
    return true; // Sudoku solved
}

// Function to solve the Sudoku using backtracking and multithreading
bool solveSudoku(vector<vector<int>>& board) {
    // Divide the workload into three threads, each starting from a different row
    const int numThreads = 3;
    vector<thread> threads;
    vector<bool> results(numThreads);
    mutex mtx;

    auto solveThread = [&](int startRow, int resultIndex) {
        bool solved = solveSudokuHelper(board, startRow);
        lock_guard<mutex> lock(mtx);
        results[resultIndex] = solved;
    };

    for (int i = 0; i < numThreads; i++) {
        int startRow = i * (N / numThreads);
        threads.emplace_back(solveThread, startRow, i);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    // Check if any thread found a solution
    return any_of(results.begin(), results.end(), [](bool result) { return result; });
}

