#ifndef __MARK_AND_SWEEP_GC__
#define __MARK_AND_SWEEP_GC__
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <mutex>

class ManagedObject {
public:
    bool marked;
    std::vector<ManagedObject*> references;

    ManagedObject() : marked(false) {}

    void addReference(ManagedObject* obj) {
        references.push_back(obj);
    }

    void use() {
        marked = true;
    }

    void release() {
        marked = false;
    }

    bool isMarked() const {
        return marked;
    }

    void setMarked(bool value) {
        marked = value;
    }
};

template<typename T>
class GarbageCollector {
public:
    std::vector<ManagedObject*> allocatedObjects;
    std::unordered_set<ManagedObject*> roots;
    std::mutex gcMutex; // Mutex for thread-safe operations

    GarbageCollector() {}

    ~GarbageCollector() {
        for (ManagedObject* obj : allocatedObjects) {
            delete obj;
        }
        allocatedObjects.clear();
    }

    void trackAllocation(ManagedObject* ptr) {
        std::lock_guard<std::mutex> lock(gcMutex);
        allocatedObjects.push_back(ptr);
    }

    void removeAllocation(ManagedObject* ptr) {
        std::lock_guard<std::mutex> lock(gcMutex);
        allocatedObjects.erase(std::remove(allocatedObjects.begin(), allocatedObjects.end(), ptr), allocatedObjects.end());
    }

    void addRoot(ManagedObject* root) {
        std::lock_guard<std::mutex> lock(gcMutex);
        roots.insert(root);
    }

    void removeRoot(ManagedObject* root) {
        std::lock_guard<std::mutex> lock(gcMutex);
        roots.erase(root);
    }

    // Sweep Function
    void sweep() {
        std::lock_guard<std::mutex> lock(gcMutex);
        auto it = allocatedObjects.begin();
        while (it != allocatedObjects.end()) {
            if (!(*it)->isMarked()) {
                delete *it;
                it = allocatedObjects.erase(it);
            } else {
                (*it)->setMarked(false); // Clear mark for next cycle
                ++it;
            }
        }
    }

    // Circular Reference Detection
    bool detectCycle(ManagedObject* obj, std::unordered_set<ManagedObject*>& visited, std::vector<ManagedObject*>& stack) {
        if (visited.find(obj) != visited.end()) {
            if (std::find(stack.begin(), stack.end(), obj) != stack.end()) {
                return true; // Cycle detected
            }
            return false; // Already visited, no cycle
        }

        visited.insert(obj);
        stack.push_back(obj);

        for (ManagedObject* ref : obj->references) {
            if (detectCycle(ref, visited, stack)) {
                return true;
            }
        }

        stack.pop_back();
        return false;
    }

    // Compact Memory Function
    void compactMemory() {
        // This is a complex operation and its implementation may vary based on the specific requirements and constraints.
        // For simplicity, this example just rearranges the objects in the allocatedObjects vector to be contiguous.
        std::lock_guard<std::mutex> lock(gcMutex);
        std::vector<ManagedObject*> compactedObjects;
        for (ManagedObject* obj : allocatedObjects) {
            if (obj->isMarked()) {
                compactedObjects.push_back(obj);
            }
        }
        allocatedObjects = compactedObjects;
    }

    // Mark All Function
    void markAll() {
        std::lock_guard<std::mutex> lock(gcMutex);
        for (ManagedObject* root : roots) {
            mark(root);
        }
    }

    // Mark Function
    void mark(ManagedObject* obj) {
        if (obj->isMarked()) {
            return; // Already marked
        }

        obj->setMarked(true);

        for (ManagedObject* ref : obj->references) {
            mark(ref);
        }
    }

    // Handle Circular References Function
    void handleCircularReferences() {
        std::lock_guard<std::mutex> lock(gcMutex);
        std::unordered_set<ManagedObject*> visited;
        std::vector<ManagedObject*> stack;

        for (ManagedObject* obj : allocatedObjects) {
            if (!obj->isMarked() && detectCycle(obj, visited, stack)) {
                // Handle circular reference, for simplicity, just mark the objects in the cycle as used
                for (ManagedObject* ref : stack) {
                    ref->setMarked(true);
                }
            }
        }
    }
};

#endif // __MARK_AND_SWEEP_GC__
