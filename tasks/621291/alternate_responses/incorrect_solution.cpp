#include <iostream>
#include <bitset>

class DES
{
public:
    // Function to perform DES encryption
    std::string encrypt(const std::string& plaintext, const std::string& key) {
        // Convert plaintext and key to binary
        std::string binaryPlaintext = stringToBinary(plaintext);
        std::string binaryKey = stringToBinary(key);

        // Pad plaintext to 64-bit blocks
        while (binaryPlaintext.length() % 64 != 0) {
            binaryPlaintext += "0";
        }

        // Initialize ciphertext
        std::string ciphertext = "";

        // Encrypt each 64-bit block
        for (int i = 0; i < binaryPlaintext.length(); i += 64) {
            std::string block = binaryPlaintext.substr(i, 64);
            std::string encryptedBlock = desEncryptBlock(block, binaryKey);
            ciphertext += encryptedBlock;
        }

        return binaryToString(ciphertext);
    }

    // Function to perform DES decryption
    std::string decrypt(const std::string& ciphertext, const std::string& key) {
        // Convert ciphertext and key to binary
        std::string binaryCiphertext = stringToBinary(ciphertext);
        std::string binaryKey = stringToBinary(key);

        // Pad ciphertext to 64-bit blocks
        while (binaryCiphertext.length() % 64 != 0) {
            binaryCiphertext += "0";
        }

        // Initialize plaintext
        std::string plaintext = "";

        // Decrypt each 64-bit block
        for (int i = 0; i < binaryCiphertext.length(); i += 64) {
            std::string block = binaryCiphertext.substr(i, 64);
            std::string decryptedBlock = desDecryptBlock(block, binaryKey);
            plaintext += decryptedBlock;
        }

        return binaryToString(plaintext);
    }

private:
    // Function to convert string to binary
    std::string stringToBinary(const std::string& str) {
        std::string binary = "";
        for (char c : str) {
            std::bitset<8> bits(c);
            binary += bits.to_string();
        }
        return binary;
    }

    // Function to convert binary to string
    std::string binaryToString(const std::string& binary) {
        std::string str = "";
        for (int i = 0; i < binary.length(); i += 8) {
            std::string byte = binary.substr(i, 8);
            char c = static_cast<char>(std::stoi(byte, 0, 2));
            str += c;
        }
        return str;
    }

    // Function to perform DES encryption on a single 64-bit block
    std::string desEncryptBlock(const std::string& block, const std::string& key) {
        // Perform initial permutation
        std::string permutedBlock = initialPermutation(block);

        // Divide block into left and right halves
        std::string leftHalf = permutedBlock.substr(0, 32);
        std::string rightHalf = permutedBlock.substr(32, 32);

        // Perform 16 rounds of encryption
        for (int i = 0; i < 16; i++) {
            // Calculate subkey for current round
            std::string subkey = calculateSubkey(key, i);

            // Perform round function
            std::string newLeftHalf = rightHalf;
            std::string newRightHalf = leftHalf ^ feistelFunction(rightHalf, subkey);

            // Update left and right halves
            leftHalf = newLeftHalf;
            rightHalf = newRightHalf;
        }

        // Perform final permutation
        std::string encryptedBlock = finalPermutation(rightHalf + leftHalf);

        return encryptedBlock;
    }

    // Function to perform DES decryption on a single 64-bit block
    std::string desDecryptBlock(const std::string& block, const std::string& key) {
        // Perform initial permutation
        std::string permutedBlock = initialPermutation(block);

        // Divide block into left and right halves
        std::string leftHalf = permutedBlock.substr(0, 32);
        std::string rightHalf = permutedBlock.substr(32, 32);

        // Perform 16 rounds of decryption
        for (int i = 15; i >= 0; i--) {
            // Calculate subkey for current round
            std::string subkey = calculateSubkey(key, i);

            // Perform round function
            std::string newLeftHalf = rightHalf;
            std::string newRightHalf = leftHalf ^ feistelFunction(rightHalf, subkey);

            // Update left and right halves
            leftHalf = newLeftHalf;
            rightHalf = newRightHalf;
        }

        // Perform final permutation
        std::string decryptedBlock = finalPermutation(rightHalf + leftHalf);

        return decryptedBlock;
    }

    // Function to perform initial permutation
    std::string initialPermutation(const std::string& block) {
        std::string permutedBlock = "";
        for (int i = 0; i < 64; i++) {
            permutedBlock += block[initialPermutationTable[i]];
        }
        return permutedBlock;
    }

    // Function to perform final permutation
    std::string finalPermutation(const std::string& block) {
        std::string permutedBlock = "";
        for (int i = 0; i < 64; i++) {
            permutedBlock += block[finalPermutationTable[i]];
        }
        return permutedBlock;
    }

    // Function to calculate subkey for a given round
    std::string calculateSubkey(const std::string& key, int round) {
        // Perform key schedule
        std::string subkey = "";
        for (int i = 0; i < 48; i++) {
            subkey += key[keyScheduleTable[round][i]];
        }
        return subkey;
    }

    // Function to perform Feistel function
    std::string feistelFunction(const std::string& rightHalf, const std::string& subkey) {
        // Perform expansion
        std::string expandedRightHalf = "";
        for (int i = 0; i < 48; i++) {
            expandedRightHalf += rightHalf[expansionTable[i]];
        }

        // Perform XOR with subkey
        std::string xoredRightHalf = "";
        for (int i = 0; i < 48; i++) {
            xoredRightHalf += (expandedRightHalf[i] ^ subkey[i]);
        }

        // Perform S-box substitution
        std::string substitutedRightHalf = "";
        for (int i = 0; i < 48; i += 6) {
            std::string input = xoredRightHalf.substr(i, 6);
            int row = std::stoi(input.substr(0, 1), 0, 2);
            int column = std::stoi(input.substr(1, 5), 0, 2);
            substitutedRightHalf += sBox[row][column];
        }

        // Perform permutation
        std::string permutedRightHalf = "";
        for (int i = 0; i < 32; i++) {
            permutedRightHalf += substitutedRightHalf[permutationTable[i]];
        }

        return permutedRightHalf;
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

    // Permutation table
    int permutationTable[32] = {
        16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10, 2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25
    };

    // S-boxes
    std::string sBox[8][4] = {
        {"14", "4", "13", "1", "2", "15", "11", "8", "3", "10", "6", "12", "5", "9", "0", "7"},
        {"0", "15", "7", "4", "14", "2", "13", "1", "10", "6", "12", "11", "9", "5", "3", "8"},
        {"4", "1", "14", "8", "13", "6", "2", "11", "15", "12", "9", "7", "3", "10", "5", "0"},
        {"15", "12", "8", "2", "4", "9", "1", "7", "5", "11", "3", "14", "10", "0", "6", "13"},
        {"15", "1", "8", "14", "6", "11", "3", "4", "9", "7", "2", "13", "12", "0", "5", "10"},
        {"3", "13", "4", "7", "15", "2", "8", "14", "12", "0", "1", "10", "6", "9", "11", "5"},
        {"0", "14", "7", "11", "10", "4", "13", "1", "5", "8", "12", "6", "9", "3", "2", "15"},
        {"13", "8", "10", "1", "3", "15", "4", "2", "11", "6", "7", "12", "0", "5", "14", "9"}
    };
};