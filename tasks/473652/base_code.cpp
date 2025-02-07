#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
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

int main() {
    srand(time(0));

    // Step 1: Generate a large prime number p (example prime)
    int p = 23; // Example prime (in real applications, use much larger primes)

    // Step 2: Choose a primitive root g modulo p
    int g = 5; // Primitive root of 23

    // Step 3: Private key selection (a random integer < p)
    int privateKey = rand() % (p - 1) + 1; // Ensure privateKey > 0

    // Step 4: Compute public key (g^privateKey mod p)
    int publicKey = modExp(g, privateKey, p);

    cout << "Public Parameters: p = " << p << ", g = " << g << endl;
    cout << "Private Key: " << privateKey << endl;
    cout << "Public Key: " << publicKey << endl;

    // Step 5: Encryption
    cout << "\nEnter a message (as an integer less than p): ";
    int message;
    cin >> message;

    // Generate random ephemeral key (k)
    int k = rand() % (p - 1) + 1; // Ensure k > 0

    // Compute ephemeral public key (g^k mod p)
    int ephemeralKey = modExp(g, k, p);

    // Compute the shared secret (publicKey^k mod p)
    int sharedSecret = modExp(publicKey, k, p);

    // Encrypt the message
    int encryptedMessage = (message * sharedSecret) % p;

    cout << "Ephemeral Key: " << ephemeralKey << endl;
    cout << "Encrypted Message: " << encryptedMessage << endl;

    // Step 6: Decryption
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
    int decryptedMessage = (encryptedMessage * decryptionSecretInverse) % p;

    cout << "Decrypted Message: " << decryptedMessage << endl;

    return 0;
}