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
    // ToDo: Implement findZero function
    // It should locate the position of the empty space (0) on the board.
    // Input: board (current state of the puzzle)
    // Output: pair<int, int> representing the row and column indices of 0.
}

// Generate possible moves from the current board
vector<pair<vector<vector<int>>, int>> getNextStates(const vector<vector<int>>& board) {
    // ToDo: Implement getNextStates function
    // This function should generate all possible next states from the current board by sliding adjacent tiles into the empty space.
    // Input: board (current state of the puzzle)
    // Output: vector<pair<vector<vector<int>>, int>> where each pair contains:
    //          - A new board state after a move.
    //          - The tile number that was moved to reach this new state.
}

// Slider Puzzle 
vector<int> slidePuzzle(vector<vector<int>> board) {
    // ToDo: Implement slidePuzzle function
    // Input: board (current state of the puzzle)
    // Output: vector<int> containing the sequence of tile numbers that need to be moved into the empty space to solve the puzzle.
    // Each inner vector<int> represents a row on the puzzle board.
    // The integer 0 denotes the empty space on the board.
}

};