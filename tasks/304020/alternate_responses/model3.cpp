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

// Helper function to check if n is a power of 2
bool isPowerOfTwo(size_t n) {
    return n && (!(n & (n - 1)));
}

// Recursive FFT with fixed implementation
void fft(vector<Complex>& a, bool invert) {
    size_t n = a.size();
    
    // Handle edge cases
    if (n == 0) return;
    if (n == 1) return;
    
    // Validate input size
    if (!isPowerOfTwo(n)) {
        throw runtime_error("Input size must be a power of 2");
    }

    // Split into even and odd indices
    vector<Complex> a0(n / 2), a1(n / 2);
    for (size_t i = 0; i < n / 2; i++) {
        a0[i] = a[2 * i];
        a1[i] = a[2 * i + 1];
    }

    // Recursive calls
    fft(a0, invert);
    fft(a1, invert);

    // Compute twiddle factors
    double angle = 2 * PI / n * (invert ? -1 : 1);
    Complex w(1), wn(cos(angle), sin(angle));

    // Combine results
    for (size_t i = 0; i < n / 2; i++) {
        Complex u = a0[i];
        Complex v = w * a1[i];
        
        // No mutex needed here as each thread works on different parts of array
        a[i] = u + v;
        a[i + n/2] = u - v;
        
        w *= wn;
    }

    // Normalize if this is inverse FFT
    if (invert) {
        for (size_t i = 0; i < n; i++) {
            a[i] /= 2;
        }
    }
}

// Parallel FFT implementation for large arrays
void parallelFFT(vector<Complex>& a, bool invert) {
    size_t n = a.size();
    
    // Handle edge cases
    if (n == 0) return;
    if (n <= 1024) {  // For small arrays, use regular FFT
        fft(a, invert);
        return;
    }

    // Split the array into two halves
    size_t half = n / 2;
    vector<Complex> left(a.begin(), a.begin() + half);
    vector<Complex> right(a.begin() + half, a.end());

    // Create threads for parallel processing
    thread t1([&]() { fft(left, invert); });
    thread t2([&]() { fft(right, invert); });

    // Wait for both threads to complete
    t1.join();
    t2.join();

    // Merge results
    double angle = 2 * PI / n * (invert ? -1 : 1);
    Complex w(1), wn(cos(angle), sin(angle));

    for (size_t i = 0; i < n / 2; i++) {
        Complex u = left[i];
        Complex v = w * right[i];
        
        a[i] = u + v;
        a[i + n/2] = u - v;
        
        w *= wn;
    }

    // Normalize if this is inverse FFT
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
        // Added more complex signal for testing
        signal[i] = Complex(sin(2 * PI * i / n), cos(2 * PI * i / n));
    }
    return signal;
}
