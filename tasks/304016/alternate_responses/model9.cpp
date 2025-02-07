#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

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

// Function to solve the Sudoku using backtracking
bool solveSudoku(vector<vector<int>>& board) {
    // Function to check if the board is solved
    auto isSolved = [&board]() {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (board[i][j] == 0) {
                    return false;
                }
            }
        }
        return true;
    };

    // Function to solve the Sudoku recursively
    std::atomic<bool> solved(false);
    std::mutex mtx;

    auto solveRecursive = [&](auto self, int row, int col) {
        if (solved) {
            return;
        }

        if (col == N) {
            col = 0;
            row++;
        }

        if (row == N) {
            solved = true;
            return;
        }

        if (board[row][col] != 0) {
            self(self, row, col + 1);
            return;
        }

        for (int num = 1; num <= N; num++) {
            if (isSafe(board, row, col, num)) {
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    board[row][col] = num;
                }
                self(self, row, col + 1);
                if (solved) {
                    return;
                }
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    board[row][col] = 0;
                }
            }
        }
    };

    // Divide the workload into threads
    int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; i++) {
        threads.emplace_back(solveRecursive, solveRecursive, i, 0);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return solved;
}

