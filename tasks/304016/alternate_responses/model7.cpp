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

// Function to solve the Sudoku using backtracking
bool solveSudoku(vector<vector<int>>& board, int row = 0, int col = 0) {
    if (row == N) {
        return true; // Sudoku is solved
    }

    if (col == N) {
        return solveSudoku(board, row + 1, 0); // Move to next row
    }

    if (board[row][col] != 0) {
        return solveSudoku(board, row, col + 1); // Skip filled cells
    }

    for (int num = 1; num <= N; num++) {
        if (isSafe(board, row, col, num)) {
            board[row][col] = num; // Try placing num at current position

            if (solveSudoku(board, row, col + 1)) {
                return true; // Recursively solve the rest of the Sudoku
            }

            board[row][col] = 0; // Backtrack if num doesn't lead to a solution
        }
    }

    return false; // No valid number can be placed at current position
}

// Function to solve Sudoku using multithreading
void solveSudokuMultithreaded(vector<vector<int>>& board, int startRow, int endRow, bool& solved) {
    for (int row = startRow; row < endRow; row++) {
        for (int col = 0; col < N; col++) {
            if (board[row][col] == 0) {
                for (int num = 1; num <= N; num++) {
                    if (isSafe(board, row, col, num)) {
                        board[row][col] = num;

                        if (solveSudoku(board, row, col + 1)) {
                            solved = true;
                            return; // Solution found, exit thread
                        }

                        board[row][col] = 0; // Backtrack
                    }
                }
            }
        }
    }
}

