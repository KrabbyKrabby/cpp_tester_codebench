#include <gtest/gtest.h>
#include "solution.cpp"
#include <stdexcept>
#include <string>

// --------------------------------------------------------
// 1) Basic Tests for put / get / size / capacity
// --------------------------------------------------------

TEST(FreqBufferBasicTest, CapacityAndSize) {
    FreqBuffer<std::string, int> buffer(3);
    EXPECT_EQ(buffer.capacity(), 3u);
    EXPECT_EQ(buffer.size(), 0u);

    buffer.put("A", 1);
    buffer.put("B", 2);
    buffer.put("C", 3);
    EXPECT_EQ(buffer.size(), 3u);

    //Check we never exceed capacity
    buffer.put("D", 4);
    EXPECT_LE(buffer.size(), 3u);
    EXPECT_EQ(buffer.capacity(), 3u);
}

TEST(FreqBufferBasicTest, PutAndGet) {
    FreqBuffer<std::string, int> buffer(2);
    buffer.put("X", 100);
    buffer.put("Y", 200);

    EXPECT_EQ(buffer.get("X"), 100);
    EXPECT_EQ(buffer.get("Y"), 200);

    // Overwrite X => get should reflect updated value
    buffer.put("X", 999);
    EXPECT_EQ(buffer.get("X"), 999);

    // Non-existent key => should throw runtime_error
    EXPECT_THROW(buffer.get("Z"), std::runtime_error);
}

TEST(FreqBufferBasicTest, FrequencyIncrementsOnGet) {
    FreqBuffer<std::string, int> buffer(3);
    buffer.put("A", 10);  // freq(A)=1
    buffer.put("B", 20);  // freq(B)=1
    buffer.put("C", 30);  // freq(C)=1

    // Access A 3 times => freq(A)=4
    EXPECT_EQ(buffer.get("A"), 10);
    EXPECT_EQ(buffer.get("A"), 10);
    EXPECT_EQ(buffer.get("A"), 10);

    // Access B 1 more time => freq(B)=2
    EXPECT_EQ(buffer.get("B"), 20);

    // Insert "D" => must evict the least frequent, which is "C" (freq=1)
    buffer.put("D", 40);
    // "C" should be gone
    EXPECT_THROW(buffer.get("C"), std::runtime_error);
    // "A", "B", "D" should still remain
    EXPECT_NO_THROW(buffer.get("A"));
    EXPECT_NO_THROW(buffer.get("B"));
    EXPECT_NO_THROW(buffer.get("D"));
    EXPECT_EQ(buffer.size(), 3u);
}

// --------------------------------------------------------
// 2) Test resize(newCapacity)
// --------------------------------------------------------

TEST(FreqBufferExtendedTest, Resize_IncreaseCapacity) {
    FreqBuffer<std::string, int> buffer(2);
    // Fill up
    buffer.put("X", 1);
    buffer.put("Y", 2);
    EXPECT_EQ(buffer.size(), 2u);

    // Resize up to 4
    buffer.resize(4);
    EXPECT_EQ(buffer.capacity(), 4u);
    
    // Insert more items => should not evict
    buffer.put("A", 10);
    buffer.put("B", 20);
    // Now we have 4 items
    EXPECT_EQ(buffer.size(), 4u);
}

TEST(FreqBufferExtendedTest, Resize_DecreaseCapacity) {
    FreqBuffer<std::string, int> buffer(5);
    buffer.put("A", 1);  // freq(A)=1
    buffer.put("B", 2);  // freq(B)=1
    buffer.put("C", 3);  // freq(C)=1
    buffer.put("D", 4);  // freq(D)=1
    buffer.put("E", 5);  // freq(E)=1

    // Increase freq of "A" a bit
    buffer.get("A"); // freq(A)=2
    buffer.get("A"); // freq(A)=3
    buffer.get("B"); // freq(B)=2

    // Now resize to 2
    buffer.resize(2);
    EXPECT_EQ(buffer.capacity(), 2u);

    // A and B should still remain
    EXPECT_EQ(buffer.get("A"), 1); 
    EXPECT_EQ(buffer.get("B"), 2); 
    EXPECT_LE(buffer.size(), 2u);
   
    // So "C", "D", "E" should be gone
    EXPECT_THROW(buffer.get("C"), std::runtime_error);
    EXPECT_THROW(buffer.get("D"), std::runtime_error);
    EXPECT_THROW(buffer.get("E"), std::runtime_error);
}

TEST(FreqBufferExtendedTest, Resize_ZeroEdgeCase) {
    FreqBuffer<std::string, int> buffer(3);
    buffer.put("A", 10);
    buffer.put("B", 20);

    //should throw error on resize < 0
    EXPECT_THROW(buffer.resize(-1), std::runtime_error);
    //ensure the size and capacity are still the same 
    EXPECT_EQ(buffer.capacity(), 3u);
    EXPECT_EQ(buffer.size(), 2u);

    // this shouldn't raise any error
    buffer.resize(0);
    EXPECT_EQ(buffer.capacity(), 0u);
    EXPECT_EQ(buffer.size(), 0u);

    // Any put should fail or do nothing
    buffer.put("C", 30);
    EXPECT_EQ(buffer.size(), 0u);
}

// --------------------------------------------------------
// 3) Test getTopKFrequent(k)
//    Return k items with highest freq in descending frequency order.
//    Ties => ascending Value order.
// --------------------------------------------------------

TEST(FreqBufferExtendedTest, GetTopKFrequent_Basic) {
    FreqBuffer<std::string, int> buffer(5);
    // Insert
    buffer.put("A", 10);  // freq(A)=1
    buffer.put("B", 20);  // freq(B)=1
    buffer.put("C", 30);  // freq(C)=1
    buffer.put("D", 40);  // freq(D)=1
    buffer.put("E", 50);  // freq(E)=1

    // Bump frequencies
    // "A" => freq=3
    buffer.get("A");
    buffer.get("A");
    // "B" => freq=2
    buffer.get("B");
    // "C" => freq=3
    buffer.get("C");
    buffer.get("C");
    // "D" => freq=1
    // "E" => freq=1

    // Now we have in the right order for freq and ascending 
    // value for ties
    // A freq=3 (Value=10)
    // C freq=3 (Value=30)
    // B freq=2 (Value=20)
    // D freq=1 (Value=40)
    // E freq=1 (Value=50)

    auto top3 = buffer.getTopKFrequent(3);
    ASSERT_EQ(top3.size(), 3u);

    auto it = top3.begin();
    EXPECT_EQ(it->first, "A"); // freq=3, val=10
    EXPECT_EQ(it->second, 10);
    ++it;
    EXPECT_EQ(it->first, "C"); // freq=3, val=30
    EXPECT_EQ(it->second, 30);
    ++it;
    EXPECT_EQ(it->first, "B"); // freq=2, val=20
    EXPECT_EQ(it->second, 20);
}

TEST(FreqBufferExtendedTest, GetTopKFrequent_KGreaterThanSize) {
    FreqBuffer<int, int> buffer(3);
    buffer.put(1, 100); // freq=1
    buffer.put(2, 200); // freq=1
    buffer.put(3, 150); // freq=1

    // Increase freq of "1"
    buffer.get(1); // freq(1)=2
    buffer.get(1); // freq(1)=3

    // Now freq(1)=3, freq(2)=1, freq(3)=1
    // getTopKFrequent(5) => we have only 3 items
    auto top5 = buffer.getTopKFrequent(5);
    ASSERT_EQ(top5.size(), 3u);

    // Should be [ (1,100), (3,150), (2,200)] in descending freq and ascending value
    auto it = top5.begin();
    EXPECT_EQ(it->first, 1); // highest freq=3
    EXPECT_EQ(it->second, 100);
    ++it;
    // For the tie freq=1 => value(200) < value(300), so (2,200) next, then (3,300)
    EXPECT_EQ(it->first, 3);
    EXPECT_EQ(it->second, 150);
    ++it;
    EXPECT_EQ(it->first, 2);
    EXPECT_EQ(it->second, 200);
}

TEST(FreqBufferExtendedTest, GetTopKFrequent_EmptyBuffer) {
    FreqBuffer<int, int> buffer(3);

    auto topK = buffer.getTopKFrequent(2);
    EXPECT_TRUE(topK.empty());
}

// --------------------------------------------------------
// 4) pop_least_used(n): Removes n least-frequently-used elements
// --------------------------------------------------------

TEST(FreqBufferExtendedTest, pop_least_used_Basic) {
    FreqBuffer<std::string, int> buffer(5);
    buffer.put("A", 1); // freq(A)=1
    buffer.put("B", 2); // freq(B)=1
    buffer.put("C", 3); // freq(C)=1
    // Bump freq of B => freq(B)=2
    EXPECT_EQ(buffer.get("B"), 2);

    // Bump freq of C => freq(C)=2
    EXPECT_EQ(buffer.get("C"), 3);

    // Bump freq of C => freq(C)=3
    EXPECT_EQ(buffer.get("C"), 3);

    // Now freq(A)=1, freq(B)=2, freq(C)=3
    // pop_least_used(1) => remove the item with freq=1 => "A"
    buffer.pop_least_used(1);
    EXPECT_THROW(buffer.get("A"), std::runtime_error); // "A" is gone

    // Buffer has B(freq=2), C(freq=3)
    EXPECT_NO_THROW(buffer.get("B"));
    EXPECT_NO_THROW(buffer.get("C"));
}

TEST(FreqBufferExtendedTest, pop_least_used_MultipleRemovals) {
    FreqBuffer<std::string, int> buffer(5);
    buffer.put("A", 10); // freq(A)=1
    buffer.put("B", 20); // freq(B)=1
    buffer.put("C", 30); // freq(C)=1
    buffer.put("D", 40); // freq(D)=1

    // Increase freq of C => freq(C)=2
    buffer.get("C");
    // Increase freq of D => freq(D)=3
    buffer.get("D");
    buffer.get("D");

    // Now freq(A)=1, freq(B)=1, freq(C)=2, freq(D)=3
    // pop_least_used(2) => remove 2 items with the lowest freq => "A" and "B" (both freq=1)
    buffer.pop_least_used(2);
    EXPECT_THROW(buffer.get("A"), std::runtime_error);
    EXPECT_THROW(buffer.get("B"), std::runtime_error);
    // "C" and "D" remain
    EXPECT_NO_THROW(buffer.get("C"));
    EXPECT_NO_THROW(buffer.get("D"));
    EXPECT_EQ(buffer.size(), 2u);
}

TEST(FreqBufferExtendedTest, pop_least_used_MoreThanSize) {
    FreqBuffer<int,int> buffer(3);
    buffer.put(1, 10);
    buffer.put(2, 20);

    // pop_least_used(5) => should remove everything, because n >= size
    buffer.pop_least_used(5);
    EXPECT_EQ(buffer.size(), 0u);
    EXPECT_THROW(buffer.get(1), std::runtime_error);
    EXPECT_THROW(buffer.get(2), std::runtime_error);
}

TEST(FreqBufferExtendedTest, pop_least_used_ZeroOrNegative) {
    // We'll test 0 => no removal
    FreqBuffer<int,int> buffer(3);
    buffer.put(1, 100);

    buffer.pop_least_used(0);
    EXPECT_EQ(buffer.size(), 1u); // no removal
    EXPECT_EQ(buffer.get(1), 100);

    // If negative => do nothing
    buffer.pop_least_used(-2);
    EXPECT_EQ(buffer.size(), 1u);
    EXPECT_EQ(buffer.get(1), 100);
}

// End of test file
