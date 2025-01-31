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
        // Square piece
        {
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
        // I piece
        {
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
        // L piece
        {
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
                {0, 1, 2, 2, 2},
                {0, 0, 0, 0, 0},
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
                {0, 0, 0, 0, 0},
                {2, 2, 2, 1, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            }
        },
        // L mirrored piece
        {
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 2, 0},
                {0, 0, 0, 2, 0},
                {0, 1, 2, 2, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {2, 2, 2, 1, 0},
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
                {1, 2, 2, 2, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            }
        },
        // N piece
        {
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 2, 1, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 1, 1, 1, 0},
                {0, 1, 0, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 1, 0, 0, 0},
                {0, 1, 2, 0, 0},
                {0, 1, 0, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 1, 1},
                {0, 1, 1, 1, 0},
                {0, 0, 0, 0, 0}
            }
        },
        // N mirrored piece
        {
            {
                {0, 0, 0, 0, 0},
                {0, 1, 0, 0, 0},
                {0, 1, 2, 0, 0},
                {0, 1, 0, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 1, 1},
                {0, 1, 1, 1, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 2, 1, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 1, 1, 1, 0},
                {0, 1, 0, 0, 0},
                {0, 0, 0, 0, 0}
            }
        },
        // T piece
        {
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 1, 2, 1, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 1, 1, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 1, 2, 1, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 1, 1, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 0, 0, 0}
            }
        }
    };

    int mPiecesInitialPosition[7][4][2] = {
        // Square piece
        {
            {-2, -3},
            {-2, -3},
            {-2, -3},
            {-2, -3}
        },
        // I piece
        {
            {-2, -4},
            {-3, -3},
            {-2, -3},
            {-3, -4}
        },
        // L piece
        {
            {-2, -4},
            {-3, -3},
            {-2, -3},
            {-3, -4}
        },
        // L mirrored piece
        {
            {-2, -4},
            {-3, -3},
            {-2, -3},
            {-3, -4}
        },
        // N piece
        {
            {-2, -4},
            {-3, -3},
            {-2, -3},
            {-3, -4}
        },
        // N mirrored piece
        {
            {-2, -4},
            {-3, -3},
            {-2, -3},
            {-3, -4}
        },
        // T piece
        {
            {-2, -4},
            {-3, -3},
            {-2, -3},
            {-3, -4}
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
        mScreenHeight = pScreenHeight;
        mPieces = pPieces;
        InitBoard();  // Initialize the board blocks with free positions
    }

    void StorePiece(int pX, int pY, int pPiece, int pRotation) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (mPieces->GetBlockType(pPiece, pRotation, i, j) != 0) {
                    mBoard[pX + i][pY + j] = POS_FILLED;
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
                i--;
            }
        }
    }

    bool IsFreeBlock(int pX, int pY) {
        return mBoard[pX][pY] == POS_FREE;
    }

    int GetXPosInPixels(int pPos) {
        return (BOARD_POSITION - (BLOCK_SIZE * (BOARD_WIDTH / 2)) + (pPos * BLOCK_SIZE));
    }

    int GetYPosInPixels(int pPos) {
        return (mScreenHeight - (BLOCK_SIZE * BOARD_HEIGHT) + (pPos * BLOCK_SIZE));
    }

    bool IsPossibleMovement(int pX, int pY, int pPiece, int pRotation) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (mPieces->GetBlockType(pPiece, pRotation, i, j) != 0) {
                    if ((pX + i < 0) || (pX + i >= BOARD_WIDTH) || (pY + j < 0) || (pY + j >= BOARD_HEIGHT)) {
                        return false;
                    }
                    if (mBoard[pX + i][pY + j] == POS_FILLED) {
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
