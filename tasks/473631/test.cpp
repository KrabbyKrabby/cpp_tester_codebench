#include <gtest/gtest.h>
#include "solution.cpp"

// Test case for the given inputs and expected outputs
TEST(SolutionTest, test1) {
    std::vector<std::pair<int, i64>> input = {
        {7, 10},
        {2, 3},
        {6, 4},
        {1, 6},
        {4, 1}
    };

    std::vector<i64> expected_output = {3, 2, 2, 6, 1};

    // Run the solution function
    std::vector<i64> output = solution(input);

    // Verify the result
    EXPECT_EQ(output, expected_output);
}

// Test case for the given inputs and expected outputs
TEST(SolutionTest, test2) {
    std::vector<std::pair<int, i64>> input;
    std::vector<i64> expected_output;

    for (i64 i = 1; i <= 85; i++) {
        input.push_back({1, i});
        expected_output.push_back(i);
    }

    // Run the solution function
    std::vector<i64> output = solution(input);

    // Verify the result
    EXPECT_EQ(output, expected_output);
}


// Test case for the given inputs and expected outputs
TEST(SolutionTest, test3) {
    std::vector<std::pair<int, i64>> input = {
        {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7}, {1, 8}, {1, 9}, {1, 10},
        {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {2, 7}, {2, 8}, {2, 9}, {2, 10},
        {3, 1}, {3, 2}, {3, 3}, {3, 4}, {3, 5}, {3, 6}, {3, 7}, {3, 8}, {3, 9}, {3, 10},
        {4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {4, 6}, {4, 7}, {4, 8}, {4, 9}, {4, 10},
        {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6}, {5, 7}, {5, 8}, {5, 9}, {5, 10},
        {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5}, {6, 6}, {6, 7}, {6, 8}, {6, 9}, {6, 10},
        {7, 1}, {7, 2}, {7, 3}, {7, 4}, {7, 5}, {7, 6}, {7, 7}, {7, 8}, {7, 9}, {7, 10},
        {8, 1}, {8, 2}, {8, 3}, {8, 4}, {8, 5}, {8, 6}, {8, 7}, {8, 8}, {8, 9}, {8, 10},
        {9, 1}, {9, 2}, {9, 3}, {9, 4}, {9, 5}, {9, 6}, {9, 7}, {9, 8}, {9, 9}, {9, 10},
        {10, 1}, {10, 2}, {10, 3}, {10, 4}, {10, 5}, {10, 6}, {10, 7}
    };

    std::vector<i64> expected_output = {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
        1, 2, 2, 3, 3, 4, 4, 5, 5, 6,
        1, 1, 2, 2, 3, 3, 3, 3, 3, 4,
        1, 2, 2, 3, 3, 3, 3, 4, 4, 4,
        1, 1, 2, 2, 3, 3, 3, 3, 3, 4,
        1, 2, 2, 2, 2, 3, 3, 3, 3, 4,
        1, 1, 1, 1, 1, 1, 2, 2, 3, 3,
        1, 2, 2, 3, 3, 3, 3, 4, 4, 4,
        1, 1, 1, 1, 1, 1, 2, 2, 3, 3,
        1, 2, 3, 3, 4, 5, 5
    };

    // Run the solution function
    std::vector<i64> output = solution(input);

    // Verify the result
    EXPECT_EQ(output, expected_output);
}


// Test case for the given inputs and expected outputs
TEST(SolutionTest, test4) {
    std::vector<std::pair<int, i64>> input = {
        {1, 1023}, {1, 1024}, {1, 1025},
        {2, 1023}, {2, 1024}, {2, 1025},
        {1, 2047}, {1, 2048}, {1, 2049},
        {2, 2047}, {2, 2048}, {2, 2049},
        {1, 4095}, {1, 4096}, {1, 4097},
        {2, 4095}, {2, 4096}, {2, 4097},
        {1, 8191}, {1, 8192}, {1, 8193},
        {2, 8191}, {2, 8192}, {2, 8193},
        {1, 16383}, {1, 16384}, {1, 16385},
        {2, 16383}, {2, 16384}, {2, 16385},
        {1, 32767}, {1, 32768}, {1, 32769},
        {2, 32767}, {2, 32768}, {2, 32769},
        {1, 65535}, {1, 65536}, {1, 65537},
        {2, 65535}, {2, 65536}, {2, 65537},
        {1, 131071}, {1, 131072}, {1, 131073},
        {2, 131071}, {2, 131072}, {2, 131073},
        {1, 262143}, {1, 262144}, {1, 262145},
        {2, 262143}, {2, 262144}, {2, 262145},
        {1, 524287}, {1, 524288}, {1, 524289}
    };

    std::vector<i64> expected_output = {
        1023, 1024, 1025,
        512, 513, 513,
        2047, 2048, 2049,
        1024, 1025, 1025,
        4095, 4096, 4097,
        2048, 2049, 2049,
        8191, 8192, 8193,
        4096, 4097, 4097,
        16383, 16384, 16385,
        8192, 8193, 8193,
        32767, 32768, 32769,
        16384, 16385, 16385,
        65535, 65536, 65537,
        32768, 32769, 32769,
        131071, 131072, 131073,
        65536, 65537, 65537,
        262143, 262144, 262145,
        131072, 131073, 131073,
        524287, 524288, 524289
    };

    // Run the solution function
    std::vector<i64> output = solution(input);

    // Verify the result
    EXPECT_EQ(output, expected_output);
}