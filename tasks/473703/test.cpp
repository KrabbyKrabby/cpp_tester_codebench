#include "gtest/gtest.h"
#include "solution.cpp"

class TestrisTest : public ::testing::Test {
protected:
    Pieces pieces;
    int screenHeight = 2 * 5;


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


    Board *board;

    virtual void SetUp() {
        // Setup code if needed
        // Board board = Board(&pieces, screenHeight);
         board = new Board(&pieces, screenHeight); 
    }

    virtual void TearDown() {
        // Cleanup code if needed
    }
};

// Test to ensure that the block type is returned correctly
TEST_F(TestrisTest, BlockTypeCorrectness) {
    // Check block type for 'Square' piece at rotation 0, position (2, 3)
    EXPECT_EQ(1, pieces.GetBlockType(0, 0, 2, 3)); // Expect 1 (normal block)

    // Check pivot block (block type 2) for 'I' piece at rotation 1, position (2, 2)
    EXPECT_EQ(2, pieces.GetBlockType(1, 1, 2, 2)); // Expect 2 (pivot block)

    // Check an empty block (block type 0) for 'L' piece at rotation 0, position (0, 0)
    EXPECT_EQ(0, pieces.GetBlockType(2, 0, 0, 0)); // Expect 0 (no-block)
}

TEST_F(TestrisTest, InitialPositionCorrectness) {

    // Check initial vertical position for 'I' piece at rotation 3
    EXPECT_EQ(-3, pieces.GetYInitialPosition(1, 3)); // Expected -3 based on array setup

    // Check initial vertical position for 'N mirrored' piece at rotation 2
    EXPECT_EQ(-2, pieces.GetYInitialPosition(5, 3)); // Expected -2 based on array setup
    
    // Check initial horizontal position for 'Square' piece at rotation 0
    EXPECT_EQ(-2, pieces.GetXInitialPosition(0, 0)); // Expected -2 based on array setup

    // Check initial horizontal position for 'I' piece at rotation 1
    EXPECT_EQ(-2, pieces.GetXInitialPosition(1, 1)); // Expected -3 based on array setup

}


// Test to ensure pieces are stored correctly on the board
TEST_F(TestrisTest, StorePieceTest) {
    board->StorePiece(0, 0, 0, 0);  // Store a 'Square' piece at the top left corner
    EXPECT_EQ(board->IsFreeBlock(2, 3), false);  // Expect block at (2, 3) to be filled
    EXPECT_EQ(board->IsFreeBlock(1, 1), true);   // Expect block at (1, 1) to be free
}


// Test to ensure that piece storage respects board boundaries
TEST_F(TestrisTest, PieceStorageBoundaryTest) {

    // Attempt to store a piece partially outside the limits of the board
    bool canStore = board->IsPossibleMovement(0, BOARD_HEIGHT, 0, 0);
    EXPECT_FALSE(canStore);  // Expect that the piece cannot be stored
}

// Test to ensure that lines are correctly deleted from the board
TEST_F(TestrisTest, DeleteLineTest) {
    // Fill a line entirely
    for (int i = 0; i < BOARD_WIDTH; i++) {
        board->StorePiece(i, 0, 0, 0);  // Store a 'Square' piece repeatedly across the bottom line
    }
    board->DeletePossibleLines();
    for (int i = 0; i < BOARD_WIDTH; i++) {
        EXPECT_EQ(board->IsFreeBlock(i, 0), true);  // Expect the bottom line to be empty after deletion
    }
}



TEST_F(TestrisTest, AllRotationsBlockTypeCorrectness)
{

    for (int piece = 0; piece < 7; ++piece) {
        for (int rotation = 0; rotation < 4; ++rotation) {
            // Check every cell in the 5×5
            for (int x = 0; x < 5; ++x) {
                for (int y = 0; y < 5; ++y) {
                    
                    int blockType = pieces.GetBlockType(piece, rotation, x, y);
                    EXPECT_EQ(mPieces[piece][rotation][x][y], blockType);
                }
            }
        }
    }
}



TEST_F(TestrisTest, AllPiecesInitialPositionCorrectness)
{
    // We can simply loop across all 7 pieces, 4 rotations, and compare.
    for (int piece = 0; piece < 7; ++piece) {
        for (int rotation = 0; rotation < 4; ++rotation) {
            int xPos = pieces.GetXInitialPosition(piece, rotation);
            int yPos = pieces.GetYInitialPosition(piece, rotation);

            static const int expected[7][4][2] = {
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

            EXPECT_EQ(expected[piece][rotation][0], xPos)
                << "X initial position mismatch for piece=" << piece
                << ", rotation=" << rotation;
            EXPECT_EQ(expected[piece][rotation][1], yPos)
                << "Y initial position mismatch for piece=" << piece
                << ", rotation=" << rotation;
        }
    }
}