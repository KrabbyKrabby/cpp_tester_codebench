#ifndef PUZZLERAY_H
#define PUZZLERAY_H

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <utility>
#include <sstream>   // For stringstream
#include <mutex> 

using namespace std;

class PuzzleRay {
public:


// Constructor: Initialize the mutex
    PuzzleRay() {
    }

// Destructor: Destroy the mutex
    ~PuzzleRay() {
    }

    // Helper function to convert the board into a string representation
    string boardToString(const vector<vector<int>>& board) {
        string state;
        for (size_t i = 0; i < board.size(); ++i) {
            for (size_t j = 0; j < board[i].size(); ++j) {
                // Using stringstream for C++98 compatibility
                stringstream ss;
                ss << board[i][j];
                state += ss.str();
            }
        }
        return state;
    }

// Find the position of the zero tile
    pair<int, int> findZero(const vector<vector<int>>& board)  {
        for (size_t i = 0; i < board.size(); ++i) {
            for (size_t j = 0; j < board[i].size(); ++j) {
                if (board[i][j] == 0) {
                    return make_pair(i, j);
                }
            }
        }
        // If zero is not found, return an invalid position
        return make_pair(-1, -1);
    }

    // Generate possible moves from the current board
    vector<pair<vector<vector<int>>, int>> getNextStates(const vector<vector<int>>& board)  {
        vector<pair<vector<vector<int>>, int>> nextStates;
        pair<int, int> zeroPos = findZero(board);
        int zeroRow = zeroPos.first;
        int zeroCol = zeroPos.second;

        if (zeroRow == -1 || zeroCol == -1) {
            // Invalid board state
            return nextStates;
        }

        // Define possible directions: up, down, left, right
        int directions[4][2] = {
            {-1, 0}, // Up
            {1, 0},  // Down
            {0, -1}, // Left
            {0, 1}   // Right
        };

        for (int d = 0; d < 4; ++d) {
            int newRow = zeroRow + directions[d][0];
            int newCol = zeroCol + directions[d][1];

            // Check if the new position is within bounds
            if (newRow >= 0 && newRow < (int)board.size() &&
                newCol >= 0 && newCol < (int)board[0].size()) {

                // Create a new board state by swapping zero with the adjacent tile
                vector<vector<int>> newBoard = board;
                int movedTile = newBoard[newRow][newCol];
                newBoard[zeroRow][zeroCol] = movedTile;
                newBoard[newRow][newCol] = 0;

                // Add the new state and the moved tile number to the nextStates
                nextStates.push_back(make_pair(newBoard, movedTile));
            }
        }

        return nextStates;
    }

    // Slider Puzzle: Solves the puzzle using BFS and returns the sequence of moves
    vector<int> slidePuzzle(vector<vector<int>> board) {
        // Lock the mutex to ensure thread safety
       lock_guard<mutex> lock(mutex);

        // Define the solved state based on the size of the board
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

        string solvedStateStr = boardToString(solvedBoard);

        // BFS initialization
        queue<pair<vector<vector<int>>, vector<int>>> q;
        unordered_set<string> visited;

        // Enqueue the initial state
        vector<int> initialMoves;
        q.push(make_pair(board, initialMoves));
        visited.insert(boardToString(board));

        while (!q.empty()) {
            pair<vector<vector<int>>, vector<int>> current = q.front();
            q.pop();

            vector<vector<int>> currentBoard = current.first;
            vector<int> currentMoves = current.second;

            string currentStateStr = boardToString(currentBoard);

            // Check if the current state is the solved state
            if (currentStateStr == solvedStateStr) {
                // Unlock the mutex before returning
                lock_guard<mutex> unlock(mutex);
                return currentMoves;
            }

            // Generate all possible next states
            vector<pair<vector<vector<int>>, int>> nextStates = getNextStates(currentBoard);

            for (size_t i = 0; i < nextStates.size(); ++i) {
                vector<vector<int>> nextBoard = nextStates[i].first;
                int movedTile = nextStates[i].second;
                string nextStateStr = boardToString(nextBoard);

                // If the state hasn't been visited, enqueue it
                if (visited.find(nextStateStr) == visited.end()) {
                    visited.insert(nextStateStr);
                    vector<int> nextMoves = currentMoves;
                    nextMoves.push_back(movedTile);
                    q.push(make_pair(nextBoard, nextMoves));
                }
            }
        }

        // Unlock the mutex before returning
        lock_guard<mutex> unlock(mutex);

        // If the puzzle is solvable, this line should never be reached
        return vector<int>();
    }



private:
    // Mutex for thread safety
    // pthread_mutex_t mutex_;
    mutable mutex mutex_;
};


#endif // PUZZLERAY_H