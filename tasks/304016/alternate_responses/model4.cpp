#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <queue>

using namespace std;

const int N = 9; // Size of the Sudoku grid
const int NUM_THREADS = 4; // Number of threads to use

struct ThreadPool {
    vector<thread> threads;
    queue<function<void()>> tasks;
    mutex queue_mutex;
    condition_variable condition;
    atomic<bool> stop{false};
    atomic<bool> solution_found{false};
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
bool solveSequential(vector<vector<int>>& board, atomic<bool>& solution_found) {
    int row, col;
    
    if (!findEmptyCell(board, row, col)) {
        return true; // Puzzle is solved
    }

    for (int num = 1; num <= 9; num++) {
        if (solution_found) return false; // Another thread found a solution
        
        if (isSafe(board, row, col, num)) {
            board[row][col] = num;
            
            if (solveSequential(board, solution_found)) {
                solution_found = true;
                return true;
            }
            
            board[row][col] = 0; // Backtrack
        }
    }
    
    return false;
}

// Main solving function that implements multithreading
bool solveSudoku(vector<vector<int>>& board) {
    ThreadPool pool;
    vector<vector<vector<int>>> boards(9, board); // Create copies for each possible first value
    atomic<bool> solution_found{false};
    mutex result_mutex;
    
    // Find first empty cell
    int first_row, first_col;
    if (!findEmptyCell(board, first_row, first_col)) {
        return true; // Already solved
    }
    
    // Create threads for different starting values
    vector<thread> solving_threads;
    for (int num = 1; num <= 9; num++) {
        if (isSafe(boards[num-1], first_row, first_col, num)) {
            solving_threads.emplace_back([&, num, first_row, first_col]() {
                vector<vector<int>> local_board = boards[num-1];
                local_board[first_row][first_col] = num;
                
                if (solveSequential(local_board, solution_found)) {
                    lock_guard<mutex> lock(result_mutex);
                    if (!solution_found.load()) {
                        board = local_board;
                        solution_found = true;
                    }
                }
            });
        }
    }
    
    // Wait for all threads to complete
    for (auto& thread : solving_threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    
    return solution_found;
}
