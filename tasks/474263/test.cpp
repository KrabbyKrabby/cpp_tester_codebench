#include <gtest/gtest.h>
#include <algorithm>
#include <random> 
#include "solution.cpp" 

class GarbageCollectorTest : public ::testing::Test {
protected:
    GarbageCollector<ManagedObject>* gc;

    void SetUp() override {
        gc = new GarbageCollector<ManagedObject>();
    }

    void TearDown() override {
        delete gc;
    }
};

// Test the sweep function, which should clean up unmarked objects
TEST_F(GarbageCollectorTest, SweepFunction) {
    ManagedObject* obj1 = new ManagedObject();
    ManagedObject* obj2 = new ManagedObject();
    obj2->use(); // Mark this object as used
    gc->trackAllocation(obj1);
    gc->trackAllocation(obj2);

    gc->sweep(); // Perform sweep, should delete obj1 but keep obj2 umarked for the next cycle.

    ASSERT_FALSE(obj2->isMarked());
    ASSERT_EQ(gc->allocatedObjects.size(), 1); // Only obj2 should remain
}

// Test the detectCycle function for correct cycle detection
TEST_F(GarbageCollectorTest, DetectCycleFunction) {
    ManagedObject* obj1 = new ManagedObject();
    ManagedObject* obj2 = new ManagedObject();
    ManagedObject* obj3 = new ManagedObject();
    obj1->addReference(obj2);
    obj2->addReference(obj3);
    obj3->addReference(obj1); // Create a cycle

    gc->trackAllocation(obj1);
    gc->trackAllocation(obj2);
    gc->trackAllocation(obj3);

    // bool hasCycle = gc->detectCycle(obj1, std::unordered_set<ManagedObject*>(), std::vector<ManagedObject*>());
    std::unordered_set<ManagedObject*> visited;
    std::vector<ManagedObject*> stack;
    bool hasCycle = gc->detectCycle(obj1, visited, stack);
    ASSERT_TRUE(hasCycle); // Should detect a cycle
}


TEST_F(GarbageCollectorTest, CycleDetection) {
    ManagedObject* obj1 = new ManagedObject();
    ManagedObject* obj2 = new ManagedObject();
    ManagedObject* obj3 = new ManagedObject();

    obj1->addReference(obj2);
    obj2->addReference(obj3);
    obj3->addReference(obj1);  // Creates a circular reference

    gc->trackAllocation(obj1);
    gc->trackAllocation(obj2);
    gc->trackAllocation(obj3);

    std::unordered_set<ManagedObject*> visited;
    std::vector<ManagedObject*> stack;

    // This should detect a cycle
    bool hasCycle = gc->detectCycle(obj1, visited, stack);

    ASSERT_TRUE(hasCycle);
}





// Test memory compaction function to ensure it rearranges objects efficiently
TEST_F(GarbageCollectorTest, CompactMemoryFunction) {
    // Populate the GC with objects
    for (int i = 0; i < 10; i++) {
        ManagedObject* obj = new ManagedObject();
        gc->trackAllocation(obj);
        if (i % 2 == 0) {
            obj->use(); // Mark even-indexed objects as in use
        } else {
            obj->release(); // Unmark odd-indexed objects
        }
    }

    // Perform a sweep to remove unmarked objects - we need to implement this logic
    gc->sweep();

    // Verify reduction in fragmentation
    size_t sizeBeforeCompaction = gc->allocatedObjects.size();

    // Shuffle objects to simulate fragmentation
    std::random_device rd; // Non-deterministic random device
    std::mt19937 g(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::shuffle(gc->allocatedObjects.begin(), gc->allocatedObjects.end(), g);

    // Perform compaction
    gc->compactMemory();

    // Verify objects are possibly more contiguous - this is symbolic since we cannot check addresses
    // We expect objects to be in a sorted order if they were moved contiguously
    bool contiguous = true;
    for (size_t i = 1; i < gc->allocatedObjects.size(); i++) {
        if (gc->allocatedObjects[i - 1] > gc->allocatedObjects[i]) {
            contiguous = false;
            break;
        }
    }

    ASSERT_TRUE(contiguous);
    ASSERT_EQ(sizeBeforeCompaction, gc->allocatedObjects.size()); // Ensure no objects were lost
}

// Test marking all roots and their reachable objects
TEST_F(GarbageCollectorTest, MarkAllFunction) {
    ManagedObject* root = new ManagedObject();
    ManagedObject* child1 = new ManagedObject();
    ManagedObject* child2 = new ManagedObject();
    root->addReference(child1);
    child1->addReference(child2);

    gc->trackAllocation(root);
    gc->trackAllocation(child1);
    gc->trackAllocation(child2);
    gc->addRoot(root);

    gc->markAll(); // Mark all roots and their reachable objects

    ASSERT_TRUE(root->isMarked());
    ASSERT_TRUE(child1->isMarked());
    ASSERT_TRUE(child2->isMarked()); // All should be marked as they are reachable from root
}

// Test the handling of circular references
TEST_F(GarbageCollectorTest, HandleCircularReferencesFunction) {
    ManagedObject* obj1 = new ManagedObject();
    ManagedObject* obj2 = new ManagedObject();
    ManagedObject* obj3 = new ManagedObject();
    obj1->addReference(obj2);
    obj2->addReference(obj3);
    obj3->addReference(obj1); // Create a cycle

    gc->trackAllocation(obj1);
    gc->trackAllocation(obj2);
    gc->trackAllocation(obj3);

    gc->handleCircularReferences(); // Should resolve circular references
    gc->sweep(); // Now sweep should be able to collect them

    ASSERT_EQ(gc->allocatedObjects.size(), 0); // All objects should be collected
}