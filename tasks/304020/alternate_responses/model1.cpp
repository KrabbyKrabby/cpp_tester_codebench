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

// Mutex for multithreading synchronization
mutex mtx;

// Improved FFT implementation
void fft(vector<Complex>& a, bool invert) {
    // Handle empty input
    if (a.empty()) return;
    
    size_t n = a.size();
    
    // Check if n is a power of 2
    if (n & (n - 1)) {
        throw runtime_error("Size must be a power of 2");
    }
    
    if (n == 1) return;

    vector<Complex> a0(n / 2), a1(n / 2);
    
    // Separate even and odd elements
    for (size_t i = 0; i < n / 2; i++) {
        a0[i] = a[2 * i];
        a1[i] = a[2 * i + 1];
    }

    // Recursive calls
    fft(a0, invert);
    fft(a1, invert);

    // Calculate twiddle factors
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

    // Normalize if this is inverse FFT
    if (invert) {
        for (size_t i = 0; i < n; i++) {
            a[i] /= 2;
        }
    }
}

// Improved signal generation with thread safety
vector<Complex> generateSignal(int n) {
    if (n <= 0) return vector<Complex>();
    
    vector<Complex> signal(n);
    
    // Use multiple threads for signal generation
    const int num_threads = thread::hardware_concurrency();
    vector<thread> threads;
    
    auto worker = [&](int start, int end) {
        for (int i = start; i < end; i++) {
            double phase = 2 * PI * i / n;
            signal[i] = Complex(sin(phase), 0);
        }
    };
    
    // Create threads
    int chunk_size = n / num_threads;
    for (int i = 0; i < num_threads; i++) {
        int start = i * chunk_size;
        int end = (i == num_threads - 1) ? n : (i + 1) * chunk_size;
        threads.emplace_back(worker, start, end);
    }
    
    // Join threads
    for (auto& thread : threads) {
        thread.join();
    }
    
    return signal;
}

// Helper function for inverse FFT
void ifft(vector<Complex>& a) {
    fft(a, true);
    // Additional normalization for inverse FFT
    size_t n = a.size();
    for (size_t i = 0; i < n; i++) {
        a[i] /= n;
    }
}
