#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

const int N = 9;

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

// Function to solve the Sudoku using backtracking
bool solveSudoku(vector<vector<int>>& board, int row = 0, int col = 0) {
    if (col == N) {
        col = 0;
        row++;
    }

    if (row == N) {
        return true;
    }

    if (board[row][col] != 0) {
        return solveSudoku(board, row, col + 1);
    }

    for (int num = 1; num <= N; num++) {
        if (isSafe(board, row, col, num)) {
            board[row][col] = num;
            if (solveSudoku(board, row, col + 1)) {
                return true;
            }
            board[row][col] = 0;
        }
    }

    return false;
}

// Function to solve the Sudoku using multithreading
bool solveSudokuMultithreaded(vector<vector<int>>& board) {
    mutex mtx;
    bool solved = false;

    // Function to solve the Sudoku in a separate thread
    auto solveThread = [&](int row, int col) {
        if (solveSudoku(board, row, col)) {
            lock_guard<mutex> lock(mtx);
            solved = true;
        }
    };

    vector<thread> threads;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 0) {
                threads.emplace_back(solveThread, i, j);
            }
        }
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return solved;
}

