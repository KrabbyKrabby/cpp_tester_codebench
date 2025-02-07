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
        // Iterate through the board to find the position of the zero tile
        for (int i = 0; i < board.size(); ++i) {
            for (int j = 0; j < board[0].size(); ++j) {
                if (board[i][j] == 0) {
                    return make_pair(i, j);
                }
            }
        }
        // If the zero tile is not found, return an invalid position
        return make_pair(-1, -1);
    }

    // Generate possible moves from the current board
    vector<pair<vector<vector<int>>, int>> getNextStates(const vector<vector<int>>& board) {
        vector<pair<vector<vector<int>>, int>> nextStates;
        pair<int, int> zeroPosition = findZero(board);

        // Check if the zero tile can be moved up
        if (zeroPosition.first > 0) {
            vector<vector<int>> upBoard = board;
            swap(upBoard[zeroPosition.first][zeroPosition.second], upBoard[zeroPosition.first - 1][zeroPosition.second]);
            nextStates.push_back(make_pair(upBoard, 3)); // 3 represents moving up
        }

        // Check if the zero tile can be moved down
        if (zeroPosition.first < board.size() - 1) {
            vector<vector<int>> downBoard = board;
            swap(downBoard[zeroPosition.first][zeroPosition.second], downBoard[zeroPosition.first + 1][zeroPosition.second]);
            nextStates.push_back(make_pair(downBoard, 1)); // 1 represents moving down
        }

        // Check if the zero tile can be moved left
        if (zeroPosition.second > 0) {
            vector<vector<int>> leftBoard = board;
            swap(leftBoard[zeroPosition.first][zeroPosition.second], leftBoard[zeroPosition.first][zeroPosition.second - 1]);
            nextStates.push_back(make_pair(leftBoard, 2)); // 2 represents moving left
        }

        // Check if the zero tile can be moved right
        if (zeroPosition.second < board[0].size() - 1) {
            vector<vector<int>> rightBoard = board;
            swap(rightBoard[zeroPosition.first][zeroPosition.second], rightBoard[zeroPosition.first][zeroPosition.second + 1]);
            nextStates.push_back(make_pair(rightBoard, 0)); // 0 represents moving right
        }

        return nextStates;
    }

    // Slider Puzzle 
    vector<int> slidePuzzle(vector<vector<int>> board) {
        int n = board.size();
        int m = board[0].size();
        vector<int> moves;

        // Create the target state
        vector<vector<int>> targetState(n, vector<int>(m));
        int num = 1;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                targetState[i][j] = num;
                num++;
            }
        }
        targetState[n - 1][m - 1] = 0;

        // Check if the initial state is already the target state
        if (board == targetState) {
            return moves;
        }

        // Initialize the BFS queue and visited set
        queue<pair<vector<vector<int>>, vector<int>>> q;
        q.push(make_pair(board, moves));
        unordered_set<string> visited;
        visited.insert(boardToString(board));

        // BFS search
        while (!q.empty()) {
            pair<vector<vector<int>>, vector<int>> current = q.front();
            q.pop();

            // Generate possible moves from the current state
            vector<pair<vector<vector<int>>, int>> nextStates = getNextStates(current.first);

            for (const auto& nextState : nextStates) {
                // Check if the next state is the target state
                if (nextState.first == targetState) {
                    current.second.push_back(nextState.second);
                    return current.second;
                }

                // Check if the next state has been visited
                string nextStateStr = boardToString(nextState.first);
                if (visited.find(nextStateStr) == visited.end()) {
                    visited.insert(nextStateStr);

                    // Add the next state to the queue
                    vector<int> nextMoves = current.second;
                    nextMoves.push_back(nextState.second);
                    q.push(make_pair(nextState.first, nextMoves));
                }
            }
        }

        // If no solution is found, return an empty vector
        return {};
    }
};
