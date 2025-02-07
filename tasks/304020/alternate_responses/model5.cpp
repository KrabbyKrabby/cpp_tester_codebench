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

// Helper function to get next power of 2
size_t nextPowerOfTwo(size_t n) {
    size_t power = 1;
    while (power < n) {
        power *= 2;
    }
    return power;
}

// Improved FFT implementation
void fft(vector<Complex>& a, bool invert) {
    // Handle empty input
    if (a.empty()) {
        return;
    }

    size_t n = a.size();

    // Ensure input size is power of 2
    if (!isPowerOfTwo(n)) {
        size_t newSize = nextPowerOfTwo(n);
        a.resize(newSize, Complex(0, 0));
        n = newSize;
    }

    // Base case
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

    // Compute twiddle factors with improved precision
    double angle = 2 * PI / n * (invert ? -1 : 1);
    Complex w(1), wn(cos(angle), sin(angle));

    // Combine results
    for (size_t i = 0; i < n / 2; i++) {
        Complex u = a0[i];
        Complex v = w * a1[i];
        
        a[i] = u + v;
        a[i + n / 2] = u - v;
        
        w *= wn;
    }

    // Normalize for inverse FFT
    if (invert) {
        for (size_t i = 0; i < n; i++) {
            a[i] /= 2;
        }
    }
}

// Improved signal generation function
vector<Complex> generateSignal(int n) {
    if (n <= 0) {
        return vector<Complex>();
    }

    vector<Complex> signal(n);
    for (int i = 0; i < n; i++) {
        // Generate a more stable sine wave
        double t = 2.0 * PI * i / static_cast<double>(n);
        signal[i] = Complex(sin(t), 0);
    }
    return signal;
}

// Helper function for inverse FFT
void ifft(vector<Complex>& a) {
    fft(a, true);
    // Additional normalization for inverse FFT
    size_t n = a.size();
    for (size_t i = 0; i < n; i++) {
        a[i] /= n;  // Normalize by n for inverse transform
    }
}
