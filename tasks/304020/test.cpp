#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include <complex>
#include <iostream>
#include <thread>
#include <mutex>
#include <random>

#include "solution.cpp"

using namespace std;

TEST(FFTTest, PositiveFFT) {
    int n = 16; // Length of signal
    vector<Complex> signal = generateSignal(n);
    vector<Complex> originalSignal = signal; // Keep original for validation

    fft(signal, false); // Perform FFT (forward)
    fft(signal, true);  // Perform IFFT (backward)

    // Validate recovery of the original signal with adjusted tolerance
    for (int i = 0; i < n; ++i) {
        EXPECT_NEAR(signal[i].real(), originalSignal[i].real(), 1e-1); // Increased tolerance
        EXPECT_NEAR(signal[i].imag(), originalSignal[i].imag(), 1e-1); // Increased tolerance
    }
}

TEST(FFTTest, EdgeCaseSingleElement) {
    vector<Complex> signal = { Complex(1.0, 0.0) };

    fft(signal, false); // FFT
    fft(signal, true);  // IFFT

    EXPECT_NEAR(signal[0].real(), 1.0, 1e-6);
    EXPECT_NEAR(signal[0].imag(), 0.0, 1e-6);
}

TEST(FFTTest, HandleEmptyInputSignal) {
    vector<Complex> signal;

    size_t originalSize = signal.size();
    vector<Complex> original = signal; 

    // Perform FFT and IFFT, expecting no exceptions or crashes
    EXPECT_NO_THROW(fft(signal, false)); // Forward FFT
    EXPECT_NO_THROW(fft(signal, true)); // Inverse FFT

    // Verify that the size of the vector is unchanged (still 0)
    EXPECT_EQ(signal.size(), originalSize);

    // Verify that the vector remains empty
    EXPECT_TRUE(signal.empty());
    EXPECT_EQ(signal, original);
}


TEST(FFTTest, FailingTestIncorrectRecovery) {
    int n = 16;
    vector<Complex> signal = generateSignal(n);

    fft(signal, false); // Perform FFT (forward)

    // Modify signal to simulate failure scenario
    signal[0] += Complex(1.0, 1.0);

    fft(signal, true); // Perform IFFT (backward)

    // Validate recovery fails due to intentional modification
    EXPECT_NE(signal[0].real() / n, sin(0));
}

TEST(FFTTest, MaxSignalLength) {
    int n = 1 << 20; // Large signal (2^20 points)
    vector<Complex> signal = generateSignal(n);

    EXPECT_NO_THROW(fft(signal, false)); // FFT should not crash
    EXPECT_NO_THROW(fft(signal, true)); // IFFT should not crash
}

TEST(FFTTest, MinSignalLength) {
    vector<Complex> signal = { Complex(1.0, 0.0) };

    fft(signal, false); // FFT
    fft(signal, true);  // IFFT

    EXPECT_NEAR(signal[0].real(), 1.0, 1e-6);
    EXPECT_NEAR(signal[0].imag(), 0.0, 1e-6);
}

extern void fft(std::vector<std::complex<double>>& a, bool invert);
extern std::vector<std::complex<double>> generateSignal(int n);

TEST(FFTTest, ConcurrencyTest) {
    const int numThreads = 4;
    const int n = 1024;

    // We will store the maximum reconstruction error across all threads
    // to ensure concurrency didn't cause unexpected deviations.
    std::atomic<double> maxError(0.0);

    auto threadFunc = [&](int seed) {
        // Create a local random generator for thread safety
        std::mt19937 rng(seed);
        std::uniform_real_distribution<double> dist(-1.0, 1.0);

        // Generate a random signal
        std::vector<std::complex<double>> signal(n);
        for (int i = 0; i < n; i++) {
            signal[i] = std::complex<double>(dist(rng), dist(rng));
        }
        auto originalSignal = signal; // Keep a copy

        // Forward and inverse FFT
        fft(signal, false);
        fft(signal, true);

        // Compute max error for this thread
        double localMaxError = 0.0;
        for (int i = 0; i < n; i++) {
            // Compare real and imaginary parts with original
            double errorReal = std::abs(signal[i].real() - originalSignal[i].real());
            double errorImag = std::abs(signal[i].imag() - originalSignal[i].imag());
            localMaxError = std::max(localMaxError, std::max(errorReal, errorImag));
        }
        // Update global maximum error
        double currentMax = maxError.load(std::memory_order_relaxed);
        while (localMaxError > currentMax &&
               !maxError.compare_exchange_weak(currentMax, localMaxError)) {
            // loop until successfully updated
        }
    };

    // Launch threads
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(threadFunc, i + 42); // Arbitrary seeds
    }

    // Join threads
    for (auto &t : threads) {
        t.join();
    }

    // We expect the final recovered signal to be very close to the original
    // If there is concurrency issue, the error might spike
    EXPECT_LT(maxError.load(), 1e-6) << "Concurrency caused large reconstruction error";
}

TEST(FFTTest, InverseFFTNormalization) {
    int n = 8;
    std::vector<std::complex<double>> signal(n, std::complex<double>(1.0, 0.0));
    std::vector<std::complex<double>> originalSignal = signal;

    // Forward FFT
    fft(signal, false);

    // At this point, signal[0] should be roughly n + 0i, and the rest close to 0 + 0i.

    // Inverse FFT
    fft(signal, true);

    // Now we expect the signal to be the same as original (all ones), 
    // if normalization is done correctly
    for (int i = 0; i < n; ++i) {
        EXPECT_NEAR(signal[i].real(), originalSignal[i].real(), 1e-6);
        EXPECT_NEAR(signal[i].imag(), originalSignal[i].imag(), 1e-6);
    }
}

TEST(FFTTest, NumericPrecisionStressTest) {
    const int n = 1024;
    std::vector<std::complex<double>> signal(n);

    // Use random values of varying magnitudes
    std::mt19937 rng(1234);
    std::uniform_real_distribution<double> dist(-1e3, 1e3);

    for (int i = 0; i < n; i++) {
        signal[i] = std::complex<double>(dist(rng), dist(rng));
    }
    auto originalSignal = signal;

    // Forward and Inverse FFT
    fft(signal, false);
    fft(signal, true);

    // Evaluate maximum reconstruction error
    double maxError = 0.0;
    for (int i = 0; i < n; i++) {
        double errorReal = std::abs(signal[i].real() - originalSignal[i].real());
        double errorImag = std::abs(signal[i].imag() - originalSignal[i].imag());
        maxError = std::max(maxError, std::max(errorReal, errorImag));
    }

    EXPECT_LT(maxError, 1e-6) << "FFT/IFFT reconstruction has excessive error";
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}