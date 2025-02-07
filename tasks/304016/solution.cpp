#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <future>
#include <queue>

using namespace std;

const int N = 9; // Size of the Sudoku grid
const int NUM_THREADS = 9; // Number of threads to use

mutex mtx;
atomic<bool> solutionFound(false);
vector<vector<int>> finalSolution;

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

// Helper function to find an empty cell
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

// Sequential solving function for deeper recursion levels
bool solveSequential(vector<vector<int>>& board) {
    int row, col;
    
    if (!findEmptyCell(board, row, col)) {
        return true; // Puzzle is solved
    }
    
    for (int num = 1; num <= 9; num++) {
        if (isSafe(board, row, col, num)) {
            board[row][col] = num;
            
            if (solveSequential(board)) {
                return true;
            }
            
            board[row][col] = 0; // Backtrack
        }
    }
    return false;
}

// Parallel solving function
void solveParallel(vector<vector<int>> board, int num, int row, int col) {
    if (solutionFound) return;
    
    if (isSafe(board, row, col, num)) {
        board[row][col] = num;
        
        if (solveSequential(board)) {
            lock_guard<mutex> lock(mtx);
            if (!solutionFound) {
                solutionFound = true;
                finalSolution = board;
            }
        }
    }
}

// Main solving function
bool solveSudoku(vector<vector<int>>& board) {
    int row, col;
    
    // Find first empty cell
    if (!findEmptyCell(board, row, col)) {
        return true; // Already solved
    }
    
    vector<thread> threads;
    solutionFound = false;
    
    // Launch threads for first empty cell
    for (int num = 1; num <= 9; num++) {
        if (threads.size() >= NUM_THREADS) {
            // Wait for threads to complete if we've reached the maximum
            for (auto& t : threads) {
                if (t.joinable()) {
                    t.join();
                }
            }
            threads.clear();
        }
        
        threads.emplace_back(solveParallel, board, num, row, col);
    }
    
    // Wait for remaining threads
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    
    if (solutionFound) {
        board = finalSolution;
        return true;
    }
    
    return false;
}