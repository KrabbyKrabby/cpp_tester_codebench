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
    string boardToString(const vector<vector<int>>& board) const {
        string state;
        for (const auto& row : board) {
            for (int cell : row) {
                state += to_string(cell);
            }
        }
        return state;
    }

    // Find the position of the zero tile
    pair<int, int> findZero(const vector<vector<int>>& board) const {
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
    vector<pair<vector<vector<int>>, int>> getNextStates(const vector<vector<int>>& board) const {
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
            
            if (newRow >= 0 && newRow < board.size() && 
                newCol >= 0 && newCol < board[0].size()) {
                vector<vector<int>> newBoard = board;
                swap(newBoard[row][col], newBoard[newRow][newCol]);
                nextStates.push_back({newBoard, moves[i]});
            }
        }
        
        return nextStates;
    }

    // Slider Puzzle 
    vector<int> slidePuzzle(vector<vector<int>> board) const {
        int rows = board.size();
        int cols = board[0].size();
        
        // Check if puzzle is already solved
        string target;
        for (int i = 0; i < rows * cols - 1; i++) {
            target += to_string(i + 1);
        }
        target += "0";
        
        string initial = boardToString(board);
        if (initial == target) {
            return vector<int>();
        }
        
        // BFS queue and visited set
        queue<pair<vector<vector<int>>, vector<int>>> q;
        unordered_set<string> visited;
        
        q.push({board, vector<int>()});
        visited.insert(boardToString(board));
        
        while (!q.empty()) {
            auto current = q.front();
            q.pop();
            
            vector<vector<int>> currentBoard = current.first;
            vector<int> moves = current.second;
            
            // Get all possible next states
            auto nextStates = getNextStates(currentBoard);
            
            for (const auto& state : nextStates) {
                vector<vector<int>> nextBoard = state.first;
                string nextBoardStr = boardToString(nextBoard);
                
                if (visited.find(nextBoardStr) == visited.end()) {
                    vector<int> newMoves = moves;
                    newMoves.push_back(state.second);
                    
                    if (nextBoardStr == target) {
                        return newMoves;
                    }
                    
                    q.push({nextBoard, newMoves});
                    visited.insert(nextBoardStr);
                }
            }
        }
        
        return vector<int>(); // Return empty vector if no solution found
    }
};
