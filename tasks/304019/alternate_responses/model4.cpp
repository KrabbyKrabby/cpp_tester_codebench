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
        int rows = board.size();
        int cols = board[0].size();
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
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
        vector<int> moves = {1, 2, 3, 4}; // corresponding move numbers
        
        for (int i = 0; i < 4; i++) {
            int newRow = row + directions[i].first;
            int newCol = col + directions[i].second;
            
            // Check if the new position is within bounds
            if (newRow >= 0 && newRow < board.size() && 
                newCol >= 0 && newCol < board[0].size()) {
                
                // Create new board with the move
                vector<vector<int>> newBoard = board;
                swap(newBoard[row][col], newBoard[newRow][newCol]);
                
                // Add the new state and the move number
                nextStates.push_back({newBoard, moves[i]});
            }
        }
        
        return nextStates;
    }

    // Slider Puzzle 
    vector<int> slidePuzzle(vector<vector<int>> board) {
        int rows = board.size();
        int cols = board[0].size();
        
        // Create target board state
        vector<int> target;
        for (int i = 1; i < rows * cols; i++) {
            target.push_back(i);
        }
        target.push_back(0);
        
        queue<pair<vector<vector<int>>, vector<int>>> q;
        unordered_set<string> visited;
        
        // Initialize queue with starting board and empty moves list
        q.push({board, vector<int>()});
        visited.insert(boardToString(board));
        
        while (!q.empty()) {
            auto current = q.front();
            q.pop();
            
            // Check if current state matches target
            vector<int> flatBoard;
            for (const auto& row : current.first) {
                flatBoard.insert(flatBoard.end(), row.begin(), row.end());
            }
            
            if (flatBoard == target) {
                return current.second;
            }
            
            // Generate and process next possible states
            for (const auto& nextState : getNextStates(current.first)) {
                string boardStr = boardToString(nextState.first);
                
                if (visited.find(boardStr) == visited.end()) {
                    vector<int> newMoves = current.second;
                    newMoves.push_back(nextState.second);
                    
                    q.push({nextState.first, newMoves});
                    visited.insert(boardStr);
                }
            }
        }
        
        return vector<int>{-1}; // Return -1 if no solution found
    }
};
