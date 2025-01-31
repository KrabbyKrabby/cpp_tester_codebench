#ifndef _TETRIS_MOV__
#define _TETRIS_MOV__

#define BOARD_LINE_WIDTH 6          
#define BLOCK_SIZE 16               
#define BOARD_POSITION 320          
#define BOARD_WIDTH 10              
#define BOARD_HEIGHT 20             
#define MIN_VERTICAL_MARGIN 20      
#define MIN_HORIZONTAL_MARGIN 20    
#define PIECE_BLOCKS 5              

class Pieces {
private:
    int mPieces[7][4][5][5] = {
        // Square
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
        // I
        {
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 1, 2, 1, 1},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 2, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 1, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {1, 1, 2, 1, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 1, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 2, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 0, 0, 0}
            }
        },
        // L
        {
            {
                {0, 0, 0, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 2, 0, 0},
                {0, 0, 1, 1, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 1, 2, 1, 0},
                {0, 1, 0, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 1, 1, 0, 0},
                {0, 0, 2, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 1, 0},
                {0, 1, 2, 1, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            }
        },
        // L mirrored
        {
            {
                {0, 0, 0, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 2, 0, 0},
                {0, 1, 1, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 1, 0, 0, 0},
                {0, 1, 2, 1, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 1, 1, 0},
                {0, 0, 2, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 1, 2, 1, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 0, 0, 0}
            }
        },
        // N
        {
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 2, 1, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 1, 2, 0, 0},
                {0, 0, 1, 1, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 1, 2, 0, 0},
                {0, 1, 0, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 1, 1, 0, 0},
                {0, 0, 2, 1, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            }
        },
        // N mirrored
        {
            {
                {0, 0, 0, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 2, 1, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 2, 1, 0},
                {0, 1, 1, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 1, 0, 0, 0},
                {0, 1, 2, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 1, 1, 0},
                {0, 1, 2, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            }
        },
        // T
        {
            {
                {0, 0, 0, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 2, 1, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 0, 0, 0}
            },
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
                {0, 1, 2, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 1, 2, 1, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            }
        }
    };

    int mPiecesInitialPosition[7][4][2] = {
        // Square
        {{-2, -3}, {-2, -3}, {-2, -3}, {-2, -3}},
        // I
        {{-2, -2}, {-2, -3}, {-2, -2}, {-2, -3}},
        // L
        {{-2, -3}, {-2, -3}, {-2, -3}, {-2, -2}},
        // L mirrored
        {{-2, -3}, {-2, -2}, {-2, -3}, {-2, -3}},
        // N
        {{-2, -3}, {-2, -3}, {-2, -3}, {-2, -2}},
        // N mirrored
        {{-2, -3}, {-2, -3}, {-2, -3}, {-2, -2}},
        // T
        {{-2, -3}, {-2, -3}, {-2, -3}, {-2, -2}}
    };

public:
    int GetBlockType(int pPiece, int pRotation, int pX, int pY) {
        return mPieces[pPiece][pRotation][pX][pY];
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
        InitBoard();
    }

    void StorePiece(int pX, int pY, int pPiece, int pRotation) {
        for (int i = 0; i < PIECE_BLOCKS; i++) {
            for (int j = 0; j < PIECE_BLOCKS; j++) {
                if (mPieces->GetBlockType(pPiece, pRotation, j, i) != 0) {
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
        for (int j = 0; j < BOARD_HEIGHT; j++) {
            int i = 0;
            while (i < BOARD_WIDTH) {
                if (mBoard[i][j] != POS_FILLED) break;
                i++;
            }
            if (i == BOARD_WIDTH) DeleteLine(j);
        }
    }

    bool IsFreeBlock(int pX, int pY) {
        if (pX < 0 || pX > BOARD_WIDTH - 1 || pY < 0 || pY > BOARD_HEIGHT - 1)
            return false;
        return mBoard[pX][pY] == POS_FREE;
    }

    int GetXPosInPixels(int pPos) {
        return (BOARD_POSITION - (BLOCK_SIZE * (BOARD_WIDTH / 2)) + (pPos * BLOCK_SIZE));
    }

    int GetYPosInPixels(int pPos) {
        return (mScreenHeight - (BLOCK_SIZE * BOARD_HEIGHT) + (pPos * BLOCK_SIZE));
    }

    bool IsPossibleMovement(int pX, int pY, int pPiece, int pRotation) {
        for (int i = 0; i < PIECE_BLOCKS; i++) {
            for (int j = 0; j < PIECE_BLOCKS; j++) {
                if (mPieces->GetBlockType(pPiece, pRotation, j, i) != 0) {
                    if (!IsFreeBlock(pX + j, pY + i))
                        return false;
                }
            }
        }
        return true;
    }

private:
    enum { POS_FREE, POS_FILLED };
    int mBoard[BOARD_WIDTH][BOARD_HEIGHT];
    Pieces *mPieces;
    int mScreenHeight;

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