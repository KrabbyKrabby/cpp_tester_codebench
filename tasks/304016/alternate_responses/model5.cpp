#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <memory>

using namespace std;

const int N = 9; // Size of the Sudoku grid
const int NUM_THREADS = 9; // Number of threads to use

// Shared variables for parallel processing
mutex mtx;
atomic<bool> solutionFound(false);
vector<vector<int>> sharedSolution;

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

// Function to find an empty cell in the Sudoku board
bool findEmptyCell(const vector<vector<int>>& board, int& row, int& col) {
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++) {
            if (board[row][col] == 0) {
                return true;
            }
        }
    }
    return false;
}

// Sequential Sudoku solver
bool solveSudokuSequential(vector<vector<int>>& board) {
    int row, col;
    
    if (!findEmptyCell(board, row, col)) {
        return true; // Puzzle is solved
    }
    
    for (int num = 1; num <= 9; num++) {
        if (isSafe(board, row, col, num)) {
            board[row][col] = num;
            
            if (solveSudokuSequential(board)) {
                return true;
            }
            
            board[row][col] = 0; // Backtrack
        }
    }
    return false;
}

// Worker function for parallel solving
void solveWorker(vector<vector<int>> board, int startNum) {
    if (solutionFound) return;
    
    int row, col;
    if (!findEmptyCell(board, row, col)) return;
    
    if (isSafe(board, row, col, startNum)) {
        board[row][col] = startNum;
        
        if (solveSudokuSequential(board)) {
            lock_guard<mutex> lock(mtx);
            if (!solutionFound.load()) {
                solutionFound = true;
                sharedSolution = board;
            }
        }
    }
}

// Main solving function with parallel processing
bool solveSudoku(vector<vector<int>>& board) {
    // Reset shared state
    solutionFound = false;
    sharedSolution.clear();
    sharedSolution = vector<vector<int>>(N, vector<int>(N));
    
    vector<thread> threads;
    
    // Launch threads for parallel solving
    for (int i = 1; i <= 9; i++) {
        vector<vector<int>> boardCopy = board;
        threads.emplace_back(solveWorker, boardCopy, i);
    }
    
    // Wait for all threads to complete
    for (auto& t : threads) {
        t.join();
    }
    
    // If solution was found, copy it to the input board
    if (solutionFound) {
        board = sharedSolution;
        return true;
    }
    
    return false;
}
