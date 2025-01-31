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
        // Define all Tetris pieces with their rotations using a 5x5 block matrix for each rotation.
        // Each of the 7 pieces  (square, I, L, L mirrored, N, N mirrored, T) will have 4 rotation configurations, defined in a 4x5x5 array.
        {
            // Square
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 2, 1, 0},
                {0, 0, 1, 1, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 2, 1, 0},
                {0, 0, 1, 1, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 2, 1, 0},
                {0, 0, 1, 1, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 2, 1, 0},
                {0, 0, 1, 1, 0},
                {0, 0, 0, 0, 0}
            }
        },
        {
            // I
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 1, 1, 1, 1},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 1, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {1, 1, 1, 1, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 1, 0, 0, 0},
                {0, 1, 0, 0, 0},
                {0, 1, 0, 0, 0},
                {0, 1, 0, 0, 0},
                {0, 0, 0, 0, 0}
            }
        },
        {
            // L
            {
                {0, 0, 0, 0, 0},
                {0, 0, 2, 0, 0},
                {0, 0, 2, 0, 0},
                {0, 0, 2, 1, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 2, 2, 2, 0},
                {0, 2, 0, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 1, 2, 0, 0},
                {0, 0, 2, 0, 0},
                {0, 0, 2, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 2, 0},
                {0, 2, 2, 2, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            }
        },
        {
            // L mirrored
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 2, 0},
                {0, 0, 0, 2, 0},
                {0, 1, 2, 2, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 2, 0, 0, 0},
                {0, 2, 2, 2, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 2, 1, 0},
                {0, 0, 2, 0, 0},
                {0, 0, 2, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 2, 2, 2, 0},
                {0, 0, 0, 2, 0},
                {0, 0, 0, 0, 0}
            }
        },
        {
            // N
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 2, 1, 0},
                {0, 0, 2, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 2, 2, 0, 0},
                {0, 0, 2, 1, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 2, 0, 0},
                {0, 2, 1, 0, 0},
                {0, 1, 0, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 1, 2, 0, 0},
                {0, 0, 2, 2, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            }
        },
        {
            // N mirrored
            {
                {0, 0, 0, 0, 0},
                {0, 1, 0, 0, 0},
                {0, 1, 2, 0, 0},
                {0, 0, 2, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 2, 2, 0},
                {0, 1, 2, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 2, 0},
                {0, 0, 1, 2, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 2, 1, 0},
                {0, 2, 2, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            }
        },
        {
            // T
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 1, 1, 1, 0},
                {0, 0, 2, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 2, 1, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 2, 0, 0},
                {0, 1, 1, 1, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 1, 0, 0, 0},
                {0, 1, 2, 0, 0},
                {0, 1, 0, 0, 0},
                {0, 0, 0, 0, 0}
            }
        }
    };

    int mPiecesInitialPosition[7][4][2] = {
        // Define initial positions for each piece and rotation to properly align them on the game board when spawned.
        {
            // Square
            {-2, -3},
            {-2, -3},
            {-2, -3},
            {-2, -3}
        },
        {
            // I
            {-2, -3},
            {-3, -2},
            {-2, -3},
            {-3, -2}
        },
        {
            // L
            {-2, -3},
            {-2, -2},
            {-2, -3},
            {-3, -2}
        },
        {
            // L mirrored
            {-2, -3},
            {-3, -2},
            {-2, -3},
            {-2, -2}
        },
        {
            // N
            {-2, -3},
            {-3, -2},
            {-2, -3},
            {-2, -2}
        },
        {
            // N mirrored
            {-2, -3},
            {-2, -2},
            {-2, -3},
            {-3, -2}
        },
        {
            // T
            {-2, -3},
            {-2, -2},
            {-2, -3},
            {-2, -2}
        }
    };

public:
    int GetBlockType(int pPiece, int pRotation, int pX, int pY) {
        return mPieces[pPiece][pRotation][pY][pX];
    }

    int GetXInitialPosition(int pPiece, int pRotation) {
        return mPiecesInitialPosition[pPiece][pRotation][0];
    }

    int GetYInitialPosition(int pPiece, int pRotation) {
        return mPiecesInitialPosition[pPiece][pRotation][1];
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
        // Store a piece on the board at position (pX, pY) with specified rotation
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                int blockType = mPieces->GetBlockType(pPiece, pRotation, j, i);
                if (blockType != 0) {
                    mBoard[pX + j][pY + i] = POS_FILLED;
                }
            }
        }
    }

    bool IsGameOver() {
        for (int i = 0; i < BOARD_WIDTH; i++) {
            if (mBoard[i][0] == POS_FILLED) return true;
        }
        return false;
    }

    void DeletePossibleLines() {
        // Check each line and delete if filled across the entire width
        for (int i = 0; i < BOARD_HEIGHT; i++) {
            bool isLineFilled = true;
            for (int j = 0; j < BOARD_WIDTH; j++) {
                if (mBoard[j][i] == POS_FREE) {
                    isLineFilled = false;
                    break;
                }
            }
            if (isLineFilled) {
                DeleteLine(i);
            }
        }
    }

    bool IsFreeBlock(int pX, int pY) {
        // Check if a specified block position is free
        if (pX < 0 || pX >= BOARD_WIDTH || pY < 0 || pY >= BOARD_HEIGHT) {
            return false;
        }
        return mBoard[pX][pY] == POS_FREE;
    }

    int GetXPosInPixels(int pPos) {
        return (BOARD_POSITION - (BLOCK_SIZE * (BOARD_WIDTH / 2)) + (pPos * BLOCK_SIZE));
    }

    int GetYPosInPixels(int pPos) {
        return (mScreenHeight - (BLOCK_SIZE * BOARD_HEIGHT) + (pPos * BLOCK_SIZE));
    }

    bool IsPossibleMovement(int pX, int pY, int pPiece, int pRotation) {
        // Determine if a piece is outside the limits of the board
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                int blockType = mPieces->GetBlockType(pPiece, pRotation, j, i);
                if (blockType != 0) {
                    int x = pX + j;
                    int y = pY + i;
                    if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT || mBoard[x][y] == POS_FILLED) {
                        return false;
                    }
                }
            }
        }
        return true;
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
