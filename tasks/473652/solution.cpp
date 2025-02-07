#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <cstdlib>
#include <ctime>
using namespace std;

class ElGamal {
private:
    const int p = 257;  // A prime number (using 251 to support ASCII characters)
    const int g = 5;    // Primitive root modulo p
    int privateKey;
    int publicKey;

    // Helper function for modular exponentiation
    long long modExp(long long base, long long exp, long long mod) {
        long long result = 1;
        base = base % mod;
        while (exp > 0) {
            if (exp % 2 == 1) {
                result = (result * base) % mod;
            }
            exp = exp >> 1;
            base = (base * base) % mod;
        }
        return result;
    }

    // Helper function to find modular multiplicative inverse
    int modInverse(int a, int m) {
        for (int i = 1; i < m; i++) {
            if ((a * i) % m == 1) {
                return i;
            }
        }
        return 1;
    }

public:
    ElGamal() {
        srand(time(0));
        // Generate private key
        privateKey = rand() % (p - 1) + 1;
        // Compute public key
        publicKey = modExp(g, privateKey, p);
    }

    // Encrypt function that takes a plaintext string and returns a pair of vectors
    // First vector contains ephemeral keys, second vector contains encrypted characters
    std::pair<std::vector<int>, std::vector<int>> Encrypt(const std::string& plaintext) {
        std::vector<int> ephemeralKeys;
        std::vector<int> encryptedChars;

        for (char c : plaintext) {
            // Generate random ephemeral key k
            int k = rand() % (p - 1) + 1;
            
            // Compute ephemeral public key (g^k mod p)
            int ephemeralKey = modExp(g, k, p);
            
            // Compute shared secret (publicKey^k mod p)
            int sharedSecret = modExp(publicKey, k, p);
            
            // Encrypt the character
            int encryptedChar = (static_cast<int>(c) * sharedSecret) % p;
            
            ephemeralKeys.push_back(ephemeralKey);
            encryptedChars.push_back(encryptedChar);
        }

        return std::make_pair(ephemeralKeys, encryptedChars);
    }

    // Decrypt function that takes the encrypted message (pair of vectors) and returns the decrypted string
    std::string Decrypt(const std::pair<std::vector<int>, std::vector<int>>& encryptedMsg) {
        std::string decryptedText;
        const std::vector<int>& ephemeralKeys = encryptedMsg.first;
        const std::vector<int>& encryptedChars = encryptedMsg.second;

        for (size_t i = 0; i < ephemeralKeys.size(); i++) {
            // Compute shared secret for decryption (ephemeralKey^privateKey mod p)
            int decryptionSecret = modExp(ephemeralKeys[i], privateKey, p);
            
            // Find modular multiplicative inverse of decryptionSecret
            int decryptionSecretInverse = modInverse(decryptionSecret, p);
            
            // Decrypt the character
            int decryptedChar = (encryptedChars[i] * decryptionSecretInverse) % p;
            
            decryptedText += static_cast<char>(decryptedChar);
        }

        return decryptedText;
    }
};