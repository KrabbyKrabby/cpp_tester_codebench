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
        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board[0].size(); j++) {
                if (board[i][j] == 0) {
                    return {i, j};
                }
            }
        }
        return {-1, -1}; // Should never reach here if input is valid
    }

    // Generate possible moves from the current board
    vector<pair<vector<vector<int>>, int>> getNextStates(const vector<vector<int>>& board) {
        vector<pair<vector<vector<int>>, int>> nextStates;
        pair<int, int> zeroPos = findZero(board);
        int row = zeroPos.first;
        int col = zeroPos.second;
        
        // Possible directions: up, right, down, left
        vector<pair<int, int>> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
        
        for (int i = 0; i < directions.size(); i++) {
            int newRow = row + directions[i].first;
            int newCol = col + directions[i].second;
            
            // Check if the new position is within bounds
            if (newRow >= 0 && newRow < board.size() && 
                newCol >= 0 && newCol < board[0].size()) {
                
                // Create new board with the move
                vector<vector<int>> newBoard = board;
                swap(newBoard[row][col], newBoard[newRow][newCol]);
                
                // Store the value that was moved
                int movedValue = board[newRow][newCol];
                nextStates.push_back({newBoard, movedValue});
            }
        }
        
        return nextStates;
    }

    // Slider Puzzle 
    vector<int> slidePuzzle(vector<vector<int>> board) {
        int m = board.size();
        int n = board[0].size();
        
        // Create target state
        string target;
        for (int i = 0; i < m * n - 1; i++) {
            target += to_string(i + 1);
        }
        target += "0";
        
        // Initialize variables for BFS
        queue<pair<vector<vector<int>>, vector<int>>> q;
        unordered_set<string> visited;
        
        // Start BFS
        q.push({board, vector<int>()});
        visited.insert(boardToString(board));
        
        while (!q.empty()) {
            auto current = q.front();
            q.pop();
            
            vector<vector<int>> currentBoard = current.first;
            vector<int> moves = current.second;
            
            // Check if current state is the target state
            if (boardToString(currentBoard) == target) {
                return moves;
            }
            
            // Generate and process next possible states
            for (const auto& nextState : getNextStates(currentBoard)) {
                string nextStateStr = boardToString(nextState.first);
                
                if (visited.find(nextStateStr) == visited.end()) {
                    visited.insert(nextStateStr);
                    vector<int> newMoves = moves;
                    newMoves.push_back(nextState.second);
                    q.push({nextState.first, newMoves});
                }
            }
        }
        
        // If no solution is found
        return vector<int>{-1};
    }
};
