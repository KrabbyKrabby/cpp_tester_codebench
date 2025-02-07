#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
using namespace std;

class ElGamal {
private:
    int p; // Large prime number
    int g; // Primitive root modulo p
    int privateKey; // Private key
    int publicKey; // Public key

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

public:
    ElGamal() {
        srand(time(0));
        p = 23; // Example prime (in real applications, use much larger primes)
        g = 5; // Primitive root of 23
        privateKey = rand() % (p - 1) + 1; // Ensure privateKey > 0
        publicKey = modExp(g, privateKey, p);
    }

    string Encrypt(const string& plaintext) {
        string encryptedMessage;
        for (char c : plaintext) {
            int message = c; // Convert character to ASCII value

            // Generate random ephemeral key (k)
            int k = rand() % (p - 1) + 1; // Ensure k > 0

            // Compute ephemeral public key (g^k mod p)
            int ephemeralKey = modExp(g, k, p);

            // Compute the shared secret (publicKey^k mod p)
            int sharedSecret = modExp(publicKey, k, p);

            // Encrypt the message
            int encryptedChar = (message * sharedSecret) % p;

            // Append encrypted character to the encrypted message
            encryptedMessage += to_string(encryptedChar) + " ";
        }
        return encryptedMessage;
    }

    string Decrypt(const string& encryptedMessage) {
        string decryptedMessage;
        size_t pos = 0;
        while ((pos = encryptedMessage.find(" ")) != string::npos) {
            int encryptedChar = stoi(encryptedMessage.substr(0, pos));

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

            // Decrypt the message
            int decryptedChar = (encryptedChar * decryptionSecretInverse) % p;

            // Append decrypted character to the decrypted message
            decryptedMessage += char(decryptedChar);

            // Remove the processed character from the encrypted message
            encryptedMessage.erase(0, pos + 1);
        }
        return decryptedMessage;
    }
};
