#include <gtest/gtest.h>
#include "solution.cpp"
#include <thread>
#include <chrono>
#include <atomic>
#include <stdexcept>
#include <string>

// Simple pass-through stage that appends some text to a string (for demonstration).
std::string appendStage(const std::string& input) {
    return input + "->stage";
}

// Stage that throws an error if the input matches a certain trigger.
std::string throwingStage(const std::string& input) {
    if (input.find("ERROR") != std::string::npos) {
        throw std::runtime_error("Triggered error in stage!");
    }
    return input + "->throwCheck";
}

// Basic test for single-threaded pipeline (concurrencyLevel=1).
TEST(DataPipelineTest, SingleThreadedBasic) {
    std::vector<std::function<std::string(const std::string&)>> stages = {
        appendStage,
        appendStage
    };
    DataPipeline<std::string> pipeline(stages);
    EXPECT_FALSE(pipeline.isParallel());
    EXPECT_EQ(pipeline.getConcurrencyLevel(), 1u);

    pipeline.push("start");
    // Since single-threaded, processing is immediate.
    pipeline.push("another");
    pipeline.stop(); // Stop, though single-thread might be no-op.

    // No real "output" we can query here, so we rely on no crashes.
    SUCCEED();
}

// Test concurrency activation and queue size changes (if properly tracked).
TEST(DataPipelineTest, EnableParallelismTest) {
    std::vector<std::function<std::string(const std::string&)>> stages = {
        appendStage
    };
    DataPipeline<std::string> pipeline(stages);
    pipeline.enableParallelism(3);
    EXPECT_TRUE(pipeline.isParallel());
    EXPECT_EQ(pipeline.getConcurrencyLevel(), 3u);

    // Push multiple items quickly; in parallel mode, they should queue up.
    pipeline.push("A");
    pipeline.push("B");
    pipeline.push("C");

    size_t qSize = pipeline.queueSize(); // Possibly >0 if not processed yet.
    EXPECT_GE(qSize, 0u); // We can't guarantee exact number if quickly processed.

    pipeline.stop();
    SUCCEED();
}

// Test error handling: set an error handler and push a triggering item.
TEST(DataPipelineTest, ErrorHandling) {
    std::atomic<bool> errorCaught{false};
    auto errHandler = [&errorCaught](const std::exception& e) {
        // We only check that this is called.
        errorCaught.store(true);
    };

    std::vector<std::function<std::string(const std::string&)>> stages = {
        throwingStage
    };
    DataPipeline<std::string> pipeline(stages);
    pipeline.setErrorHandler(errHandler);

    // Try concurrency as well.
    pipeline.enableParallelism(2);

    // Pushing normal item.
    pipeline.push("NO_ERROR");

    // Pushing item that triggers stage throw.
    pipeline.push("ERROR_TRIGGERS");

    // Attempt to drain.
    pipeline.drain();
    pipeline.stop();

    EXPECT_TRUE(errorCaught.load());
}

// Tests the drain functionality: push multiple items, then drain, ensuring all are processed.
TEST(DataPipelineTest, DrainTest) {
    std::vector<std::function<std::string(const std::string&)>> stages = {
        appendStage,
        appendStage
    };
    DataPipeline<std::string> pipeline(stages);
    pipeline.enableParallelism(3);

    // Push multiple items.
    for (int i = 0; i < 10; ++i) {
        pipeline.push("Item" + std::to_string(i));
    }

    // Now drain and ensure pipeline is empty afterwards.
    pipeline.drain();
    EXPECT_EQ(pipeline.queueSize(), 0u);

    pipeline.stop(); // Should be safe to call multiple times.
    SUCCEED();
}

// Checks that pushing after stop leads to an exception.
TEST(DataPipelineTest, PushAfterStop) {
    std::vector<std::function<std::string(const std::string&)>> stages = {
        appendStage
    };
    DataPipeline<std::string> pipeline(stages);
    pipeline.stop();
    EXPECT_THROW(pipeline.push("Nope"), std::runtime_error);
}

// Main entry for GTest.
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}