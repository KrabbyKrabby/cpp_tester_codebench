#include <gtest/gtest.h>
#include "solution.cpp"


#include <gtest/gtest.h>
#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

// A small helper to sleep for given milliseconds
static void sleepMs(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// -----------------------------------------------------------------------------
// 1) Test: Single Task + runNext()
// -----------------------------------------------------------------------------
TEST(TaskSchedulerTest, SingleTaskRunNext) {
    TaskScheduler scheduler;

    std::atomic<bool> ran{false};
    auto task = [&ran]() {
        ran = true;
    };

    size_t taskId = scheduler.addTask(task);
    EXPECT_EQ(scheduler.getTaskStatus(taskId), TaskStatus::PENDING);

    // runNext() should run exactly one task
    scheduler.runNext();
    EXPECT_TRUE(ran);
    EXPECT_EQ(scheduler.getTaskStatus(taskId), TaskStatus::COMPLETED);
}

// -----------------------------------------------------------------------------
// 2) Test: Multiple Tasks + runNext() in FIFO order (when scheduled for 'now')
// -----------------------------------------------------------------------------
TEST(TaskSchedulerTest, MultipleTasksRunNextFIFO) {
    TaskScheduler scheduler;

    std::vector<int> executionOrder;
    auto taskA = [&]() { executionOrder.push_back(1); };
    auto taskB = [&]() { executionOrder.push_back(2); };
    auto taskC = [&]() { executionOrder.push_back(3); };

    size_t idA = scheduler.addTask(taskA);
    size_t idB = scheduler.addTask(taskB);
    size_t idC = scheduler.addTask(taskC);

    // Run each task, single-threaded
    scheduler.runNext();  // Should run taskA first
    scheduler.runNext();  // Then taskB
    scheduler.runNext();  // Then taskC

    ASSERT_EQ(executionOrder.size(), static_cast<size_t>(3));
    EXPECT_EQ(executionOrder[0], 1);
    EXPECT_EQ(executionOrder[1], 2);
    EXPECT_EQ(executionOrder[2], 3);

    // All should be completed
    EXPECT_EQ(scheduler.getTaskStatus(idA), TaskStatus::COMPLETED);
    EXPECT_EQ(scheduler.getTaskStatus(idB), TaskStatus::COMPLETED);
    EXPECT_EQ(scheduler.getTaskStatus(idC), TaskStatus::COMPLETED);
}

// -----------------------------------------------------------------------------
// 3) Test: runAllConcurrently() with multiple tasks
// -----------------------------------------------------------------------------
TEST(TaskSchedulerTest, RunAllConcurrently) {
    TaskScheduler scheduler;

    constexpr int taskCount = 5;
    std::atomic<int> counter{0};

    // Add tasks that each increment the counter after a short delay
    for (int i = 0; i < taskCount; ++i) {
        scheduler.addTask([&counter]() {
            sleepMs(100); // simulate some work
            counter.fetch_add(1, std::memory_order_relaxed);
        });
    }

    // Start concurrent execution (non-blocking)
    scheduler.runAllConcurrently();
    // Wait for all tasks to finish
    scheduler.waitAll();

    EXPECT_EQ(counter.load(), taskCount);

    // Verify all tasks are completed
    // (Here, tasks start from ID = 0 to ID = taskCount-1, in order)
    for (int i = 0; i < taskCount; ++i) {
        EXPECT_EQ(scheduler.getTaskStatus(i), TaskStatus::COMPLETED);
    }
}

// -----------------------------------------------------------------------------
// 4) Test: Scheduling tasks in the future (not immediately ready)
// -----------------------------------------------------------------------------
TEST(TaskSchedulerTest, ScheduleTaskForFuture) {
    TaskScheduler scheduler;

    std::atomic<bool> ran{false};
    auto task = [&ran]() {
        ran = true;
    };

    using Clock = std::chrono::system_clock;
    auto futureTime = Clock::now() + std::chrono::milliseconds(200);


    size_t futureTaskId = scheduler.scheduleTask(futureTime, task);
    // runNext() immediately: should do nothing because the scheduled time hasn't arrived
    scheduler.runNext();
    EXPECT_FALSE(ran) << "Task should not run yet (time not reached).";
    EXPECT_EQ(scheduler.getTaskStatus(futureTaskId), TaskStatus::PENDING);

    // Wait for time to pass
    auto remainingTime = std::chrono::duration_cast<std::chrono::milliseconds>(futureTime - Clock::now()).count();
    if (remainingTime > 0) {
        sleepMs(remainingTime + 50); // Add buffer
    }

    // Now runNext() again; time is past, so it should run
    scheduler.runNext();
    EXPECT_TRUE(ran);
    EXPECT_EQ(scheduler.getTaskStatus(futureTaskId), TaskStatus::COMPLETED);
}

// -----------------------------------------------------------------------------
// 5) Test: Cancel a PENDING task
// -----------------------------------------------------------------------------
// TEST(TaskSchedulerTest, CancelPendingTask) {
//     TaskScheduler scheduler;

//     std::atomic<bool> ranA{false};
//     std::atomic<bool> ranB{false};

//     size_t idA = scheduler.addTask([&ranA]() { ranA = true; });
//     size_t idB = scheduler.addTask([&ranB]() { ranB = true; });

//     // Cancel task A before running anything
//     bool canceled = scheduler.cancelTask(idA);
//     EXPECT_TRUE(canceled);
//     EXPECT_EQ(scheduler.getTaskStatus(idA), TaskStatus::CANCELED);

//     // Now runNext() for the other task
//     scheduler.runNext();
//     EXPECT_TRUE(ranB);
//     EXPECT_EQ(scheduler.getTaskStatus(idB), TaskStatus::COMPLETED);

//     // Confirm that A never ran
//     EXPECT_FALSE(ranA);
//     EXPECT_EQ(scheduler.getTaskStatus(idA), TaskStatus::CANCELED);
// }
TEST(TaskSchedulerTest, CancelPendingTask) {
    TaskScheduler scheduler;

    std::atomic<bool> ranA{false};
    std::atomic<bool> ranB{false};

    size_t idA = scheduler.addTask([&ranA]() { ranA = true; });
    size_t idB = scheduler.addTask([&ranB]() { ranB = true; });

    // Cancel task A before running anything
    bool canceled = scheduler.cancelTask(idA);
    EXPECT_TRUE(canceled);
    EXPECT_EQ(scheduler.getTaskStatus(idA), TaskStatus::CANCELED);

    // Now runNext() for the other task
    scheduler.runNext();
    EXPECT_TRUE(ranB);  // Task B should have run
    EXPECT_EQ(scheduler.getTaskStatus(idB), TaskStatus::COMPLETED);

    // Confirm that A never ran
    EXPECT_FALSE(ranA);  // Task A should not have run
    EXPECT_EQ(scheduler.getTaskStatus(idA), TaskStatus::CANCELED);
}


// -----------------------------------------------------------------------------
// 6) Test: Cancel fails if task is already RUNNING or COMPLETED
// -----------------------------------------------------------------------------
TEST(TaskSchedulerTest, CancelFailsIfAlreadyRunning) {
    TaskScheduler scheduler;

    // We'll schedule a task that deliberately sleeps so we can attempt cancellation
    std::atomic<bool> ran{false};
    auto slowTask = [&ran]() {
        ran = true;
        sleepMs(200);
    };

    size_t taskId = scheduler.addTask(slowTask);

    // Start concurrent execution
    scheduler.runAllConcurrently();
    // Let the task start running
    sleepMs(50);

    // Attempt cancellation (should fail because it's RUNNING)
    bool canceled = scheduler.cancelTask(taskId);
    EXPECT_FALSE(canceled);

    // Wait for it to complete
    scheduler.waitAll();
    EXPECT_TRUE(ran);
    EXPECT_EQ(scheduler.getTaskStatus(taskId), TaskStatus::COMPLETED);

    // Another cancel attempt also fails (already COMPLETED)
    canceled = scheduler.cancelTask(taskId);
    EXPECT_FALSE(canceled);
}

// -----------------------------------------------------------------------------
// 7) Test: getTaskStatus throws on invalid Task ID
// -----------------------------------------------------------------------------
TEST(TaskSchedulerTest, GetTaskStatusInvalidId) {
    TaskScheduler scheduler;

    // Not adding any tasks, so valid IDs are none
    // A large ID should throw
    EXPECT_THROW(scheduler.getTaskStatus(9999), std::out_of_range);

    // Add a task and check status
    size_t validId = scheduler.addTask([]() {});
    EXPECT_NO_THROW(scheduler.getTaskStatus(validId));
}

