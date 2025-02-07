#include <gtest/gtest.h>

#include WRAPPER_FILE

// Positive test: Basic operations on a stack without using getSize()
TEST(StackTest, PositiveTest) {
    Stack<int> stack(5);
    
    // Push first element
    stack.push(10);
    EXPECT_FALSE(stack.isEmpty()) << "Stack should not be empty after one push.";

    // Push second element
    stack.push(20);
    EXPECT_FALSE(stack.isEmpty()) << "Stack should not be empty after two pushes.";

    // Pop once (removing the top: 20)
    stack.pop();
    // Now top should be 10
    EXPECT_EQ(stack.top(), 10) << "After popping once, top should be 10.";

    // Pop the remaining element
    stack.pop();
    EXPECT_TRUE(stack.isEmpty()) << "Stack should be empty after popping all elements.";
}

// Negative test: Handling of underflow (popping from an empty stack)
TEST(StackTest, NegativeTest) {
    Stack<int> stack(2);
    stack.push(10);
    stack.push(20);

    // Pop both elements
    stack.pop();
    stack.pop();

    // For this test, we expect that popping from an empty stack produces an underflow message.
    // Capture stderr output to check for the underflow message.
    testing::internal::CaptureStderr();
    stack.pop();  // This should trigger underflow behavior.
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_NE(output.find("Stack underflow"), std::string::npos)
        << "Should print underflow message when popping from an empty stack.";
}

// Corner case: Stack with minimum capacity
TEST(StackTest, CornerTest) {
    Stack<int> stack(1);
    // Push a single element
    stack.push(1);
    EXPECT_FALSE(stack.isEmpty()) << "Stack should not be empty after pushing one element.";

    // Pop the only element
    stack.pop();
    EXPECT_TRUE(stack.isEmpty()) << "Stack should be empty after popping its only element.";
}

// Edge case: Stack with maximum capacity (very large number)
TEST(StackTest, EdgeTest) {
    const size_t maxCapacity = 1000000;  // Large capacity for edge test
    Stack<int> stack(maxCapacity);

    // Push up to the maximum capacity
    for (size_t i = 0; i < maxCapacity; ++i) {
        stack.push(static_cast<int>(i));
    }

    // Verify top is maxCapacity-1 (the last pushed element)
    EXPECT_EQ(stack.top(), static_cast<int>(maxCapacity - 1))
        << "Top should be the last pushed element.";

    // Pop one element and check the new top
    stack.pop();
    EXPECT_EQ(stack.top(), static_cast<int>(maxCapacity - 2))
        << "After popping once, top should be the second to last pushed element.";

    // Optionally pop everything to ensure it becomes empty
    for (size_t i = 0; i < (maxCapacity - 1); ++i) {
        stack.pop();
    }
    EXPECT_TRUE(stack.isEmpty())
        << "Stack should be empty after popping all elements.";
}

// Multi-threaded test: Stress test with multiple threads performing push and pop
TEST(StackTest, MultiThreadedTest) {
    Stack<int> stack(10);

    // Define the multi-threaded helper function as a lambda local to this test.
    // Note: The lambda has an empty capture list, so it can be converted to a function pointer.
    auto testStackConcurrency = [](void* arg) -> void* {
        auto* localStack = static_cast<Stack<int>*>(arg);
        for (int i = 0; i < 1000; ++i) {
            if (i % 2 == 0) {
                localStack->push(i);
            } else {
                localStack->pop();
            }
        }
        return nullptr;
    };

    pthread_t t1, t2;
    // The unary '+' forces conversion of the lambda to a function pointer.
    pthread_create(&t1, nullptr, +testStackConcurrency, &stack);
    pthread_create(&t2, nullptr, +testStackConcurrency, &stack);
    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);

    // Basic check: we can still push and verify top without errors.
    stack.push(9999);
    EXPECT_EQ(stack.top(), 9999)
        << "Top should be 9999 after pushing 9999 post-concurrency operations.";
}

// Stress testing with thousands of operations
TEST(StackTest, StressTest) {
    Stack<int> stack(10);
    const int numOperations = 10000;

    // Push a large number of elements
    for (int i = 0; i < numOperations; ++i) {
        stack.push(i);
    }

    // Pop half the elements
    for (int i = 0; i < numOperations / 2; ++i) {
        stack.pop();
    }

    // Count how many are left by popping until empty
    int popCount = 0;
    while (!stack.isEmpty()) {
        stack.pop();
        popCount++;
    }

    // We expect exactly half of the original push operations to remain
    EXPECT_EQ(popCount, numOperations / 2)
        << "After popping half, exactly half of the pushed elements should remain.";
}

// Null pointer test: Handling null pointer dereference
TEST(StackTest, NullPointerTest) {
    Stack<int>* stack = nullptr;

    try {
        if (stack == nullptr) {
            throw std::runtime_error("Null pointer dereference attempted.");
        }
        stack->push(10);  // This line will not execute if the pointer is null
        FAIL() << "Expected exception to be thrown";  // Fail if no exception is thrown
    } catch (const std::exception& e) {
        EXPECT_STREQ(e.what(), "Null pointer dereference attempted.");
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}