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

// Recursive FFT with fixes
void fft(vector<Complex>& a, bool invert) {
    size_t n = a.size();
    
    // Handle empty input or size 1
    if (n <= 1) return;
    
    // Validate input size
    if (!isPowerOfTwo(n)) {
        throw invalid_argument("Input size must be a power of 2");
    }

    // Split into even and odd
    vector<Complex> a0(n / 2), a1(n / 2);
    for (size_t i = 0; i < n / 2; i++) {
        a0[i] = a[2 * i];
        a1[i] = a[2 * i + 1];
    }

    // Recursive calls
    fft(a0, invert);
    fft(a1, invert);

    // Calculate root of unity
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

    // Normalize if inverse transform
    if (invert) {
        for (size_t i = 0; i < n; i++) {
            a[i] /= 2;
        }
    }
}

// Parallel FFT for large inputs
void parallelFFT(vector<Complex>& a, bool invert, int threadLimit = 4) {
    size_t n = a.size();
    
    // Handle empty input or small sizes
    if (n <= 1) return;
    
    // Validate input size
    if (!isPowerOfTwo(n)) {
        throw invalid_argument("Input size must be a power of 2");
    }

    // For small inputs, use regular FFT
    if (n <= 1024 || threadLimit <= 1) {
        fft(a, invert);
        return;
    }

    // Split into even and odd
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

    // Calculate root of unity
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

    // Normalize if inverse transform
    if (invert) {
        for (size_t i = 0; i < n; i++) {
            a[i] /= 2;
        }
    }
}

// Function to generate signal
vector<Complex> generateSignal(int n) {
    if (n <= 0) return vector<Complex>();
    
    vector<Complex> signal(n);
    for (int i = 0; i < n; i++) {
        signal[i] = Complex(sin(2 * PI * i / n), 0); // Pure sine wave
    }
    return signal;
}

// Helper function for forward FFT
void forwardFFT(vector<Complex>& a) {
    parallelFFT(a, false);
}

// Helper function for inverse FFT
void inverseFFT(vector<Complex>& a) {
    parallelFFT(a, true);
    // Additional normalization for inverse FFT
    size_t n = a.size();
    for (size_t i = 0; i < n; i++) {
        a[i] /= n;
    }
}
