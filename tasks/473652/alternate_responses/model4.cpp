#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
using namespace std;

struct EncryptedMessage {
    vector<int> ephemeralKeys;
    vector<int> encryptedText;
};

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
        // Calculate public key
        publicKey = modExp(g, privateKey, p);
    }

    EncryptedMessage Encrypt(const string& plaintext) {
        EncryptedMessage result;
        
        for (char c : plaintext) {
            // Generate random ephemeral key (k)
            int k = rand() % (p - 1) + 1;
            
            // Compute ephemeral public key (g^k mod p)
            int ephemeralKey = modExp(g, k, p);
            
            // Compute shared secret (publicKey^k mod p)
            int sharedSecret = modExp(publicKey, k, p);
            
            // Convert character to number and encrypt
            int message = static_cast<int>(c);
            int encryptedChar = (message * sharedSecret) % p;
            
            // Store both ephemeral key and encrypted character
            result.ephemeralKeys.push_back(ephemeralKey);
            result.encryptedText.push_back(encryptedChar);
        }
        
        return result;
    }

    string Decrypt(const EncryptedMessage& encrypted) {
        string decryptedText;
        
        for (size_t i = 0; i < encrypted.encryptedText.size(); i++) {
            // Compute shared secret for decryption (ephemeralKey^privateKey mod p)
            int decryptionSecret = modExp(encrypted.ephemeralKeys[i], privateKey, p);
            
            // Find modular multiplicative inverse of decryption secret
            int decryptionSecretInverse = findModInverse(decryptionSecret, p);
            
            // Decrypt the character
            int decryptedChar = (encrypted.encryptedText[i] * decryptionSecretInverse) % p;
            
            // Convert number back to character
            decryptedText += static_cast<char>(decryptedChar);
        }
        
        return decryptedText;
    }
};
