#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <string>
#include <algorithm>

using namespace std;

class PuzzleRay{
public:

    // Helper function to convert the board into a string representation
    string boardToString(const vector<vector<int>>& board) {
        string state;
        for (const auto& row : board) {
            for (int cell : row) {
                state += to_string(cell);
            }
        }
        return state;
    }

    // Find the position of the zero tile
    pair<int, int> findZero(const vector<vector<int>>& board) {
        for (int i = 0; i < board.size(); ++i) {
            for (int j = 0; j < board[0].size(); ++j) {
                if (board[i][j] == 0) {
                    return {i, j};
                }
            }
        }
        return {-1, -1}; // Return invalid position if zero is not found
    }

    // Generate possible moves from the current board
    vector<pair<vector<vector<int>>, int>> getNextStates(const vector<vector<int>>& board) {
        vector<pair<vector<vector<int>>, int>> nextStates;
        pair<int, int> zeroPos = findZero(board);

        // Check all four directions (up, down, left, right)
        vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (const auto& dir : directions) {
            int newRow = zeroPos.first + dir.first;
            int newCol = zeroPos.second + dir.second;

            // Check if the new position is within the board boundaries
            if (newRow >= 0 && newRow < board.size() && newCol >= 0 && newCol < board[0].size()) {
                vector<vector<int>> newState = board;
                swap(newState[zeroPos.first][zeroPos.second], newState[newRow][newCol]);
                nextStates.push_back({newState, dir.first == 0 ? (dir.second == 1 ? 1 : 3) : (dir.first == 1 ? 2 : 0)});
            }
        }

        return nextStates;
    }

    // Slider Puzzle 
    vector<int> slidePuzzle(vector<vector<int>> board) {
        unordered_set<string> visited;
        queue<pair<vector<vector<int>>, vector<int>>> q;
        q.push({board, {}});
        visited.insert(boardToString(board));

        while (!q.empty()) {
            auto [currBoard, moves] = q.front();
            q.pop();

            if (isSolved(currBoard)) {
                return moves;
            }

            for (const auto& [nextBoard, move] : getNextStates(currBoard)) {
                string nextBoardStr = boardToString(nextBoard);
                if (visited.find(nextBoardStr) == visited.end()) {
                    visited.insert(nextBoardStr);
                    vector<int> newMoves = moves;
                    newMoves.push_back(move);
                    q.push({nextBoard, newMoves});
                }
            }
        }

        return {}; // Return empty vector if no solution is found
    }

    // Helper function to check if the puzzle is solved
    bool isSolved(const vector<vector<int>>& board) {
        int expected = 1;
        for (const auto& row : board) {
            for (int cell : row) {
                if (cell != 0 && cell != expected) {
                    return false;
                }
                expected++;
            }
        }
        return true;
    }
};
