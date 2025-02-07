#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <sstream>

struct EncryptedMessage {
    std::vector<int> ephemeralKeys;
    std::vector<int> encryptedText;
};

class ElGamal {
private:
    int p;  // Prime number
    int g;  // Primitive root
    int privateKey;
    int publicKey;

    // Function to find modular exponentiation
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

    // Function to find modular multiplicative inverse
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
        
        // Initialize with example values (in practice, use larger primes)
        p = 23;  // Prime number
        g = 5;   // Primitive root
        
        // Generate private key
        privateKey = rand() % (p - 1) + 1;
        
        // Calculate public key
        publicKey = modExp(g, privateKey, p);
    }

    EncryptedMessage Encrypt(const std::string& plaintext) {
        EncryptedMessage result;
        
        for (char c : plaintext) {
            // Generate random ephemeral key (k)
            int k = rand() % (p - 1) + 1;
            
            // Compute ephemeral public key (g^k mod p)
            int ephemeralKey = modExp(g, k, p);
            
            // Compute shared secret (publicKey^k mod p)
            int sharedSecret = modExp(publicKey, k, p);
            
            // Encrypt the character
            int message = static_cast<int>(c);
            int encryptedChar = (message * sharedSecret) % p;
            
            result.ephemeralKeys.push_back(ephemeralKey);
            result.encryptedText.push_back(encryptedChar);
        }
        
        return result;
    }

    std::string Decrypt(const EncryptedMessage& encrypted) {
        std::string decryptedText;
        
        for (size_t i = 0; i < encrypted.encryptedText.size(); i++) {
            // Compute shared secret for decryption (ephemeralKey^privateKey mod p)
            int decryptionSecret = modExp(encrypted.ephemeralKeys[i], privateKey, p);
            
            // Find modular multiplicative inverse of decryptionSecret
            int decryptionSecretInverse = findModInverse(decryptionSecret, p);
            
            // Decrypt the character
            int decryptedChar = (encrypted.encryptedText[i] * decryptionSecretInverse) % p;
            
            decryptedText += static_cast<char>(decryptedChar);
        }
        
        return decryptedText;
    }
};
