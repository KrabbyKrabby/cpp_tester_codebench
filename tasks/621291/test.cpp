#include "solution.cpp"
#include <gtest/gtest.h>
using namespace std;

TEST(DESTest, StandardTest) {
    DES des;
    bitset<64> plaintext(0x0123456789ABCDEF);
    bitset<64> key(0x133457799BBCDFF1);
    bitset<64> expectedCiphertext(0x772C402DBF2BA134);

    auto ciphertext = des.encrypt(plaintext, key);
    EXPECT_EQ(ciphertext, expectedCiphertext);

    auto decryptedText = des.decrypt(ciphertext, key);
    EXPECT_EQ(decryptedText, plaintext);
}

TEST(DESTest, AllZerosTest) {
    DES des;
    bitset<64> plaintext(0x0000000000000000);
    bitset<64> key(0x0000000000000000);
    bitset<64> expectedCiphertext(0x3A3BC667DA033C26);

    auto ciphertext = des.encrypt(plaintext, key);
    EXPECT_EQ(ciphertext, expectedCiphertext);

    auto decryptedText = des.decrypt(ciphertext, key);
    EXPECT_EQ(decryptedText, plaintext);
}

TEST(DESTest, AllOnesTest) {
    DES des;
    bitset<64> plaintext(0xFFFFFFFFFFFFFFFF);
    bitset<64> key(0xFFFFFFFFFFFFFFFF);
    bitset<64> expectedCiphertext(0xC5C4399825FCC3D9);

    auto ciphertext = des.encrypt(plaintext, key);
    EXPECT_EQ(ciphertext, expectedCiphertext);

    auto decryptedText = des.decrypt(ciphertext, key);
    EXPECT_EQ(decryptedText, plaintext);
}

TEST(DESTest, PlaintextEqualsKeyTest) {
    DES des;
    bitset<64> plaintext(0x0123456789ABCDEF);
    bitset<64> key(0x0123456789ABCDEF);
    bitset<64> expectedCiphertext(0xD85F31E186883EEE);

    auto ciphertext = des.encrypt(plaintext, key);
    EXPECT_EQ(ciphertext, expectedCiphertext);

    auto decryptedText = des.decrypt(ciphertext, key);
    EXPECT_EQ(decryptedText, plaintext);
}

TEST(DESTest, MinimalKeyTest) {
    DES des;
    bitset<64> plaintext(0xFEDCBA9876543210);
    bitset<64> key(0x0000000000000001);
    bitset<64> expectedCiphertext(0x2514539B2843FE85);

    auto ciphertext = des.encrypt(plaintext, key);
    EXPECT_EQ(ciphertext, expectedCiphertext);

    auto decryptedText = des.decrypt(ciphertext, key);
    EXPECT_EQ(decryptedText, plaintext);
}

TEST(DESTest, RandomTest) {
    DES des;
    bitset<64> plaintext(0x6A6F6770706C6568);
    bitset<64> key(0x7A7A7A7A7A7A7A7A);
    bitset<64> expectedCiphertext(0x31020F8F7496ADBC);

    auto ciphertext = des.encrypt(plaintext, key);
    EXPECT_EQ(ciphertext, expectedCiphertext);

    auto decryptedText = des.decrypt(ciphertext, key);
    EXPECT_EQ(decryptedText, plaintext);
}

TEST(DESTest, EmptyKeyTest) {
    DES des;
    bitset<64> plaintext(0xFFFFFFFFFFFFFFFE);
    bitset<64> key(0x0000000000000000);
    bitset<64> expectedCiphertext(0x61DFAB87DA15E7FA);

    auto ciphertext = des.encrypt(plaintext, key);
    EXPECT_EQ(ciphertext, expectedCiphertext);

    auto decryptedText = des.decrypt(ciphertext, key);
    EXPECT_EQ(decryptedText, plaintext);
}

TEST(DESTest, SymmetryTest) {
    DES des;
    bitset<64> plaintext(0x0123456789ABCDEF);
    bitset<64> key(0x1122334455667788);
    bitset<64> expectedCiphertext(0xEB6A36E6748713B3);

    auto ciphertext = des.encrypt(plaintext, key);
    EXPECT_EQ(ciphertext, expectedCiphertext);

    auto decryptedText = des.decrypt(ciphertext, key);
    EXPECT_EQ(decryptedText, plaintext);
}


int main(int argc, char* argv[]) {

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

    return 0;
}