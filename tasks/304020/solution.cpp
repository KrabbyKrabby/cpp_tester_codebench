#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <stdexcept>

using namespace std;

// Define complex number type
using Complex = complex<double>;
const double PI = acos(-1);

// Helper function to calculate the FFT of a signal
void fft(vector<complex<double>>& signal, bool inverse) {
    int n = signal.size();
    if (n <= 1) return;

    // Divide the signal into even and odd parts
    vector<complex<double>> even(n / 2);
    vector<complex<double>> odd(n / 2);
    for (int i = 0; i < n / 2; ++i) {
        even[i] = signal[i * 2];
        odd[i] = signal[i * 2 + 1];
    }

    // Recursively apply FFT to even and odd parts
    fft(even, inverse);
    fft(odd, inverse);

    // Combine the results
    double angle = 2 * PI / n * (inverse ? -1 : 1);
    complex<double> w(1), wn(cos(angle), sin(angle));

    for (int i = 0; i < n / 2; ++i) {
        signal[i] = even[i] + w * odd[i];
        signal[i + n / 2] = even[i] - w * odd[i];

        if (inverse) {
            signal[i] /= 2;
            signal[i + n / 2] /= 2;
        }
        w *= wn;
    }
}

// Function to generate a test signal
vector<Complex> generateSignal(int n) {
    if (n <= 0) throw invalid_argument("Signal size must be positive.");
    if (n & (n - 1)) throw invalid_argument("Signal size must be a power of 2.");

    vector<Complex> signal(n);
    for (int i = 0; i < n; i++) {
        signal[i] = Complex(sin(2 * PI * i / n), 0); // Pure sine wave
    }
    return signal;
}