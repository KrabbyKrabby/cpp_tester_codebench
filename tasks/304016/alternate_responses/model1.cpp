#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <queue>

using namespace std;

const int N = 9; // Size of the Sudoku grid
const int NUM_THREADS = 4; // Number of parallel threads

struct SharedState {
    vector<vector<int>> solution;
    atomic<bool> solutionFound{false};
    mutex mtx;
    condition_variable cv;
};

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

// Sequential solver used by individual threads
bool solveSequential(vector<vector<int>>& board, SharedState& state) {
    int row, col;
    
    if (!findEmptyCell(board, row, col)) {
        return true; // Puzzle is solved
    }

    for (int num = 1; num <= 9; num++) {
        if (state.solutionFound) return false; // Another thread found solution
        
        if (isSafe(board, row, col, num)) {
            board[row][col] = num;
            
            if (solveSequential(board, state)) {
                return true;
            }
            
            board[row][col] = 0; // Backtrack
        }
    }
    
    return false;
}

// Thread worker function
void solveWorker(vector<vector<int>> board, int startNum, SharedState& state) {
    int row, col;
    if (!findEmptyCell(board, row, col)) return;
    
    if (isSafe(board, row, col, startNum)) {
        board[row][col] = startNum;
        
        if (solveSequential(board, state)) {
            lock_guard<mutex> lock(state.mtx);
            if (!state.solutionFound) {
                state.solution = board;
                state.solutionFound = true;
                state.cv.notify_all();
            }
        }
    }
}

// Main Sudoku solver function with multithreading
bool solveSudoku(vector<vector<int>>& board) {
    SharedState state;
    vector<thread> threads;
    
    // Create threads for different starting numbers
    for (int num = 1; num <= 9; num++) {
        if (threads.size() >= NUM_THREADS) break;
        threads.emplace_back(solveWorker, board, num, ref(state));
    }
    
    // Wait for solution or all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
    
    if (state.solutionFound) {
        board = state.solution;
        return true;
    }
    
    return false;
}
