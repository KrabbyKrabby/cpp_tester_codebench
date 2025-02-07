#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
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
    int p; // Prime number
    int g; // Primitive root
    int privateKey; // Private key
    int publicKey; // Public key

public:
    // Default constructor
    ElGamal() {
        srand(time(0));
        p = 23; // Example prime (in real applications, use much larger primes)
        g = 5; // Primitive root of 23
        privateKey = rand() % (p - 1) + 1; // Ensure privateKey > 0
        publicKey = modExp(g, privateKey, p);
    }

    // Function to encrypt a message
    string Encrypt(string plaintext) {
        string encryptedMessage = "";
        // Generate random ephemeral key (k)
        int k = rand() % (p - 1) + 1; // Ensure k > 0
        // Compute ephemeral public key (g^k mod p)
        int ephemeralKey = modExp(g, k, p);
        // Compute the shared secret (publicKey^k mod p)
        int sharedSecret = modExp(publicKey, k, p);

        // Encrypt each character in the message
        for (char c : plaintext) {
            int ascii = static_cast<int>(c);
            int encryptedChar = (ascii * sharedSecret) % p;
            encryptedMessage += to_string(encryptedChar) + " ";
        }

        // Append the ephemeral key to the encrypted message
        encryptedMessage += to_string(ephemeralKey);

        return encryptedMessage;
    }

    // Function to decrypt a message
    string Decrypt(string encryptedMessage) {
        // Extract the ephemeral key from the encrypted message
        size_t lastSpace = encryptedMessage.find_last_of(' ');
        string ephemeralKeyStr = encryptedMessage.substr(lastSpace + 1);
        int ephemeralKey = stoi(ephemeralKeyStr);

        // Compute shared secret for decryption (ephemeralKey^privateKey mod p)
        int decryptionSecret = modExp(ephemeralKey, privateKey, p);

        // Modular multiplicative inverse of decryptionSecret
        int decryptionSecretInverse = 0;
        for (int i = 1; i < p; i++) {
            if ((decryptionSecret * i) % p == 1) {
                decryptionSecretInverse = i;
                break;
            }
        }

        // Decrypt each character in the message
        string decryptedMessage = "";
        size_t pos = 0;
        while (pos < lastSpace) {
            size_t nextSpace = encryptedMessage.find(' ', pos);
            string encryptedCharStr = encryptedMessage.substr(pos, nextSpace - pos);
            int encryptedChar = stoi(encryptedCharStr);
            int decryptedChar = (encryptedChar * decryptionSecretInverse) % p;
            decryptedMessage += static_cast<char>(decryptedChar);
            pos = nextSpace + 1;
        }

        return decryptedMessage;
    }
};
