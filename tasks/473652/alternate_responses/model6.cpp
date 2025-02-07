#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
#include <vector>
using namespace std;

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

class ElGamal {
private:
    int p;  // Prime number
    int g;  // Primitive root modulo p
    int privateKey; // Private key
    int publicKey; // Public key

public:
    ElGamal() {
        srand(time(0));
        p = 257; // Example prime (in real applications, use much larger primes)
        g = 3;   // Primitive root of p
        privateKey = rand() % (p - 1) + 1; // Ensure privateKey > 0
        publicKey = modExp(g, privateKey, p);
    }

    string Encrypt(const string& plaintext) {
        string encryptedText;
        int message;
        int k = rand() % (p - 1) + 1; // Ensure k > 0
        int ephemeralKey = modExp(g, k, p);
        int sharedSecret = modExp(publicKey, k, p);

        // Encrypt the message character by character
        for (char c : plaintext) {
            message = static_cast<int>(c);
            int encryptedMessage = (message * sharedSecret) % p;
            encryptedText += static_cast<char>(encryptedMessage);
        }

        // Append the ephemeral key to the encrypted text
        encryptedText += static_cast<char>(ephemeralKey);
        return encryptedText;
    }

    string Decrypt(const string& encryptedText) {
        string decryptedText;
        int decryptionSecret;
        int decryptionSecretInverse;
        int encryptedMessage;
        int ephemeralKey = static_cast<int>(encryptedText.back());

        // Compute shared secret for decryption
        decryptionSecret = modExp(ephemeralKey, privateKey, p);

        // Find modular multiplicative inverse of decryptionSecret
        for (int i = 1; i < p; i++) {
            if ((decryptionSecret * i) % p == 1) {
                decryptionSecretInverse = i;
                break;
            }
        }

        // Decrypt the message character by character
        for (size_t i = 0; i < encryptedText.size() - 1; ++i) {
            encryptedMessage = static_cast<int>(encryptedText[i]);
            int decryptedMessage = (encryptedMessage * decryptionSecretInverse) % p;
            decryptedText += static_cast<char>(decryptedMessage);
        }

        return decryptedText;
    }
};
