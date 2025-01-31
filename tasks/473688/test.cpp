#include "gtest/gtest.h"
#include "solution.cpp"  // Replace WRAPPER_FILE with the actual header file name as needed

// Test for basic functionality and edge cases
TEST(CacheBufferTest, BasicOperations) {
    CacheBuffer<int, std::string> buffer(3);
    ASSERT_NO_THROW(buffer.put(1, "one"));
    ASSERT_NO_THROW(buffer.put(2, "two"));
    ASSERT_NO_THROW(buffer.put(3, "three"));

    // Check basic retrieval
    EXPECT_EQ("one", buffer.get(1));
    EXPECT_EQ("two", buffer.get(2));
    EXPECT_EQ("three", buffer.get(3));

    // Test update and frequency increase
    buffer.put(1, "new one");
    EXPECT_EQ("new one", buffer.get(1));
    EXPECT_THROW(buffer.get(4), std::runtime_error);  // Accessing non-existent key
}

// Test to verify the eviction policy based on capacity and frequency
TEST(CacheBufferTest, EvictionPolicy) {
    CacheBuffer<int, std::string> buffer(2);  // Capacity forces eviction
    buffer.put(1, "one");
    buffer.put(2, "two");
    buffer.put(1, "updated one");  // Increase frequency of '1'
    buffer.put(3, "three");  // Should evict '2', not '1'

    EXPECT_EQ("updated one", buffer.get(1));
    EXPECT_THROW(buffer.get(2), std::runtime_error);  // '2' should be evicted
    EXPECT_EQ("three", buffer.get(3));
}

// Test for handling of duplicate keys and frequency updates
TEST(CacheBufferTest, DuplicateKeyHandling) {
    CacheBuffer<int, std::string> buffer(3);
    buffer.put(1, "one");
    buffer.put(1, "new one");  // This should not add a new element but update existing

    EXPECT_EQ(1, buffer.size());  // Ensure the size is still 1
    EXPECT_EQ("new one", buffer.get(1));
}

// Special case: Testing buffer operations under load
TEST(CacheBufferTest, BufferUnderLoad) {
    CacheBuffer<int, int> buffer(100);
    for (int i = 0; i < 200; i++) {  // Double the capacity to force evictions
        buffer.put(i, i);
    }
    // Only the last 100 entries should be present
    EXPECT_THROW(buffer.get(0), std::runtime_error);  // First inserted element should be evicted
    EXPECT_NO_THROW(buffer.get(150));  // Later elements should be present
    EXPECT_NO_THROW(buffer.get(199));  // Last element should definitely be present
}

// Test cases for merging multiple buffers
TEST(CacheBufferTest, MergeBuffers) {
    CacheBuffer<int, std::string> buffer1(5), buffer2(5), buffer3(5);
    buffer1.put(1, "one");
    buffer1.put(2, "two");
    buffer2.put(2, "two");
    buffer2.put(3, "three");
    buffer3.put(4, "four");
    buffer3.put(5, "five");
//cap: 5, 5, 5
//size: 2, 2, 2
    std::vector<CacheBuffer<int, std::string>> buffers = {buffer1, buffer2, buffer3};
    auto merged = CacheBuffer<int, std::string>::mergeBuffers(buffers, 10);
//merged; cap: 10, size: 5.  elements: 1,2,3,4,5 

    // Verifying that all keys are present and frequencies are summed for duplicates
    EXPECT_EQ("one", merged.get(1));
    EXPECT_EQ("two", merged.get(2));
    EXPECT_EQ("three", merged.get(3));
    EXPECT_EQ("four", merged.get(4));
    EXPECT_EQ("five", merged.get(5));
    EXPECT_EQ(5, merged.size());  // Expected size should respect total capacity

    // Test the scenario where merged capacity is less than the sum of elements
    auto smallerMerged = CacheBuffer<int, std::string>::mergeBuffers(buffers, 1);
    EXPECT_EQ(1, smallerMerged.size());  // Check that only two elements remain after eviction

    //After reducing merge the key 2 should be the only remaining element 
    //because it has the highest frequency 
    EXPECT_EQ("two", smallerMerged.get(2));
    EXPECT_THROW(smallerMerged.get(1), std::runtime_error); //getting any other key should throw an error 
}

// Testing backup and restore functionality
TEST(CacheBufferTest, BackupAndRestore) {
    CacheBuffer<int, std::string> buffer(3);
    buffer.put(1, "one");
    buffer.put(2, "two");

    buffer.backup();  // Backup current state

    // Modify buffer after backup
    buffer.put(3, "three");
    buffer.put(1, "updated one");

    // Restore and verify state matches backup
    buffer.restore();
    EXPECT_EQ("one", buffer.get(1));
    EXPECT_EQ("two", buffer.get(2));
    EXPECT_THROW(buffer.get(3), std::runtime_error);  // 'three' was added after backup and should not exist

    // Verify the integrity of backup after multiple restores
    buffer.put(3, "three");
    buffer.restore();  // Restore again to verify backup wasn't altered
    EXPECT_THROW(buffer.get(3), std::runtime_error);
}

// Test to verify the correct exception is thrown if restore is called without a backup
TEST(CacheBufferTest, RestoreWithoutBackup) {
    CacheBuffer<int, std::string> buffer(3);
    EXPECT_THROW(buffer.restore(), std::runtime_error);  // Expect an error since no backup was made
}
