#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <utility>

class ElGamal {
private:
    const int p = 23; // Prime number (should be much larger in real applications)
    const int g = 5;  // Primitive root
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

    // Structure to hold encrypted message components
    struct EncryptedPair {
        int ephemeralKey;
        int encryptedChar;
    };

public:
    ElGamal() {
        srand(time(0));
        // Generate private key
        privateKey = rand() % (p - 1) + 1;
        // Compute public key
        publicKey = modExp(g, privateKey, p);
    }

    std::string Encrypt(const std::string& message) {
        std::vector<EncryptedPair> encryptedChars;
        
        for (char c : message) {
            // Generate new ephemeral key for each character
            int k = rand() % (p - 1) + 1;
            
            // Compute ephemeral public key
            int ephemeralKey = modExp(g, k, p);
            
            // Compute shared secret
            int sharedSecret = modExp(publicKey, k, p);
            
            // Encrypt the character (convert to int and encrypt)
            int numericChar = static_cast<int>(c);
            int encryptedChar = (numericChar * sharedSecret) % p;
            
            encryptedChars.push_back({ephemeralKey, encryptedChar});
        }

        // Convert encrypted pairs to string format
        std::string encryptedMessage;
        for (const auto& pair : encryptedChars) {
            encryptedMessage += std::to_string(pair.ephemeralKey) + "," 
                            + std::to_string(pair.encryptedChar) + ";";
        }
        
        return encryptedMessage;
    }

    std::string Decrypt(const std::string& encryptedMessage) {
        std::string decryptedMessage;
        std::string currentNumber;
        std::vector<EncryptedPair> encryptedPairs;

        // Parse the encrypted message string
        int ephemeralKey = 0;
        bool isFirstNumber = true;
        
        for (size_t i = 0; i < encryptedMessage.length(); i++) {
            if (encryptedMessage[i] == ',') {
                if (isFirstNumber) {
                    ephemeralKey = std::stoi(currentNumber);
                    isFirstNumber = false;
                    currentNumber.clear();
                }
            }
            else if (encryptedMessage[i] == ';') {
                int encryptedChar = std::stoi(currentNumber);
                encryptedPairs.push_back({ephemeralKey, encryptedChar});
                isFirstNumber = true;
                currentNumber.clear();
            }
            else {
                currentNumber += encryptedMessage[i];
            }
        }

        // Decrypt each character
        for (const auto& pair : encryptedPairs) {
            // Compute shared secret for decryption
            int decryptionSecret = modExp(pair.ephemeralKey, privateKey, p);
            
            // Find modular multiplicative inverse
            int decryptionSecretInverse = findModInverse(decryptionSecret, p);
            
            // Decrypt the character
            int decryptedChar = (pair.encryptedChar * decryptionSecretInverse) % p;
            
            // Convert back to char and append to result
            decryptedMessage += static_cast<char>(decryptedChar);
        }

        return decryptedMessage;
    }
};
