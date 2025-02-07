#include <gtest/gtest.h>
#include <string>
#include <stdexcept>
#include <iostream>       
#include <vector>   

#include "solution.cpp"  

// User-Defined Type for Testing (Complex Number)
struct ComplexNumber {
    double real;
    double imag;

    // Constructor
    ComplexNumber(double r, double i) : real(r), imag(i) {}

    // Overloaded operator+ for Complex Number Addition
    ComplexNumber operator+(const ComplexNumber& other) const {
        return ComplexNumber(real + other.real, imag + other.imag);
    }

    bool operator==(const ComplexNumber& other) const {
        return (real == other.real) && (imag == other.imag);
    }
};
std::ostream& operator<<(std::ostream& os, const ComplexNumber& c) {
    return os << "(" << c.real << " + " << c.imag << "i)";
}

// 1. Arithmetic Tests

TEST(AddTest, ArithmeticOperations) {
    EXPECT_DOUBLE_EQ(add(3, 3.14), 6.14);
    EXPECT_FLOAT_EQ(add(2.5f, 2), 4.5f);
    EXPECT_EQ(add(10, 20), 30);
    EXPECT_EQ(add(-5, 15), 10);
}


// 2. Char + Char → std::string
TEST(AddTest, CharAndCharReturnsString) {
    EXPECT_EQ(add('a', 'b'), "ab");
    EXPECT_EQ(add('X', 'Y'), "XY");
}


// 3. Char + Int (ASCII Addition)
TEST(AddTest, CharAndIntAsciiAddition) {
    EXPECT_EQ(add('A', 5), 70);   // 'A' (65) + 5 = 70
    EXPECT_EQ(add(10, 'B'), 76);  // 'B' (66) + 10 = 76
}


// 4. Char + String → Concatenation
TEST(AddTest, CharStringConcatenation) {
    EXPECT_EQ(add('A', std::string("BC")), "ABC");
    EXPECT_EQ(add(std::string("Hi"), '!'), "Hi!");
}

// 5. String + String → Concatenation
TEST(AddTest, StringAndStringConcatenation) {
    EXPECT_EQ(add(std::string("Hello"), std::string("World")), "HelloWorld");
}

// 6. Complex Number Addition (User-Defined Type)
TEST(AddTest, UserDefinedComplexNumber) {
    ComplexNumber c1(3.5, 2.5);
    ComplexNumber c2(1.5, 4.0);
    ComplexNumber expected(5.0, 6.5); // (3.5 + 1.5, 2.5 + 4.0)

    EXPECT_EQ(add(c1, c2), expected);
}

// 7. Unsupported Operations (Should Throw)
TEST(AddTest, ThrowsOnUnsupportedOperation) {
    EXPECT_THROW(add(5, std::vector<int>{}), std::invalid_argument);
    EXPECT_THROW(add(std::string("Test"), std::vector<int>{1, 2, 3}), std::invalid_argument);
    EXPECT_THROW(add(5.5, std::vector<double>{1.1, 2.2}), std::invalid_argument);
    EXPECT_THROW(add(std::vector<int>{1, 2}, std::vector<int>{3, 4}), std::invalid_argument);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}