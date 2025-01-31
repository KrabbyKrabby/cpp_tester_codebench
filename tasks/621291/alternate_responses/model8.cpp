#include <iostream>
#include <bitset>
#include <vector>

using namespace std;

class DES
{
    private:
        bitset<64> key;
        vector<bitset<48>> subkeys;

        // Function to generate subkeys
        void generateSubkeys()
        {
            // PC1 permutation
            bitset<56> pc1Key;
            for (int i = 0; i < 56; i++)
            {
                int pos = i;
                if (i >= 28)
                    pos = 55 - i;
                pc1Key[i] = key[pos];
            }

            // Divide into two halves
            bitset<28> c0, d0;
            for (int i = 0; i < 28; i++)
            {
                c0[i] = pc1Key[i];
                d0[i] = pc1Key[i + 28];
            }

            // Generate 16 subkeys
            for (int i = 0; i < 16; i++)
            {
                bitset<28> c = c0, d = d0;

                // Left shift
                for (int j = 0; j < i + 1; j++)
                {
                    c = (c << 1) | (c >> 27);
                    d = (d << 1) | (d >> 27);
                }

                // PC2 permutation
                bitset<48> subkey;
                for (int j = 0; j < 48; j++)
                {
                    int pos = j;
                    if (j >= 24)
                        pos = 47 - j;
                    subkey[j] = (j < 24) ? c[pos] : d[pos];
                }

                subkeys.push_back(subkey);
            }
        }

        // S-box substitution
        bitset<32> sBoxSubstitution(bitset<48> input)
        {
            bitset<32> output;

            for (int i = 0; i < 8; i++)
            {
                int row = (input[i * 6] << 1) | input[i * 6 + 5];
                int col = (input[i * 6 + 1] << 3) | (input[i * 6 + 2] << 2) | (input[i * 6 + 3] << 1) | input[i * 6 + 4];

                int sBoxOutput = 0;
                switch (i)
                {
                    case 0:
                        sBoxOutput = (row * 16) + col;
                        break;
                    case 1:
                        sBoxOutput = ((row + 1) % 4 * 16) + col;
                        break;
                    case 2:
                        sBoxOutput = ((row + 2) % 4 * 16) + col;
                        break;
                    case 3:
                        sBoxOutput = ((row + 3) % 4 * 16) + col;
                        break;
                    case 4:
                        sBoxOutput = (row * 16) + ((col + 1) % 16);
                        break;
                    case 5:
                        sBoxOutput = (row * 16) + ((col + 2) % 16);
                        break;
                    case 6:
                        sBoxOutput = (row * 16) + ((col + 3) % 16);
                        break;
                    case 7:
                        sBoxOutput = (row * 16) + ((col + 4) % 16);
                        break;
                }

                output[i * 4] = (sBoxOutput >> 3) & 1;
                output[i * 4 + 1] = (sBoxOutput >> 2) & 1;
                output[i * 4 + 2] = (sBoxOutput >> 1) & 1;
                output[i * 4 + 3] = sBoxOutput & 1;
            }

            return output;
        }

    public:
        // Constructor to initialize the key
        DES(bitset<64> key)
        {
            this->key = key;
            generateSubkeys();
        }

        // Function to encrypt the plaintext
        bitset<64> encrypt(bitset<64> plaintext)
        {
            // IP permutation
            bitset<64> ipPlaintext;
            for (int i = 0; i < 64; i++)
            {
                int pos = i;
                if (i >= 32)
                    pos = 63 - i;
                ipPlaintext[i] = plaintext[pos];
            }

            // Divide into two halves
            bitset<32> l0, r0;
            for (int i = 0; i < 32; i++)
            {
                l0[i] = ipPlaintext[i];
                r0[i] = ipPlaintext[i + 32];
            }

            // 16 rounds of encryption
            for (int i = 0; i < 16; i++)
            {
                // Expansion permutation
                bitset<48> expandedR;
                for (int j = 0; j < 48; j++)
                {
                    int pos = j;
                    if (j >= 24)
                        pos = 47 - j;
                    expandedR[j] = r0[pos % 32];
                }

                // XOR with subkey
                expandedR ^= subkeys[i];

                // S-box substitution
                bitset<32> sBoxOutput = sBoxSubstitution(expandedR);

                // Permutation
                bitset<32> permutedSBoxOutput;
                for (int j = 0; j < 32; j++)
                {
                    int pos = j;
                    if (j >= 16)
                        pos = 31 - j;
                    permutedSBoxOutput[j] = sBoxOutput[pos];
                }

                // XOR with left half
                bitset<32> newL = l0 ^ permutedSBoxOutput;
                l0 = r0;
                r0 = newL;
            }

            // Combine the two halves
            bitset<64> ciphertext;
            for (int i = 0; i < 32; i++)
            {
                ciphertext[i] = r0[i];
                ciphertext[i + 32] = l0[i];
            }

            // IP-1 permutation
            bitset<64> ip1Ciphertext;
            for (int i = 0; i < 64; i++)
            {
                int pos = i;
                if (i >= 32)
                    pos = 63 - i;
                ip1Ciphertext[i] = ciphertext[pos];
            }

            return ip1Ciphertext;
        }

        // Function to decrypt the ciphertext
        bitset<64> decrypt(bitset<64> ciphertext)
        {
            // IP permutation
            bitset<64> ipCiphertext;
            for (int i = 0; i < 64; i++)
            {
                int pos = i;
                if (i >= 32)
                    pos = 63 - i;
                ipCiphertext[i] = ciphertext[pos];
            }

            // Divide into two halves
            bitset<32> l0, r0;
            for (int i = 0; i < 32; i++)
            {
                l0[i] = ipCiphertext[i];
                r0[i] = ipCiphertext[i + 32];
            }

            // 16 rounds of decryption
            for (int i = 15; i >= 0; i--)
            {
                // Expansion permutation
                bitset<48> expandedR;
                for (int j = 0; j < 48; j++)
                {
                    int pos = j;
                    if (j >= 24)
                        pos = 47 - j;
                    expandedR[j] = r0[pos % 32];
                }

                // XOR with subkey
                expandedR ^= subkeys[i];

                // S-box substitution
                bitset<32> sBoxOutput = sBoxSubstitution(expandedR);

                // Permutation
                bitset<32> permutedSBoxOutput;
                for (int j = 0; j < 32; j++)
                {
                    int pos = j;
                    if (j >= 16)
                        pos = 31 - j;
                    permutedSBoxOutput[j] = sBoxOutput[pos];
                }

                // XOR with left half
                bitset<32> newL = l0 ^ permutedSBoxOutput;
                l0 = r0;
                r0 = newL;
            }

            // Combine the two halves
            bitset<64> plaintext;
            for (int i = 0; i < 32; i++)
            {
                plaintext[i] = r0[i];
                plaintext[i + 32] = l0[i];
            }

            // IP-1 permutation
            bitset<64> ip1Plaintext;
            for (int i = 0; i < 64; i++)
            {
                int pos = i;
                if (i >= 32)
                    pos = 63 - i;
                ip1Plaintext[i] = plaintext[pos];
            }

            return ip1Plaintext;
        }
};
