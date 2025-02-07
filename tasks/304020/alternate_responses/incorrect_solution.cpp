#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <thread>
#include <mutex>

using namespace std;

// Define complex number type
using Complex = complex<double>;
const double PI = acos(-1);

// Mutex for multithreading
mutex mtx;

// Recursive FFT with subtle bugs fixed
void fft(vector<Complex>& a, bool invert) {
    size_t n = a.size();
    if (n == 1) return; // Base case: if size is 1, return immediately

    // Handle edge case: empty input signal
    if (n == 0) {
        return; // Return immediately if the input signal is empty
    }

    vector<Complex> a0(n / 2), a1(n / 2);
    for (size_t i = 0; i < n / 2; i++) {
        a0[i] = a[i * 2];      // Even-indexed elements
        a1[i] = a[i * 2 + 1];  // Odd-indexed elements
    }

    // Recursively compute FFT for a0 and a1
    fft(a0, invert);
    fft(a1, invert);

    double angle = 2 * PI / n * (invert ? -1 : 1);
    Complex w(1), wn(cos(angle), sin(angle));

    for (size_t i = 0; i < n / 2; i++) {
        Complex u = a0[i];
        Complex v = w * a1[i];

        // Use lock_guard to ensure thread safety
        lock_guard<mutex> lock(mtx);

        a[i] = u + v;              // Combine results
        a[i + n / 2] = u - v;      // Combine results
        w *= wn;
    }

    // Normalize the signal for inverse FFT
    if (invert) {
        for (size_t i = 0; i < n; i++) {
            a[i] /= n; // Normalize the signal
        }
    }
}

// Function to generate signal
vector<Complex> generateSignal(int n) {
    vector<Complex> signal(n);
    for (int i = 0; i < n; i++) {
        signal[i] = Complex(sin(2 * PI * i / n), 0); // Pure sine wave
    }
    return signal;
}
