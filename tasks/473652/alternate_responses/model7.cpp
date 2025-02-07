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
    int p; // Example prime (in real applications, use much larger primes)
    int g; // Primitive root of 23
    int privateKey; // Private key selection (a random integer < p)
    int publicKey; // Public key (g^privateKey mod p)

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
        for (char c : plaintext) {
            // Convert character to numerical value (ASCII)
            int message = static_cast<int>(c);

            // Generate random ephemeral key (k)
            int k = rand() % (p - 1) + 1; // Ensure k > 0

            // Compute ephemeral public key (g^k mod p)
            int ephemeralKey = modExp(g, k, p);

            // Compute the shared secret (publicKey^k mod p)
            int sharedSecret = modExp(publicKey, k, p);

            // Encrypt the message
            int encryptedChar = (message * sharedSecret) % p;

            // Append the encrypted character and ephemeral key to the encrypted message
            encryptedMessage += to_string(encryptedChar) + "," + to_string(ephemeralKey) + ",";
        }
        return encryptedMessage;
    }

    string Decrypt(const string& encryptedMessage) {
        string decryptedMessage;
        size_t pos = 0;
        while ((pos = encryptedMessage.find(',')) != string::npos) {
            int encryptedChar = stoi(encryptedMessage.substr(0, pos));
            size_t nextPos = encryptedMessage.find(',', pos + 1);
            int ephemeralKey = stoi(encryptedMessage.substr(pos + 1, nextPos - pos - 1));

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

            // Decrypt the message
            int decryptedChar = (encryptedChar * decryptionSecretInverse) % p;

            // Convert decrypted numerical value back to character (ASCII)
            decryptedMessage += static_cast<char>(decryptedChar);

            // Move to the next encrypted character and ephemeral key
            pos = nextPos;
        }
        return decryptedMessage;
    }
};
