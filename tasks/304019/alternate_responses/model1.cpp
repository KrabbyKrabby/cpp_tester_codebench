#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <unordered_map>

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
        vector<int> moves = {1, 4, 2, 3}; // Corresponding move numbers
        
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
    vector<int> slidePuzzle(vector<vector<int>> board) {
        int rows = board.size();
        int cols = board[0].size();
        
        // Create target board
        vector<vector<int>> target(rows, vector<int>(cols));
        int val = 1;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                target[i][j] = val++ % (rows * cols);
            }
        }
        
        // If board is already solved
        if (board == target) {
            return vector<int>();
        }
        
        queue<vector<vector<int>>> q;
        unordered_set<string> visited;
        unordered_map<string, pair<string, int>> parent; // {current_state -> {parent_state, move}}
        
        q.push(board);
        visited.insert(boardToString(board));
        
        while (!q.empty()) {
            vector<vector<int>> curr = q.front();
            q.pop();
            
            if (curr == target) {
                // Reconstruct path
                vector<int> path;
                string currState = boardToString(curr);
                string initialState = boardToString(board);
                
                while (currState != initialState) {
                    path.push_back(parent[currState].second);
                    currState = parent[currState].first;
                }
                
                reverse(path.begin(), path.end());
                return path;
            }
            
            vector<pair<vector<vector<int>>, int>> nextStates = getNextStates(curr);
            for (const auto& next : nextStates) {
                string nextState = boardToString(next.first);
                if (visited.find(nextState) == visited.end()) {
                    visited.insert(nextState);
                    parent[nextState] = {boardToString(curr), next.second};
                    q.push(next.first);
                }
            }
        }
        
        return vector<int>(); // Return empty vector if no solution exists
    }
};
