#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <thread>
#include <mutex>
#include <stdexcept>

using namespace std;

// Define complex number type
using Complex = complex<double>;
const double PI = acos(-1);

// Helper function to check if number is power of 2
bool isPowerOfTwo(size_t n) {
    return n && (!(n & (n - 1)));
}

// Improved FFT implementation
void fft(vector<Complex>& a, bool invert) {
    // Handle empty input
    if (a.empty()) {
        return;
    }

    size_t n = a.size();

    // Validate input size
    if (!isPowerOfTwo(n)) {
        throw invalid_argument("Input size must be a power of 2");
    }

    if (n == 1) return;

    // Split into even and odd indices
    vector<Complex> a0(n / 2), a1(n / 2);
    for (size_t i = 0; i < n / 2; i++) {
        a0[i] = a[2 * i];
        a1[i] = a[2 * i + 1];
    }

    // Recursive calls
    fft(a0, invert);
    fft(a1, invert);

    // Improved angle calculation for better precision
    double angle = 2 * PI / n * (invert ? -1 : 1);
    Complex w(1), wn(cos(angle), sin(angle));

    // Combine results
    for (size_t i = 0; i < n / 2; i++) {
        Complex u = a0[i];
        Complex v = w * a1[i];
        
        a[i] = u + v;
        a[i + n/2] = u - v;
        
        w *= wn;
    }

    // Normalize if this is the inverse FFT
    if (invert && n > 1) {
        for (size_t i = 0; i < n; i++) {
            a[i] /= 2;
        }
    }
}

// Parallel FFT for large inputs
void parallelFFT(vector<Complex>& a, bool invert, int threadLimit = 4) {
    if (a.size() <= 1024 || threadLimit <= 1) {
        fft(a, invert);
        return;
    }

    size_t n = a.size();
    vector<Complex> a0(n / 2), a1(n / 2);
    
    for (size_t i = 0; i < n / 2; i++) {
        a0[i] = a[2 * i];
        a1[i] = a[2 * i + 1];
    }

    // Create threads for parallel processing
    thread t1([&]() { parallelFFT(a0, invert, threadLimit / 2); });
    thread t2([&]() { parallelFFT(a1, invert, threadLimit / 2); });

    t1.join();
    t2.join();

    double angle = 2 * PI / n * (invert ? -1 : 1);
    Complex w(1), wn(cos(angle), sin(angle));

    for (size_t i = 0; i < n / 2; i++) {
        Complex u = a0[i];
        Complex v = w * a1[i];
        
        a[i] = u + v;
        a[i + n/2] = u - v;
        
        w *= wn;
    }

    if (invert && n > 1) {
        for (size_t i = 0; i < n; i++) {
            a[i] /= 2;
        }
    }
}

// Function to generate signal
vector<Complex> generateSignal(int n) {
    if (n <= 0) {
        return vector<Complex>();
    }

    if (!isPowerOfTwo(n)) {
        throw invalid_argument("Signal size must be a power of 2");
    }

    vector<Complex> signal(n);
    for (int i = 0; i < n; i++) {
        signal[i] = Complex(sin(2 * PI * i / n), 0);
    }
    return signal;
}
