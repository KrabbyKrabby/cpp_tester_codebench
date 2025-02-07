#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <utility>

class ElGamal {
private:
    const int p = 23;  // Prime number (should be much larger in real applications)
    const int g = 5;   // Primitive root
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
    int findModInverse(int a, int m) {
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

    // Encrypt function that takes a text message and returns a vector of pairs
    // Each pair contains (ephemeral key, encrypted character)
    std::vector<std::pair<int, int>> Encrypt(const std::string& plaintext) {
        std::vector<std::pair<int, int>> encryptedMessage;
        
        for (char c : plaintext) {
            // Generate new ephemeral key for each character
            int k = rand() % (p - 1) + 1;
            
            // Compute ephemeral public key
            int ephemeralKey = modExp(g, k, p);
            
            // Compute shared secret
            int sharedSecret = modExp(publicKey, k, p);
            
            // Encrypt the character (converting to ASCII value)
            int encryptedChar = (static_cast<int>(c) * sharedSecret) % p;
            
            // Store the ephemeral key and encrypted character
            encryptedMessage.push_back(std::make_pair(ephemeralKey, encryptedChar));
        }
        
        return encryptedMessage;
    }

    // Decrypt function that takes the encrypted message and returns the original text
    std::string Decrypt(const std::vector<std::pair<int, int>>& encryptedMessage) {
        std::string decryptedText;
        
        for (const auto& pair : encryptedMessage) {
            int ephemeralKey = pair.first;
            int encryptedChar = pair.second;
            
            // Compute shared secret for decryption
            int decryptionSecret = modExp(ephemeralKey, privateKey, p);
            
            // Find modular multiplicative inverse
            int decryptionSecretInverse = findModInverse(decryptionSecret, p);
            
            // Decrypt the character
            int decryptedChar = (encryptedChar * decryptionSecretInverse) % p;
            
            // Convert back to character and append to result
            decryptedText += static_cast<char>(decryptedChar);
        }
        
        return decryptedText;
    }
};
