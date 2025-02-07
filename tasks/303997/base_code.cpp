#include <iostream>
#include <string>

// Function to add two integers
int add(int a, int b) {
    return a + b;
}

// Function to add two floats
float add(float a, float b) {
    return a + b;
}

// Function to add two doubles
double add(double a, double b) {
    return a + b;
}

// Function to concatenate two characters
std::string add(char a, char b) {
    std::string result(1, a);
    result += b;
    return result;
}

// Function to concatenate two strings
std::string add(const std::string& a, const std::string& b) {
    return a + b;
}