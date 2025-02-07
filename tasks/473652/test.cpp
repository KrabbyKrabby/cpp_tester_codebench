#include "solution.cpp"
#include <gtest/gtest.h>

TEST(ElGamalTest, EncryptionDecryption) {
    ElGamal elgamal;
    string message = "Hello";
    auto encryptedMessage = elgamal.Encrypt(message);
    string decryptedMessage = elgamal.Decrypt(encryptedMessage);
    EXPECT_EQ(message, decryptedMessage);
}

TEST(ElGamalTest, EmptyMessage) {
    ElGamal elgamal;
    string message = "";
    auto encryptedMessage = elgamal.Encrypt(message);
    string decryptedMessage = elgamal.Decrypt(encryptedMessage);
    EXPECT_EQ(message, decryptedMessage);
}

TEST(ElGamalTest, SingleCharacter) {
    ElGamal elgamal;
    string message = "A";
    auto encryptedMessage = elgamal.Encrypt(message);
    string decryptedMessage = elgamal.Decrypt(encryptedMessage);
    EXPECT_EQ(message, decryptedMessage);
}

TEST(ElGamalTest, SpecialCharacters) {
    ElGamal elgamal;
    string message = "!@#$%^&*()";
    auto encryptedMessage = elgamal.Encrypt(message);
    string decryptedMessage = elgamal.Decrypt(encryptedMessage);
    EXPECT_EQ(message, decryptedMessage);
}

TEST(ElGamalTest, LongMessage) {
    ElGamal elgamal;
    string message = "This is a test of a very long message to check the robustness of the encryption and decryption.";
    auto encryptedMessage = elgamal.Encrypt(message);
    string decryptedMessage = elgamal.Decrypt(encryptedMessage);
    EXPECT_EQ(message, decryptedMessage);
}

TEST(ElGamalTest, EdgeCharacters) {
    ElGamal elgamal;
    string message = "\x00\x01\x7F"; // ASCII control characters
    auto encryptedMessage = elgamal.Encrypt(message);
    string decryptedMessage = elgamal.Decrypt(encryptedMessage);
    EXPECT_EQ(message, decryptedMessage);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}