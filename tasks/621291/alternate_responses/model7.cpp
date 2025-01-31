#include <iostream>
#include <bitset>
#include <vector>

// Define the DES class
class DES {
public:
    // Function to encrypt the plaintext using the provided key
    std::bitset<64> encrypt(std::bitset<64> plaintext, std::bitset<64> key) {
        // Perform initial permutation on the plaintext
        std::bitset<64> permutedPlaintext = initialPermutation(plaintext);

        // Divide the plaintext into left and right halves
        std::bitset<32> leftHalf = permutedPlaintext.to_ulong() >> 32;
        std::bitset<32> rightHalf = permutedPlaintext.to_ulong() & 0xFFFFFFFF;

        // Perform 16 rounds of encryption
        for (int i = 0; i < 16; i++) {
            // Generate the subkey for this round
            std::bitset<48> subkey = generateSubkey(key, i);

            // Perform the round function
            std::bitset<32> roundResult = roundFunction(rightHalf, subkey);

            // XOR the round result with the left half
            leftHalf ^= roundResult;

            // Swap the left and right halves
            std::swap(leftHalf, rightHalf);
        }

        // Combine the left and right halves
        std::bitset<64> ciphertext = (leftHalf.to_ulong() << 32) | rightHalf.to_ulong();

        // Perform the final permutation on the ciphertext
        ciphertext = finalPermutation(ciphertext);

        return ciphertext;
    }

    // Function to decrypt the ciphertext using the provided key
    std::bitset<64> decrypt(std::bitset<64> ciphertext, std::bitset<64> key) {
        // Perform initial permutation on the ciphertext
        std::bitset<64> permutedCiphertext = initialPermutation(ciphertext);

        // Divide the ciphertext into left and right halves
        std::bitset<32> leftHalf = permutedCiphertext.to_ulong() >> 32;
        std::bitset<32> rightHalf = permutedCiphertext.to_ulong() & 0xFFFFFFFF;

        // Perform 16 rounds of decryption
        for (int i = 15; i >= 0; i--) {
            // Generate the subkey for this round
            std::bitset<48> subkey = generateSubkey(key, i);

            // Perform the round function
            std::bitset<32> roundResult = roundFunction(rightHalf, subkey);

            // XOR the round result with the left half
            leftHalf ^= roundResult;

            // Swap the left and right halves
            std::swap(leftHalf, rightHalf);
        }

        // Combine the left and right halves
        std::bitset<64> plaintext = (leftHalf.to_ulong() << 32) | rightHalf.to_ulong();

        // Perform the final permutation on the plaintext
        plaintext = finalPermutation(plaintext);

        return plaintext;
    }

private:
    // Function to perform the initial permutation on the input
    std::bitset<64> initialPermutation(std::bitset<64> input) {
        std::bitset<64> permutedInput;
        for (int i = 0; i < 64; i++) {
            int index = initialPermutationTable[i];
            permutedInput[i] = input[index];
        }
        return permutedInput;
    }

    // Function to perform the final permutation on the input
    std::bitset<64> finalPermutation(std::bitset<64> input) {
        std::bitset<64> permutedInput;
        for (int i = 0; i < 64; i++) {
            int index = finalPermutationTable[i];
            permutedInput[i] = input[index];
        }
        return permutedInput;
    }

    // Function to generate the subkey for a given round
    std::bitset<48> generateSubkey(std::bitset<64> key, int round) {
        // Perform the key schedule to generate the subkey
        std::bitset<48> subkey;
        for (int i = 0; i < 48; i++) {
            int index = keyScheduleTable[round][i];
            subkey[i] = key[index];
        }
        return subkey;
    }

    // Function to perform the round function
    std::bitset<32> roundFunction(std::bitset<32> input, std::bitset<48> subkey) {
        // Expand the input to 48 bits
        std::bitset<48> expandedInput = expandInput(input);

        // XOR the expanded input with the subkey
        expandedInput ^= subkey;

        // Perform the S-box substitution
        std::bitset<32> substitutedInput = sBoxSubstitution(expandedInput);

        // Perform the permutation
        std::bitset<32> permutedInput = permutation(substitutedInput);

        return permutedInput;
    }

    // Function to expand the input to 48 bits
    std::bitset<48> expandInput(std::bitset<32> input) {
        std::bitset<48> expandedInput;
        for (int i = 0; i < 48; i++) {
            int index = expansionTable[i];
            expandedInput[i] = input[index];
        }
        return expandedInput;
    }

    // Function to perform the S-box substitution
    std::bitset<32> sBoxSubstitution(std::bitset<48> input) {
        std::bitset<32> substitutedInput;
        for (int i = 0; i < 8; i++) {
            int row = (input[i * 6] << 1) | input[i * 6 + 5];
            int col = (input[i * 6 + 1] << 3) | (input[i * 6 + 2] << 2) | (input[i * 6 + 3] << 1) | input[i * 6 + 4];
            int output = sBoxes[i][row][col];
            substitutedInput[i * 4] = (output >> 3) & 1;
            substitutedInput[i * 4 + 1] = (output >> 2) & 1;
            substitutedInput[i * 4 + 2] = (output >> 1) & 1;
            substitutedInput[i * 4 + 3] = output & 1;
        }
        return substitutedInput;
    }

    // Function to perform the permutation
    std::bitset<32> permutation(std::bitset<32> input) {
        std::bitset<32> permutedInput;
        for (int i = 0; i < 32; i++) {
            int index = permutationTable[i];
            permutedInput[i] = input[index];
        }
        return permutedInput;
    }

    // Initial permutation table
    int initialPermutationTable[64] = {
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7
    };

    // Final permutation table
    int finalPermutationTable[64] = {
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25
    };

    // Key schedule table
    int keyScheduleTable[16][48] = {
        {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32},
        {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32},
        {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32},
        {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32},
        {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32},
        {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32},
        {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32},
        {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32},
        {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32},
        {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32},
        {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32},
        {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32},
        {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32},
        {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32},
        {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32},
        {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32}
    };

    // Expansion table
    int expansionTable[48] = {
        32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17, 16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1
    };

    // S-boxes
    int sBoxes[8][4][16] = {
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

    // Permutation table
    int permutationTable[32] = {
        16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
        2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25
    };
};