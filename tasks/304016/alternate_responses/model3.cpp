#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>

using namespace std;

const int N = 9; // Size of the Sudoku grid
atomic<bool> solutionFound(false);
mutex boardMutex;

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

// Helper function to find an empty position
bool findEmptyPosition(const vector<vector<int>>& board, int& row, int& col) {
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++) {
            if (board[row][col] == 0) {
                return true;
            }
        }
    }
    return false;
}

// Sequential solving function for each thread
bool solveSequential(vector<vector<int>>& board) {
    int row, col;
    
    if (!findEmptyPosition(board, row, col)) {
        return true; // Puzzle is solved
    }
    
    for (int num = 1; num <= 9; num++) {
        if (solutionFound) return false; // Another thread found a solution
        
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

// Thread worker function
void solveWorker(vector<vector<int>> boardCopy, int startNum, vector<vector<int>>& finalBoard) {
    int row, col;
    if (!findEmptyPosition(boardCopy, row, col)) {
        return; // No empty position found
    }
    
    if (isSafe(boardCopy, row, col, startNum)) {
        boardCopy[row][col] = startNum;
        
        if (solveSequential(boardCopy)) {
            lock_guard<mutex> lock(boardMutex);
            if (!solutionFound.exchange(true)) {
                finalBoard = boardCopy;
            }
        }
    }
}

// Main solving function with multithreading
bool solveSudoku(vector<vector<int>>& board) {
    vector<thread> threads;
    solutionFound = false;
    
    // Create threads for each possible starting number
    for (int num = 1; num <= 9; num++) {
        threads.emplace_back(solveWorker, board, num, ref(board));
    }
    
    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
    
    return solutionFound;
}
