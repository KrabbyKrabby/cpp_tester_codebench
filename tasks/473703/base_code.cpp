#ifndef _TETRIS_MOV__
#define _TETRIS_MOV__


#define BOARD_LINE_WIDTH 6          // Width of each of the two lines that delimit the board
#define BLOCK_SIZE 16               // Width and Height of each block of a piece
#define BOARD_POSITION 320          // Center position of the board from the left of the screen
#define BOARD_WIDTH 10              // Board width in blocks 
#define BOARD_HEIGHT 20             // Board height in blocks
#define MIN_VERTICAL_MARGIN 20      // Minimum vertical margin for the board limit      
#define MIN_HORIZONTAL_MARGIN 20    // Minimum horizontal margin for the board limit
#define PIECE_BLOCKS 5              // Number of horizontal and vertical blocks of a matrix piece

class Pieces {
private:
    int mPieces[7][4][5][5] = {
        // TODO: Define all Tetris pieces with their rotations using a 5x5 block matrix for each rotation.
        // Each of the 7 pieces  (square, I, L, L mirrored, N, N mirrored, T) will have 4 rotation configurations, defined in a 4x5x5 array.
        // Example for the 'Square' piece, which has the same configuration for all rotations:
        {
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 2, 1, 0},
                {0, 0, 1, 1, 0},
                {0, 0, 0, 0, 0}
            },
            // Repeat this block for each of the 4 rotations
            // ...
        },
        // Define configurations for other pieces (I, L, L mirrored, N, N mirrored, T)
    };

    int mPiecesInitialPosition[7][4][2] = {
        // TODO: Define initial positions for each piece and rotation to properly align them on the game board when spawned.
        // The initial position offsets are used to handle the top and left positioning of pieces when first created.
        // Example initial positions for the 'Square' piece:
        {
            {-2, -3},
            {-2, -3},
            {-2, -3},
            {-2, -3}
        },
        // Define initial positions for other pieces
    };

public:
    int GetBlockType(int pPiece, int pRotation, int pX, int pY) {
        // TODO: Implement method to return the block type of the piece at the specified coordinates and rotation.
    }

    int GetXInitialPosition(int pPiece, int pRotation) {
        // TODO: Implement method to return the initial X position (horizontal displacement) of the piece based on its rotation.
    }

    int GetYInitialPosition(int pPiece, int pRotation) {
        // TODO: Implement method to return the initial Y position (vertical displacement) of the piece based on its rotation.
    }
};


class Board {
public:

    Board (Pieces *pPieces, int pScreenHeight) {
        // Initialize member variables with given parameters
        mScreenHeight = pScreenHeight;
        mPieces = pPieces;
        InitBoard();  // Initialize the board blocks with free positions
    }

    void StorePiece(int pX, int pY, int pPiece, int pRotation) {
        // TODO: Implement storing a piece on the board at position (pX, pY) with specified rotation

    }

    bool IsGameOver() {
        for (int i = 0; i < BOARD_WIDTH; i++) {
            if (mBoard[i][0] == POS_FILLED) return true;
        }
        return false;
    }

    void DeletePossibleLines() {
        // TODO: Check each line and delete if filled across the entire width

    }

    bool IsFreeBlock(int pX, int pY) {
        // TODO: Check if a specified block position is free
    }

    int GetXPosInPixels(int pPos) {
        return (BOARD_POSITION - (BLOCK_SIZE * (BOARD_WIDTH / 2)) + (pPos * BLOCK_SIZE));
    }

    int GetYPosInPixels(int pPos) {
        return (mScreenHeight - (BLOCK_SIZE * BOARD_HEIGHT) + (pPos * BLOCK_SIZE));
    }

    bool IsPossibleMovement(int pX, int pY, int pPiece, int pRotation) {
        // TODO: Determine if a piece is outside the limits of the board
    }

private:
    enum { POS_FREE, POS_FILLED };  // Board position states
    int mBoard[BOARD_WIDTH][BOARD_HEIGHT];  // Board grid
    Pieces *mPieces;  // Reference to the pieces class for accessing piece information
    int mScreenHeight;  // Screen height for positioning calculations

    void InitBoard() {
        for (int i = 0; i < BOARD_WIDTH; i++)
            for (int j = 0; j < BOARD_HEIGHT; j++)
                mBoard[i][j] = POS_FREE;
    }

    void DeleteLine(int pY) {
        for (int j = pY; j > 0; j--) {
            for (int i = 0; i < BOARD_WIDTH; i++) {
                mBoard[i][j] = mBoard[i][j-1];
            }
        }
    }
};

#endif // _TETRIS_MOV__