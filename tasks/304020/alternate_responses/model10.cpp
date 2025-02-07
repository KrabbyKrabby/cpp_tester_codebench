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

// Recursive FFT with fixes
void fft(vector<Complex>& a, bool invert) {
    size_t n = a.size();
    if (n == 1) return;

    // Handle empty input signal
    if (n == 0) {
        return;
    }

    vector<Complex> a0(n / 2), a1(n / 2);
    for (size_t i = 0; i < n / 2; i++) {
        a0[i] = a[i * 2];      
        a1[i] = a[i * 2 + 1];
    }

    fft(a0, invert);
    fft(a1, invert);

    double angle = 2 * PI / n * (invert ? -1 : 1);
    Complex w(1), wn(cos(angle), sin(angle));

    for (size_t i = 0; i < n / 2; i++) {
        Complex u = a0[i];
        Complex v = w * a1[i];

        // Fix race condition risk for multithreading
        lock_guard<mutex> lock(mtx);

        a[i] = u + v;
        a[i + n / 2] = u - v;
        w *= wn;
    }

    // Normalize for inverse FFT
    if (invert) {
        for (size_t i = 0; i < n; i++) {
            a[i] /= n;
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
