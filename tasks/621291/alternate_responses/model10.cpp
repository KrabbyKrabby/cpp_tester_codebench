#include <iostream>
#include <bitset>
#include <string>

class DES {
private:
    // Permutation tables for initial and final permutations
    const unsigned char IP[64] = {
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7
    };

    const unsigned char FP[64] = {
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25
    };

    // S-boxes
    const unsigned char S[8][4][16] = {
        {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
         {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
         {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
         {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},

        {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
         {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
         {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
         {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},

        {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
         {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
         {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
         {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},

        {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
         {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
         {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
         {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},

        {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
         {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
         {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
         {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},

        {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
         {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
         {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
         {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},

        {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
         {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
         {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
         {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},

        {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
         {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
         {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
         {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}
    };

    // Function to perform initial permutation
    std::bitset<64> initialPermutation(const std::bitset<64>& input) {
        std::bitset<64> output;
        for (int i = 0; i < 64; ++i) {
            output[i] = input[IP[i] - 1];
        }
        return output;
    }

    // Function to perform final permutation
    std::bitset<64> finalPermutation(const std::bitset<64>& input) {
        std::bitset<64> output;
        for (int i = 0; i < 64; ++i) {
            output[i] = input[FP[i] - 1];
        }
        return output;
    }

    // Function to perform S-box substitution
    std::bitset<4> sBoxSubstitution(const std::bitset<6>& input, int sBoxIndex) {
        int row = (input[0].to_ulong() << 1) | input[5].to_ulong();
        int col = (input[1].to_ulong() << 3) | (input[2].to_ulong() << 2) | (input[3].to_ulong() << 1) | input[4].to_ulong();
        return std::bitset<4>(S[sBoxIndex][row][col]);
    }

    // Function to perform DES encryption round
    std::bitset<64> desRound(const std::bitset<64>& input, const std::bitset<48>& key) {
        std::bitset<32> left = input.to_ulong() >> 32;
        std::bitset<32> right = input.to_ulong() & 0xFFFFFFFF;

        std::bitset<48> expandedRight;
        for (int i = 0; i < 48; ++i) {
            expandedRight[i] = right[(i % 32)];
        }

        std::bitset<48> xored = expandedRight ^ key;

        std::bitset<32> substituted;
        for (int i = 0; i < 8; ++i) {
            std::bitset<6> inputToSBox;
            for (int j = 0; j < 6; ++j) {
                inputToSBox[j] = xored[(i * 6) + j];
            }
            std::bitset<4> outputFromSBox = sBoxSubstitution(inputToSBox, i);
            for (int j = 0; j < 4; ++j) {
                substituted[(i * 4) + j] = outputFromSBox[j];
            }
        }

        std::bitset<32> permuted;
        for (int i = 0; i < 32; ++i) {
            permuted[i] = substituted[(i + 2) % 32];
        }

        std::bitset<32> xoredLeft = left ^ permuted;

        std::bitset<64> output;
        for (int i = 0; i < 32; ++i) {
            output[i] = xoredLeft[i];
        }
        for (int i = 0; i < 32; ++i) {
            output[i + 32] = right[i];
        }

        return output;
    }

public:
    // Function to perform DES encryption
    std::bitset<64> encrypt(const std::bitset<64>& plaintext, const std::bitset<64>& key) {
        std::bitset<64> permutedPlaintext = initialPermutation(plaintext);

        std::bitset<48> roundKey;
        for (int i = 0; i < 48; ++i) {
            roundKey[i] = key[i];
        }

        std::bitset<64> ciphertext = permutedPlaintext;
        for (int i = 0; i < 16; ++i) {
            ciphertext = desRound(ciphertext, roundKey);
            // Rotate the key for the next round
            roundKey = (roundKey.to_ulong() << 1) | (roundKey.to_ulong() >> 47);
        }

        return finalPermutation(ciphertext);
    }

    // Function to perform DES decryption
    std::bitset<64> decrypt(const std::bitset<64>& ciphertext, const std::bitset<64>& key) {
        // DES decryption is the same as encryption with the key schedule reversed
        std::bitset<48> roundKey;
        for (int i = 0; i < 48; ++i) {
            roundKey[i] = key[i];
        }
        // Rotate the key for the first round
        for (int i = 0; i < 16; ++i) {
            roundKey = (roundKey.to_ulong() << 1) | (roundKey.to_ulong() >> 47);
        }

        std::bitset<64> plaintext = initialPermutation(ciphertext);
        for (int i = 0; i < 16; ++i) {
            plaintext = desRound(plaintext, roundKey);
            // Rotate the key for the next round
            roundKey = (roundKey.to_ulong() >> 1) | (roundKey.to_ulong() << 47);
        }

        return finalPermutation(plaintext);
    }
};
