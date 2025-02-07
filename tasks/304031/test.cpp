#include <gtest/gtest.h>
#include <thread>
#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>
#include <mutex>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <filesystem>
#include "solution.cpp"
using namespace std;
namespace fs = std::filesystem;

// Test Fixture for RedisDataBase
class RedisDataBaseTest : public ::testing::Test {
protected:
    RedisDataBase db;

    void SetUp() override {
        db.clear();
    }

    void TearDown() override {
        db.clear();
    }
};

// Positive Test Case: Test push and pop operations
TEST_F(RedisDataBaseTest, PushPopTest) {
    db.push(10);
    db.push(20);
    auto res = db.read();
    EXPECT_EQ(res.size(), 2);
    EXPECT_EQ(res[0], 10);
    EXPECT_EQ(res[1], 20);
    db.pop();
    res = db.read();
    EXPECT_EQ(res.size(), 1);
    EXPECT_EQ(res[0], 10);
    db.pop();
    EXPECT_TRUE(db.read().empty()); 
}

// Negative Test Case: Test pop on an empty database
TEST_F(RedisDataBaseTest, PopEmptyDatabaseTest) {
    EXPECT_EQ(-1, db.pop());
}

// Corner Case: Test rollback without any commit
TEST_F(RedisDataBaseTest, RollbackWithoutCommitTest) {
    db.push(10);
    db.push(20);
    EXPECT_EQ(-1, db.rollback());
}

// Edge Case: Test commit and rollback functionality
TEST_F(RedisDataBaseTest, CommitRollbackTest) {
    db.push(10);
    db.commit();
    db.push(20);
    db.commit();
    auto res = db.read(); 
    EXPECT_EQ(res.size(), 2);
    EXPECT_EQ(res[0], 10);
    EXPECT_EQ(res[1], 20);
    EXPECT_EQ(0, db.rollback());

    res = db.read(); 
    EXPECT_EQ(res.size(), 2);
    EXPECT_EQ(res[0], 10);
    EXPECT_EQ(res[1], 20);
    EXPECT_EQ(0, db.rollback());

    res = db.read();
    EXPECT_EQ(res.size(), 1);
    EXPECT_EQ(res[0], 10); 
    EXPECT_EQ(-1, db.rollback());
}

// Edge Case: Test concurrent access
TEST_F(RedisDataBaseTest, ConcurrentAccessTest) {
    auto writer = [this](int i) {
        for(int j = i * 100; j < i*100+100; j++){
            db.push(j); 
        }
    };

    auto reader = [this]() {
        auto res = db.read();
        std::sort(res.begin(), res.end());
        for(int i = 0; i < 100*100; i++){
            EXPECT_EQ(res[i], i);
        }
    };

    std::vector<std::thread> t_write;
    for(int i = 0; i < 100; i++){
        t_write.emplace_back(writer, i);
    }
    for(int i = 0; i < 100; i++){
        t_write[i].join();
    }
    std::thread t2(reader);
    t2.join();
}

// Test loadStateFromDisk functionality
TEST_F(RedisDataBaseTest, LoadStateFromDisk) {
    // Create and save initial state
    {
        RedisDataBase db1;
        db1.push(30);
        db1.push(40);
        db1.commit(); // Save to disk
    }

    // Load state from disk in a new instance
    RedisDataBase db2;
    auto state = db2.read();
    ASSERT_EQ(state.size(), 2);
    EXPECT_EQ(state[0], 30);
    EXPECT_EQ(state[1], 40);
}

// Test clear functionality
TEST_F(RedisDataBaseTest, Clear) {
    db.push(50);
    db.push(60);
    db.commit();
    db.clear();

    EXPECT_TRUE(db.read().empty()); 
}

// Test destructor functionality
TEST_F(RedisDataBaseTest, DestructorSavesState) {
    {
        RedisDataBase db1;
        db1.push(70);
        db1.push(80);
    }

    // Reload state in a new instance
    RedisDataBase db2;
    auto state = db2.read();
    ASSERT_EQ(state.size(), 2);
    EXPECT_EQ(state[0], 70);
    EXPECT_EQ(state[1], 80);
}

// Test for handling empty state during load
TEST_F(RedisDataBaseTest, LoadEmptyState) {
    EXPECT_TRUE(db.read().empty());

}

// Test rollback after commit
TEST_F(RedisDataBaseTest, RollbackAfterCommit) {
    db.push(90);
    db.push(100);
    db.commit();
    db.push(110);
    db.rollback();

    auto state = db.read();
    ASSERT_EQ(state.size(), 2);
    EXPECT_EQ(state[0], 90);
    EXPECT_EQ(state[1], 100);
}