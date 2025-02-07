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
    int p; // Prime number
    int g; // Primitive root of p
    int privateKey; // Private key
    int publicKey; // Public key

public:
    ElGamal() {
        srand(time(0));

        // Step 1: Generate a large prime number p (example prime)
        p = 23; // Example prime (in real applications, use much larger primes)

        // Step 2: Choose a primitive root g modulo p
        g = 5; // Primitive root of 23

        // Step 3: Private key selection (a random integer < p)
        privateKey = rand() % (p - 1) + 1; // Ensure privateKey > 0

        // Step 4: Compute public key (g^privateKey mod p)
        publicKey = modExp(g, privateKey, p);
    }

    string Encrypt(const string& plaintext) {
        string encryptedMessage;

        // Convert each character of the plaintext to ASCII value
        for (char c : plaintext) {
            int message = static_cast<int>(c);

            // Generate random ephemeral key (k)
            int k = rand() % (p - 1) + 1; // Ensure k > 0

            // Compute ephemeral public key (g^k mod p)
            int ephemeralKey = modExp(g, k, p);

            // Compute the shared secret (publicKey^k mod p)
            int sharedSecret = modExp(publicKey, k, p);

            // Encrypt the message
            int encryptedChar = (message * sharedSecret) % p;

            // Append the encrypted character to the encrypted message
            encryptedMessage += static_cast<char>(encryptedChar);
        }

        return encryptedMessage;
    }

    string Decrypt(const string& encryptedMessage) {
        string decryptedMessage;

        // Convert each character of the encrypted message to ASCII value
        for (char c : encryptedMessage) {
            int encryptedChar = static_cast<int>(c);

            // Compute shared secret for decryption (ephemeralKey^privateKey mod p)
            int decryptionSecret = modExp(encryptedChar, privateKey, p);

            // Modular multiplicative inverse of decryptionSecret
            int decryptionSecretInverse = 0;
            for (int i = 1; i < p; i++) {
                if ((decryptionSecret * i) % p == 1) {
                    decryptionSecretInverse = i;
                    break;
                }
            }

            // Decrypt the character
            int decryptedChar = (encryptedChar * decryptionSecretInverse) % p;

            // Append the decrypted character to the decrypted message
            decryptedMessage += static_cast<char>(decryptedChar);
        }

        return decryptedMessage;
    }
};
