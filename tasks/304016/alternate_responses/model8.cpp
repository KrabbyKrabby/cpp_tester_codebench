#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

const int N = 9; // Size of the Sudoku grid
const int NUM_THREADS = 4; // Number of threads to use

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
    // Find the first empty cell
    int row = 0, col = 0;
    while (row < N && col < N) {
        if (board[row][col] == 0) {
            break;
        }
        col++;
        if (col == N) {
            col = 0;
            row++;
        }
    }

    // If no empty cells, return true
    if (row == N && col == N) {
        return true;
    }

    // Try numbers 1-9
    for (int num = 1; num <= 9; num++) {
        if (isSafe(board, row, col, num)) {
            board[row][col] = num;
            if (solveSudoku(board)) {
                return true;
            }
            board[row][col] = 0; // Backtrack
        }
    }

    return false;
}

// Function to solve the Sudoku using backtracking and multithreading
bool solveSudokuMultithreaded(vector<vector<int>>& board) {
    // Create a mutex and condition variable for synchronization
    mutex mtx;
    condition_variable cv;

    // Create a vector to store the results from each thread
    vector<bool> results(NUM_THREADS, false);

    // Divide the workload among threads
    auto worker = [&](int startRow, int endRow, int threadId) {
        // Create a local copy of the board
        vector<vector<int>> localBoard = board;

        // Solve the Sudoku for the assigned rows
        for (int row = startRow; row < endRow; row++) {
            for (int col = 0; col < N; col++) {
                if (localBoard[row][col] == 0) {
                    // Try numbers 1-9
                    for (int num = 1; num <= 9; num++) {
                        if (isSafe(localBoard, row, col, num)) {
                            localBoard[row][col] = num;
                            if (solveSudoku(localBoard)) {
                                // Update the result and notify other threads
                                {
                                    lock_guard<mutex> lock(mtx);
                                    results[threadId] = true;
                                }
                                cv.notify_all();
                                return;
                            }
                            localBoard[row][col] = 0; // Backtrack
                        }
                    }
                }
            }
        }
    };

    // Create and start the threads
    vector<thread> threads;
    int rowsPerThread = N / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; i++) {
        int startRow = i * rowsPerThread;
        int endRow = (i == NUM_THREADS - 1) ? N : (i + 1) * rowsPerThread;
        threads.emplace_back(worker, startRow, endRow, i);
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }

    // Check if any thread found a solution
    for (bool result : results) {
        if (result) {
            return true;
        }
    }

    return false;
}

