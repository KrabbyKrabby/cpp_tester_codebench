#include <thread>
#include <gtest/gtest.h>
#include "solution.cpp"


// Test Fixture for PuzzleRayTest
class PuzzleRayTest : public ::testing::Test {
protected:
    PuzzleRay solver;
 

 // Helper function to define the solved state based on board size
    vector<vector<int>> getSolvedBoard(const vector<vector<int>>& board) {
        vector<vector<int>> solvedBoard = board;
        int numRows = board.size();
        int numCols = board[0].size();
        int tile = 1;
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                if (i == numRows - 1 && j == numCols - 1) {
                    solvedBoard[i][j] = 0;
                } else {
                    solvedBoard[i][j] = tile++;
                }
            }
        }
        return solvedBoard;
    }


    vector<vector<int>> applyMoves(vector<vector<int>> board, const vector<int>& moves) {
    pair<int, int> zeroPos = solver.findZero(board);
    for (size_t i = 0; i < moves.size(); ++i) {
        int tile = moves[i];
        pair<int, int> tilePos = make_pair(-1, -1);
        // Find the position of the tile to move
        for (size_t r = 0; r < board.size(); ++r) {
            for (size_t c = 0; c < board[r].size(); ++c) {
                if (board[r][c] == tile) {
                    tilePos = make_pair(r, c);
                    break;
                }
            }
            if (tilePos.first != -1) break;
        }
        // Swap tile with zero
        if (tilePos.first != -1) {
            board[zeroPos.first][zeroPos.second] = tile;
            board[tilePos.first][tilePos.second] = 0;
            zeroPos = tilePos;
        }
    }
    return board;
}

};




TEST_F(PuzzleRayTest, ProvidedExample) {
    vector<vector<int>> initialBoard = {
        {1, 2, 3},
        {4, 0, 5},
        {6, 7, 8}
    };

    // Call slidePuzzle using PuzzleRay instance
    vector<int> actualMoves = solver.slidePuzzle(initialBoard);

    // Apply the moves to verify the final state
    vector<vector<int>> finalBoard = applyMoves(initialBoard, actualMoves);
    vector<vector<int>> solvedBoard = getSolvedBoard(initialBoard);

    EXPECT_EQ(finalBoard, solvedBoard);
}

TEST_F(PuzzleRayTest, EmptySpaceInCorner) {
    vector<vector<int>> initialBoard = {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8}
    };

    // Call slidePuzzle using PuzzleRay instance
    vector<int> actualMoves = solver.slidePuzzle(initialBoard);

    // Apply the moves to verify the final state
    vector<vector<int>> finalBoard = applyMoves(initialBoard, actualMoves);
    vector<vector<int>> solvedBoard = getSolvedBoard(initialBoard);

    EXPECT_EQ(finalBoard, solvedBoard);
}

TEST_F(PuzzleRayTest, AlreadySolved) {
    vector<vector<int>> initialBoard = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}
    };

    // Call slidePuzzle using PuzzleRay instance
    vector<int> actualMoves = solver.slidePuzzle(initialBoard);

    // No moves needed
    vector<int> expectedMoves;
    EXPECT_EQ(actualMoves, expectedMoves);
}

TEST_F(PuzzleRayTest, MaximumMoves3x3) {
    // A puzzle that requires a high number of moves to solve
    vector<vector<int>> initialBoard = {
        {8, 6, 7},
        {2, 5, 4},
        {3, 0, 1}
    };

    // Call slidePuzzle using PuzzleRay instance
    vector<int> actualMoves = solver.slidePuzzle(initialBoard);

    // Apply the moves to verify the final state
    vector<vector<int>> finalBoard = applyMoves(initialBoard, actualMoves);
    vector<vector<int>> solvedBoard = getSolvedBoard(initialBoard);

    EXPECT_EQ(finalBoard, solvedBoard);
}

TEST_F(PuzzleRayTest, DifferentZeroPositions) {
    // Zero in the middle
    vector<vector<int>> initialBoard1 = {
        {1, 2, 3},
        {4, 0, 5},
        {6, 7, 8}
    };

    // Zero in the top-right corner
    vector<vector<int>> initialBoard2 = {
        {1, 2, 0},
        {4, 5, 3},
        {6, 7, 8}
    };

    // Call slidePuzzle using PuzzleRay instance
    vector<int> moves1 = solver.slidePuzzle(initialBoard1);
    vector<int> moves2 = solver.slidePuzzle(initialBoard2);

    // Apply the moves and verify
    vector<vector<int>> finalBoard1 = applyMoves(initialBoard1, moves1);
    vector<vector<int>> finalBoard2 = applyMoves(initialBoard2, moves2);
    vector<vector<int>> solvedBoard = getSolvedBoard(initialBoard1); // Both boards have same solved state

    EXPECT_EQ(finalBoard1, solvedBoard);
    EXPECT_EQ(finalBoard2, solvedBoard);
}

TEST_F(PuzzleRayTest, LargerPuzzle4x4) {
    // Define a 4x4 initial board
    vector<vector<int>> initialBoard = {
        {5, 1, 2, 4},
        {0, 6, 3, 8},
        {9, 10, 7, 12},
        {13, 14, 11, 15}
    };

    // Call slidePuzzle using PuzzleRay instance
    vector<int> actualMoves = solver.slidePuzzle(initialBoard);

    // Apply the moves to verify the final state
    vector<vector<int>> finalBoard = applyMoves(initialBoard, actualMoves);
    vector<vector<int>> solvedBoard = getSolvedBoard(initialBoard);

    EXPECT_EQ(finalBoard, solvedBoard);
}

TEST_F(PuzzleRayTest, OneMoveAway) {
    vector<vector<int>> initialBoard = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 0, 8}
    };

    // Call slidePuzzle using PuzzleRay instance
    vector<int> actualMoves = solver.slidePuzzle(initialBoard);

    // Expected to move tile 8 into the empty space
    vector<int> expectedMoves = {8};
    EXPECT_EQ(actualMoves, expectedMoves);
}

TEST_F(PuzzleRayTest, TwoMovesAway) {
    vector<vector<int>> initialBoard = {
        {1, 2, 3},
        {4, 5, 6},
        {0, 7, 8}
    };

    // Call slidePuzzle using PuzzleRay instance
    vector<int> actualMoves = solver.slidePuzzle(initialBoard);

    // Expected to move tile 7 and then 8 into the empty space
    vector<int> expectedMoves = {7, 8};
    EXPECT_EQ(actualMoves, expectedMoves);
}

TEST_F(PuzzleRayTest, AlreadySolved4x4) {
    vector<vector<int>> initialBoard = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 0}
    };

    // Call slidePuzzle using PuzzleRay instance
    vector<int> actualMoves = solver.slidePuzzle(initialBoard);

    // No moves needed
    vector<int> expectedMoves;
    EXPECT_EQ(actualMoves, expectedMoves);
}


// -------------------------------------------------------------
// New Test Case: Concurrent Puzzle Solving
// -------------------------------------------------------------
TEST_F(PuzzleRayTest, ConcurrentPuzzleSolving) {
    // Define multiple initial boards for concurrent testing
    vector<vector<int>> initialBoards[5] = {
        // Puzzle 1
        {
            {1, 2, 3},
            {4, 0, 5},
            {6, 7, 8}
        },
        // Puzzle 2
        {
            {0, 1, 2},
            {3, 4, 5},
            {6, 7, 8}
        },
        // Puzzle 3
        {
            {1, 2, 3},
            {4, 5, 6},
            {7, 0, 8}
        },
        // Puzzle 4 (4x4)
        {
            {5, 1, 2, 4},
            {0, 6, 3, 8},
            {9, 10, 7, 12},
            {13, 14, 11, 15}
        },
        // Puzzle 5
        {
            {1, 2, 3},
            {4, 5, 6},
            {0, 7, 8}
        }
    };

    // Vector to hold threads
    vector<thread> threads;
    // Vector to hold results
    vector<vector<int>> results(5);
    
    // Lambda function to solve a puzzle and store the result
    auto solvePuzzle = [&](int index) {
        results[index] = solver.slidePuzzle(initialBoards[index]);
    };

    // Launch threads to solve puzzles concurrently
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(solvePuzzle, i);
    }

    // Wait for all threads to complete
    for (auto& th : threads) {
        if (th.joinable()) {
            th.join();
        }
    }

    // Define the solved states for verification
    vector<vector<int>> solvedBoards[5] = {
        // Solved Puzzle 1
        {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 0}
        },
        // Solved Puzzle 2
        {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 0}
        },
        // Solved Puzzle 3
        {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 0}
        },
        // Solved Puzzle 4
        {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12},
            {13, 14, 15, 0}
        },
        // Solved Puzzle 5
        {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 0}
        }
    };

    // Verify each puzzle was solved correctly
    for (int i = 0; i < 5; ++i) {
        vector<vector<int>> finalBoard = applyMoves(initialBoards[i], results[i]);
        EXPECT_EQ(finalBoard, solvedBoards[i]) << "Failed on Puzzle " << i+1;
    }
}