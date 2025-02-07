#include <gtest/gtest.h>

#include "solution.cpp"

using namespace std;

// Helper: isValidCompletedSudoku
// Checks if a 9x9 board is a valid, completed Sudoku (no zeroes, all rules).
bool isValidCompletedSudoku(const vector<vector<int>>& board) {
    // Each row
    for (int r = 0; r < 9; r++) {
        vector<bool> seen(10, false);
        for (int c = 0; c < 9; c++) {
            int val = board[r][c];
            if (val < 1 || val > 9 || seen[val]) {
                return false;
            }
            seen[val] = true;
        }
    }
    // Each column
    for (int c = 0; c < 9; c++) {
        vector<bool> seen(10, false);
        for (int r = 0; r < 9; r++) {
            int val = board[r][c];
            if (val < 1 || val > 9 || seen[val]) {
                return false;
            }
            seen[val] = true;
        }
    }
    // Each 3x3 box
    for (int box = 0; box < 9; box++) {
        vector<bool> seen(10, false);
        int startRow = (box / 3) * 3;
        int startCol = (box % 3) * 3;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                int val = board[startRow + i][startCol + j];
                if (val < 1 || val > 9 || seen[val]) {
                    return false;
                }
                seen[val] = true;
            }
        }
    }
    return true;
}

// Test 1: Solvable Sudoku
TEST(SudokuSolverTests, SolvableSudoku) {
    vector<vector<int>> board = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    EXPECT_TRUE(solveSudoku(board)) << "Expected Sudoku puzzle to be solvable.";
    EXPECT_TRUE(isValidCompletedSudoku(board)) << "Solution does not satisfy Sudoku constraints.";
}

// Test 2: Unsolvable Sudoku
TEST(SudokuSolverTests, UnsolvableSudoku) {
    // This board has conflicting values (two 5's in the same row or column)
    vector<vector<int>> board = {
        {5, 3, 5, 0, 7, 0, 0, 0, 0}, // 5 repeated in row
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    EXPECT_FALSE(solveSudoku(board)) << "Puzzle should be unsolvable due to conflicts.";
}

// Test 3: Already Solved Sudoku
TEST(SudokuSolverTests, AlreadySolvedSudoku) {
    vector<vector<int>> board = {
        {5, 3, 4, 6, 7, 8, 9, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 9}
    };

    // Make a copy to see if solveSudoku changes it
    vector<vector<int>> originalBoard = board;

    EXPECT_TRUE(solveSudoku(board)) << "Should return true for an already solved board.";
    EXPECT_EQ(board, originalBoard) << "Board should remain unchanged if it's already solved.";
}

// Test 4: Empty Sudoku
TEST(SudokuSolverTests, EmptySudoku) {
    vector<vector<int>> board(9, vector<int>(9, 0));  // All zeros
    EXPECT_TRUE(solveSudoku(board)) << "Even an empty Sudoku can be filled in some valid way.";
    EXPECT_TRUE(isValidCompletedSudoku(board)) << "The filled board doesn't meet Sudoku constraints.";
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}