#include <gtest/gtest.h>
#include <algorithm>

//This header "test_class.h" contains the model implementation to test. 
//Just create this file, copy the model implementation into it and compile the code
#include "solution.cpp"

#include <stdexcept>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

// -----------------------------------------------------------
// 1. Tests for Basic `get` and `put` behavior
// -----------------------------------------------------------

TEST(LRUCacheTest, GetFromEmptyCacheShouldThrow) {
    LRUCache<std::string, int> cache(2);
    EXPECT_THROW(cache.get("anyKey"), std::runtime_error);
}

TEST(LRUCacheTest, CapacityAndSize) {
    LRUCache<std::string, int> cache(3);
    EXPECT_EQ(cache.capacity(), 3u);
    EXPECT_EQ(cache.size(), 0u);

    cache.put("A", 1);
    cache.put("B", 2);
    cache.put("C", 3);
    EXPECT_EQ(cache.size(), 3u);
}

TEST(LRUCacheTest, BasicPutGet) {
    LRUCache<std::string, int> cache(4);
    cache.put("X", 100);
    cache.put("Y", 200);

    EXPECT_EQ(cache.get("X"), 100);
    EXPECT_EQ(cache.get("Y"), 200);

    // Non-existent key should throw an error
    EXPECT_THROW(cache.get("B"), std::runtime_error);

    // Update "X" to a new value
    cache.put("X", 999);
    EXPECT_EQ(cache.get("X"), 999);

    //verify that the size is still 2
    EXPECT_EQ(cache.size(), 2u);
}

TEST(LRUCacheTest, Eviction) {
    LRUCache<std::string, int> cache(2);
    cache.put("K1", 10);
    cache.put("K2", 20);

    // Access "K1", so "K2" becomes LRU
    EXPECT_EQ(cache.get("K1"), 10);
    
    // Insert "K3" => LRU is "K2" because "K1" was used recently
    cache.put("K3", 30);
    
    // "K2" should have been evicted
    EXPECT_THROW(cache.get("K2"), std::runtime_error);
    // "K1" and "K3" should exist
    EXPECT_EQ(cache.get("K1"), 10);
    EXPECT_EQ(cache.get("K3"), 30);
}

TEST(LRUCacheTest, UsageOrderCheck) {
    LRUCache<std::string, int> cache(3);
    cache.put("A", 1); // most recent: A
    cache.put("B", 2); // order: B -> A
    cache.put("C", 3); // order: C -> B -> A

    // Access B => order: B -> C -> A
    EXPECT_EQ(cache.get("B"), 2);

    auto usageOrder = cache.getUsageOrder();
    std::vector<std::string> keys(usageOrder.begin(), usageOrder.end());
    EXPECT_EQ(keys, (std::vector<std::string>{"B", "C", "A"}));
}

// -----------------------------------------------------------
// 2. Test setExpirationTime and Time-based Expiration
// -----------------------------------------------------------

TEST(LRUCacheTest, ExpirationTime_NoExpirationIfZero) {
    // TTL = 0 => disable time-based eviction
    LRUCache<std::string, int> cache(2);
    cache.setExpirationTime(std::chrono::milliseconds(0));

    cache.put("A", 1);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    // Should not expire
    EXPECT_NO_THROW(EXPECT_EQ(cache.get("A"), 1));
}

TEST(LRUCacheTest, ExpirationTime_BasicExpiry) {
    LRUCache<std::string, int> cache(2);
    // Set 50ms as TTL
    cache.setExpirationTime(std::chrono::milliseconds(50));

    cache.put("X", 10);
    // Sleep for slightly more than 50ms
    std::this_thread::sleep_for(std::chrono::milliseconds(60));

    // "X" should have expired, thus get(...) should throw and size should be 0
    EXPECT_THROW(cache.get("X"), std::runtime_error);
    EXPECT_EQ(cache.size(), 0u);
}

TEST(LRUCacheTest, ExpirationTime_ResetOnAccess) {
    LRUCache<std::string, int> cache(2);
    cache.setExpirationTime(std::chrono::milliseconds(200));

    cache.put("K", 123);
    // Wait 100ms
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // Access "K" => should reset its expiration timer
    EXPECT_NO_THROW(EXPECT_EQ(cache.get("K"), 123));

    // Sleep another 100ms (total 210ms from initial put)
    // But only 100ms from last access
    std::this_thread::sleep_for(std::chrono::milliseconds(110));

    // "K" should still be valid because we accessed it at 100ms
    EXPECT_NO_THROW(EXPECT_EQ(cache.get("K"), 123));
}

TEST(LRUCacheTest, ExpirationTime_ExistingKeysNotAffectedUntilUpdate) {
    // This test checks if the implementation only applies TTL
    // to new or updated entries.
    LRUCache<std::string, int> cache(2);

    // Insert first, no TTL yet
    cache.put("First", 111);

    // Now set TTL
    cache.setExpirationTime(std::chrono::milliseconds(50));
    // Insert second after TTL is set
    cache.put("Second", 222);

    // Sleep 60ms
    std::this_thread::sleep_for(std::chrono::milliseconds(60));

    // "Second" should be expired
    EXPECT_THROW(cache.get("Second"), std::runtime_error);
    // "first" should still be present  
    EXPECT_NO_THROW(EXPECT_EQ(cache.get("First"), 111));
}


// -----------------------------------------------------------
// 3. Test Bulk Insertion: putAll(...)
// -----------------------------------------------------------

TEST(LRUCacheTest, PutAll_Basic) {
    LRUCache<int, int> cache(5);

    // Prepare a small batch of items
    std::vector<std::pair<int,int>> items = {
        {1, 10},
        {2, 20},
        {3, 30},
    };
    cache.putAll(items);

    // All items should be in the cache
    EXPECT_EQ(cache.get(1), 10);
    EXPECT_EQ(cache.get(2), 20);
    EXPECT_EQ(cache.get(3), 30);

    // Size check
    EXPECT_EQ(cache.size(), 3u);

    // Try putting an empty vector
    std::vector<std::pair<int,int>> emptyList;
    cache.putAll(emptyList);

    // Nothing should have changed
    EXPECT_EQ(cache.size(), 3u);
    EXPECT_EQ(cache.get(1), 10);
}


TEST(LRUCacheTest, PutAll_CapacityOverflow) {
    LRUCache<std::string, int> cache(3);

    // Fill cache
    cache.put("K1", 1);
    cache.put("K2", 2);
    cache.put("K3", 3);
    EXPECT_EQ(cache.size(), 3u);

    // This will insert 2 more => must evict some
    std::vector<std::pair<std::string,int>> batch = {
        {"K4", 4},
        {"K5", 5}
    };
    cache.putAll(batch);

    // "K3" should be in the cache (was most recently used),
    // but let's confirm by forcibly "touching" it before putAll:
    EXPECT_NO_THROW(EXPECT_EQ(cache.get("K3"), 3));
    // "K4" and "K5" obviously in the cache
    EXPECT_NO_THROW(EXPECT_EQ(cache.get("K4"), 4));
    EXPECT_NO_THROW(EXPECT_EQ(cache.get("K5"), 5));

    // "K1" or "K2" should be evicted, so let's check both
    EXPECT_THROW(cache.get("K1"), std::runtime_error);
    EXPECT_THROW(cache.get("K2"), std::runtime_error);

    // Final size = 3
    EXPECT_EQ(cache.size(), 3u);
}

TEST(LRUCacheTest, PutAll_WithExpirationTime) {
    LRUCache<std::string, int> cache(3);
    cache.setExpirationTime(std::chrono::milliseconds(50));

    std::vector<std::pair<std::string,int>> data = {
        {"A", 100},
        {"B", 200}
    };
    cache.putAll(data);

    // Immediately accessible
    EXPECT_EQ(cache.get("A"), 100);
    EXPECT_EQ(cache.get("B"), 200);

    // Wait until they expire
    std::this_thread::sleep_for(std::chrono::milliseconds(60));
    // Should now be expired
    EXPECT_THROW(cache.get("A"), std::runtime_error);
    EXPECT_THROW(cache.get("B"), std::runtime_error);
}

// End of tests

