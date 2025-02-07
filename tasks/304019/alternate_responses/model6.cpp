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
            for (int j = 0; j < board[i].size(); ++j) {
                if (board[i][j] == 0) {
                    return {i, j};
                }
            }
        }
        return {-1, -1}; // Return invalid position if 0 is not found
    }

    // Generate possible moves from the current board
    vector<pair<vector<vector<int>>, int>> getNextStates(const vector<vector<int>>& board) {
        vector<pair<vector<vector<int>>, int>> nextStates;
        pair<int, int> zeroPos = findZero(board);
        int rows = board.size();
        int cols = board[0].size();

        // Check all possible moves (up, down, left, right)
        vector<pair<int, int>> moves = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (int i = 0; i < moves.size(); ++i) {
            int newRow = zeroPos.first + moves[i].first;
            int newCol = zeroPos.second + moves[i].second;

            // Check if the move is valid
            if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
                vector<vector<int>> newBoard = board;
                swap(newBoard[zeroPos.first][zeroPos.second], newBoard[newRow][newCol]);
                nextStates.push_back({newBoard, i}); // i represents the direction of the move
            }
        }
        return nextStates;
    }

    // Slider Puzzle 
    vector<int> slidePuzzle(vector<vector<int>> board) {
        int rows = board.size();
        int cols = board[0].size();
        vector<int> goalState(rows * cols);
        iota(goalState.begin(), goalState.end(), 0);

        unordered_set<string> visited;
        queue<pair<vector<vector<int>>, vector<int>>> bfsQueue;
        bfsQueue.push({board, {}});
        visited.insert(boardToString(board));

        while (!bfsQueue.empty()) {
            auto [currentState, moves] = bfsQueue.front();
            bfsQueue.pop();

            // Check if the current state is the goal state
            if (boardToString(currentState) == boardToString(vector<vector<int>>(goalState.begin(), goalState.end()))) {
                return moves;
            }

            // Generate next states and add them to the queue
            for (auto [nextState, direction] : getNextStates(currentState)) {
                string nextStateStr = boardToString(nextState);
                if (visited.find(nextStateStr) == visited.end()) {
                    visited.insert(nextStateStr);
                    vector<int> newMoves = moves;
                    newMoves.push_back(direction);
                    bfsQueue.push({nextState, newMoves});
                }
            }
        }

        // If no solution is found, return an empty vector
        return {};
    }

};
