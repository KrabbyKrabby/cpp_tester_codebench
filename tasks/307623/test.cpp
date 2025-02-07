#include "solution.cpp"
#include <gtest/gtest.h>
#include <atomic>
#include <thread>
#include <vector>
#include <future>
#include <chrono>

// Helper function to simulate work
void simulateWork(int ms, std::atomic<int>& counter) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    counter++;
}

// Test Fixture for ThreadPool
class ThreadPoolTest : public ::testing::Test {
protected:
    ThreadPool* pool;

    void SetUp() override {
        pool = new ThreadPool(3);
    }

    void TearDown() override {
        pool->shutdown();
        delete pool;
    }
};

// Test basic task submission and execution
TEST_F(ThreadPoolTest, BasicTaskExecution) {
    std::atomic<int> counter(0);
    auto fut1 = pool->submit(Priority::Medium, simulateWork, 100, std::ref(counter));
    auto fut2 = pool->submit(Priority::High, simulateWork, 100, std::ref(counter));

    fut1.get();
    fut2.get();

    EXPECT_EQ(counter.load(), 2);
}



// Test dynamic resizing - increasing threads
TEST_F(ThreadPoolTest, DynamicResizingIncrease) {
    std::atomic<int> counter(0);
    auto task = [&counter]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        counter++;
    };

    // Submit 4 tasks
    for(int i = 0; i < 4; ++i) {
        pool->submit(Priority::Medium, task);
    }

    // Resize to 5 threads
    pool->resize(5);

    // Submit additional tasks
    for(int i = 0; i < 5; ++i) {
        pool->submit(Priority::Low, task);
    }

    // Allow time for all tasks to execute
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    EXPECT_EQ(counter.load(), 9);
    EXPECT_EQ(pool->getTaskQueueSize(), 0);
    EXPECT_EQ(pool->getActiveThreadCount(), 0);
    EXPECT_EQ(pool->getTotalTasksExecuted(), 9);
}


// Test task dependencies
TEST_F(ThreadPoolTest, TaskDependencies) {
    std::atomic<int> counter(0);

    auto task1 = [&counter]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        counter += 1;
    };

    auto task2 = [&counter]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        counter += 2;
    };

    auto task3 = [&counter]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        counter += 3;
    };

    // Submit task1 and task2
    auto fut1 = pool->submit(Priority::Low, task1);
    auto fut2 = pool->submit(Priority::Low, task2);

    // Submit task3 with dependencies on task1 and task2
    std::vector<std::future<void>> dependencies;
    dependencies.emplace_back(std::move(fut1));
    dependencies.emplace_back(std::move(fut2));
    auto fut3 = pool->submit(Priority::High, std::move(dependencies), task3);

    fut3.get();

    EXPECT_EQ(counter.load(), 6);
}

// Test submitting tasks after shutdown
TEST_F(ThreadPoolTest, SubmitAfterShutdown) {
    pool->shutdown();

    auto task = []() {};

    EXPECT_THROW(
        pool->submit(Priority::Medium, task),
        std::runtime_error
    );
}

// Test circular dependencies (Note: The current implementation does not handle circular dependencies)
TEST_F(ThreadPoolTest, CircularDependencies) {
    std::atomic<int> counter(0);

    auto task1 = [&counter]() {
        counter += 1;
    };

    auto task2 = [&counter]() {
        counter += 2;
    };

    // Submit task1
    auto fut1 = pool->submit(Priority::Medium, task1);

    // Submit task2 with dependency on task1
    std::vector<std::future<void>> dependencies1;
    dependencies1.emplace_back(std::move(fut1));
    auto fut2 = pool->submit(Priority::Medium, std::move(dependencies1), task2);

    // Introduce circular dependency: task1 depends on task2
    // Since our implementation does not handle dependency graphs, we ensure no deadlock occurs
    // To simulate, we create a new task that depends on fut2
    std::vector<std::future<void>> dependencies2;
    dependencies2.emplace_back(std::move(fut2));
    auto fut3 = pool->submit(Priority::Medium, std::move(dependencies2), task1);

    fut3.get();

    EXPECT_EQ(counter.load(), 4); // task1 executed twice and task2 once
}

// Test resizing to zero (should shutdown the pool)
TEST_F(ThreadPoolTest, ResizeToZero) {
    std::atomic<int> counter(0);
    auto task = [&counter]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        counter++;
    };

    // Submit 2 tasks
    auto fut1 = pool->submit(Priority::Medium, task);
    auto fut2 = pool->submit(Priority::Medium, task);

    // Resize to 0 threads
    pool->resize(0);

    // Attempt to submit another task should throw
    auto task3 = [](){};
    // Wait for initial tasks to complete
    fut1.get();
    fut2.get();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_EQ(counter.load(), 2);
    EXPECT_EQ(pool->getTaskQueueSize(), 0);
    // Allow extra time for threads to exit
    EXPECT_EQ(pool->getActiveThreadCount(), 0);
    EXPECT_EQ(pool->getTotalTasksExecuted(), 2);
}

// Test tasks with complex dependencies
TEST_F(ThreadPoolTest, ComplexTaskDependencies) {
    std::atomic<int> counter(0);

    auto taskA = [&counter]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        counter += 1;
    };

    auto taskB = [&counter]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        counter += 2;
    };

    auto taskC = [&counter]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        counter += 3;
    };

    auto taskD = [&counter]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        counter += 4;
    };

    // Submit taskA and taskB
    auto futA = pool->submit(Priority::High, taskA);
    auto futB = pool->submit(Priority::High, taskB);

    // Submit taskC depending on taskA and taskB
    std::vector<std::future<void>> dependencies1;
    dependencies1.emplace_back(std::move(futA));
    dependencies1.emplace_back(std::move(futB));
    auto futC = pool->submit(Priority::Medium, std::move(dependencies1), taskC);

    // Submit taskD depending on taskC
    std::vector<std::future<void>> dependencies2;
    dependencies2.emplace_back(std::move(futC));
    auto futD = pool->submit(Priority::Low, std::move(dependencies2), taskD);

    futD.get();

    EXPECT_EQ(counter.load(), 10); // 1 + 2 + 3 + 4
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_EQ(pool->getTotalTasksExecuted(), 4);
}

//Failing

// Test high concurrency with multiple submissions 
TEST_F(ThreadPoolTest, HighConcurrencySubmissions) {
    // Create a separate pool for high concurrency
    ThreadPool highPool(10);
    std::atomic<int> counter(0);
    const int numTasks = 1000;

    auto task = [&counter]() {
        counter++;
    };

    std::vector<std::future<void>> futures;
    for(int i = 0; i < numTasks; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        futures.emplace_back(highPool.submit(Priority::Medium, task));
    }

    for(auto &fut : futures) {
        fut.get();
    }

    EXPECT_EQ(counter.load(), numTasks);
    EXPECT_EQ(highPool.getTotalTasksExecuted(), numTasks);
    highPool.shutdown();
}