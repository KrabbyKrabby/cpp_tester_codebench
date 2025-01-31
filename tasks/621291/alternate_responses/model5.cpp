#include <iostream>
#include <bitset>
#include <vector>

class DES {
private:
    // Initial Permutation Table
    static const int IP[64];
    // Final Permutation Table
    static const int FP[64];
    // Expansion Table
    static const int E[48];
    // Permutation Table
    static const int P[32];
    // S-boxes
    static const int S[8][4][16];
    // PC-1 Table
    static const int PC1[56];
    // PC-2 Table
    static const int PC2[48];
    // Key Shift Table
    static const int SHIFTS[16];

    std::vector<std::bitset<48>> subKeys;

    // Helper functions
    std::bitset<32> f(std::bitset<32> R, std::bitset<48> k) {
        std::bitset<48> expanded = expand(R);
        expanded ^= k;
        return permute(sBox(expanded));
    }

    std::bitset<48> expand(std::bitset<32> input) {
        std::bitset<48> output;
        for(int i = 0; i < 48; i++) {
            output[47-i] = input[32-E[i]];
        }
        return output;
    }

    std::bitset<32> sBox(std::bitset<48> input) {
        std::bitset<32> output;
        int pos = 0;
        for(int i = 0; i < 8; i++) {
            int row = input[47-6*i]*2 + input[47-6*i-5];
            int col = input[47-6*i-1]*8 + input[47-6*i-2]*4 + 
                     input[47-6*i-3]*2 + input[47-6*i-4];
            int val = S[i][row][col];
            for(int j = 0; j < 4; j++) {
                output[31-pos-j] = (val >> j) & 1;
            }
            pos += 4;
        }
        return output;
    }

    std::bitset<32> permute(std::bitset<32> input) {
        std::bitset<32> output;
        for(int i = 0; i < 32; i++) {
            output[31-i] = input[32-P[i]];
        }
        return output;
    }

    void generateSubKeys(std::bitset<64> key) {
        std::bitset<56> permKey;
        // Apply PC1
        for(int i = 0; i < 56; i++) {
            permKey[55-i] = key[64-PC1[i]];
        }

        std::bitset<28> C, D;
        // Split into C and D
        for(int i = 0; i < 28; i++) {
            C[27-i] = permKey[55-i];
            D[27-i] = permKey[27-i];
        }

        subKeys.clear();
        // Generate 16 subkeys
        for(int i = 0; i < 16; i++) {
            // Perform left shifts
            for(int j = 0; j < SHIFTS[i]; j++) {
                bool c_temp = C[27];
                bool d_temp = D[27];
                C <<= 1;
                D <<= 1;
                C[0] = c_temp;
                D[0] = d_temp;
            }

            // Combine C and D
            std::bitset<56> combined;
            for(int j = 0; j < 28; j++) {
                combined[55-j] = C[27-j];
                combined[27-j] = D[27-j];
            }

            // Apply PC2
            std::bitset<48> subKey;
            for(int j = 0; j < 48; j++) {
                subKey[47-j] = combined[56-PC2[j]];
            }
            subKeys.push_back(subKey);
        }
    }

public:
    std::bitset<64> encrypt(std::bitset<64> plaintext, std::bitset<64> key) {
        generateSubKeys(key);

        // Initial Permutation
        std::bitset<64> permuted;
        for(int i = 0; i < 64; i++) {
            permuted[63-i] = plaintext[64-IP[i]];
        }

        // Split into left and right halves
        std::bitset<32> L, R;
        for(int i = 0; i < 32; i++) {
            L[31-i] = permuted[63-i];
            R[31-i] = permuted[31-i];
        }

        // 16 rounds
        for(int i = 0; i < 16; i++) {
            std::bitset<32> temp = R;
            R = L ^ f(R, subKeys[i]);
            L = temp;
        }

        // Combine R and L (reverse order)
        std::bitset<64> combined;
        for(int i = 0; i < 32; i++) {
            combined[63-i] = R[31-i];
            combined[31-i] = L[31-i];
        }

        // Final Permutation
        std::bitset<64> ciphertext;
        for(int i = 0; i < 64; i++) {
            ciphertext[63-i] = combined[64-FP[i]];
        }

        return ciphertext;
    }

    std::bitset<64> decrypt(std::bitset<64> ciphertext, std::bitset<64> key) {
        generateSubKeys(key);

        // Initial Permutation
        std::bitset<64> permuted;
        for(int i = 0; i < 64; i++) {
            permuted[63-i] = ciphertext[64-IP[i]];
        }

        // Split into left and right halves
        std::bitset<32> L, R;
        for(int i = 0; i < 32; i++) {
            L[31-i] = permuted[63-i];
            R[31-i] = permuted[31-i];
        }

        // 16 rounds in reverse
        for(int i = 15; i >= 0; i--) {
            std::bitset<32> temp = R;
            R = L ^ f(R, subKeys[i]);
            L = temp;
        }

        // Combine R and L (reverse order)
        std::bitset<64> combined;
        for(int i = 0; i < 32; i++) {
            combined[63-i] = R[31-i];
            combined[31-i] = L[31-i];
        }

        // Final Permutation
        std::bitset<64> plaintext;
        for(int i = 0; i < 64; i++) {
            plaintext[63-i] = combined[64-FP[i]];
        }

        return plaintext;
    }
};

// Define the static const arrays
const int DES::IP[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

const int DES::FP[64] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};

const int DES::E[48] = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1
};

const int DES::P[32] = {
    16, 7, 20, 21, 29, 12, 28, 17,
    1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9,
    19, 13, 30, 6, 22, 11, 4, 25
};

const int DES::S[8][4][16] = {
    {
        {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
        {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
        {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
        {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
    },
    {
        {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
        {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
        {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
        {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
    },
    {
        {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
        {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
        {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
        {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
    },
    {
        {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
        {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
        {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
        {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
    },
    {
        {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
        {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
        {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
        {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
    },
    {
        {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
        {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
        {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
        {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
    },
    {
        {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
        {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
        {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
        {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
    },
    {
        {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
        {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
        {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
        {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
    }
};

const int DES::PC1[56] = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4
};

const int DES::PC2[48] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

const int DES::SHIFTS[16] = {
    1, 1, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 1
};