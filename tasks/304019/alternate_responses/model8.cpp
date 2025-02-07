#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <string>
#include <algorithm>

using namespace std;

class PuzzleRay {
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
            for (int j = 0; j < board[i].size(); ++j) {
                if (board[i][j] == 0) {
                    return {i, j};
                }
            }
        }
        return {-1, -1}; // Return invalid position if zero not found
    }

    // Generate possible moves from the current board
    vector<pair<vector<vector<int>>, int>> getNextStates(const vector<vector<int>>& board) {
        vector<pair<vector<vector<int>>, int>> nextStates;
        auto zeroPos = findZero(board);
        int row = zeroPos.first;
        int col = zeroPos.second;
        vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        for (const auto& dir : directions) {
            int newRow = row + dir.first;
            int newCol = col + dir.second;

            if (newRow >= 0 && newRow < board.size() && newCol >= 0 && newCol < board[0].size()) {
                vector<vector<int>> newState = board;
                swap(newState[row][col], newState[newRow][newCol]);
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
            auto currentState = q.front().first;
            auto moves = q.front().second;
            q.pop();

            if (currentState == vector<vector<int>>({{1, 2, 3}, {4, 5, 6}, {7, 8, 0}})) {
                return moves;
            }

            for (const auto& nextState : getNextStates(currentState)) {
                string nextStateStr = boardToString(nextState.first);
                if (visited.find(nextStateStr) == visited.end()) {
                    visited.insert(nextStateStr);
                    vector<int> newMoves = moves;
                    newMoves.push_back(nextState.second);
                    q.push({nextState.first, newMoves});
                }
            }
        }
        return {}; // Return empty vector if no solution found
    }
};
