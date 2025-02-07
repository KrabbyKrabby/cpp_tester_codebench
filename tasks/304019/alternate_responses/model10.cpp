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
                return make_pair(i, j);
            }
        }
    }
    return make_pair(-1, -1); // Return invalid position if zero not found
}

// Generate possible moves from the current board
vector<pair<vector<vector<int>>, int>> getNextStates(const vector<vector<int>>& board) {
    vector<pair<vector<vector<int>>, int>> nextStates;
    pair<int, int> zeroPos = findZero(board);
    int rows = board.size();
    int cols = board[0].size();
    vector<int> directions = {-1, 0, 1, 0, -1}; // Up, right, down, left

    for (int i = 0; i < 4; ++i) {
        int newRow = zeroPos.first + directions[i];
        int newCol = zeroPos.second + directions[i + 1];
        if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
            vector<vector<int>> newState = board;
            swap(newState[zeroPos.first][zeroPos.second], newState[newRow][newCol]);
            nextStates.push_back(make_pair(newState, i)); // i represents the direction
        }
    }
    return nextStates;
}

// Slider Puzzle 
vector<int> slidePuzzle(vector<vector<int>> board) {
    int rows = board.size();
    int cols = board[0].size();
    string targetState;
    for (int i = 1; i < rows * cols; ++i) {
        targetState += to_string(i);
    }
    targetState += "0";

    unordered_set<string> visited;
    queue<pair<vector<vector<int>>, vector<int>>> q;
    q.push(make_pair(board, vector<int>()));

    while (!q.empty()) {
        pair<vector<vector<int>>, vector<int>> currentState = q.front();
        q.pop();
        string currentStateStr = boardToString(currentState.first);
        if (currentStateStr == targetState) {
            return currentState.second;
        }
        if (visited.find(currentStateStr) != visited.end()) {
            continue;
        }
        visited.insert(currentStateStr);
        vector<pair<vector<vector<int>>, int>> nextStates = getNextStates(currentState.first);
        for (const auto& nextState : nextStates) {
            vector<int> moves = currentState.second;
            moves.push_back(nextState.second); // Add direction to moves
            q.push(make_pair(nextState.first, moves));
        }
    }
    return vector<int>(); // Return empty vector if no solution found
}
};
