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

    void sweep() {
        std::lock_guard<std::mutex> lock(gcMutex);
        allocatedObjects.erase(std::remove_if(allocatedObjects.begin(), allocatedObjects.end(),
            [](ManagedObject* obj) { return !obj->isMarked(); }), allocatedObjects.end());
        for (ManagedObject* obj : allocatedObjects) {
            obj->setMarked(false);
        }
    }

    bool detectCycle(ManagedObject* obj, std::unordered_set<ManagedObject*>& visited, std::vector<ManagedObject*>& stack) {
        if (visited.find(obj) != visited.end()) {
            return std::find(stack.begin(), stack.end(), obj) != stack.end();
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

    void compactMemory() {
        std::lock_guard<std::mutex> lock(gcMutex);
        std::vector<ManagedObject*> newAllocatedObjects;
        for (ManagedObject* obj : allocatedObjects) {
            if (obj->isMarked()) {
                newAllocatedObjects.push_back(obj);
            }
        }
        allocatedObjects = newAllocatedObjects;
    }

    void markAll() {
        std::lock_guard<std::mutex> lock(gcMutex);
        std::unordered_set<ManagedObject*> visited;
        for (ManagedObject* root : roots) {
            mark(root, visited);
        }
    }

    void mark(ManagedObject* obj, std::unordered_set<ManagedObject*>& visited) {
        if (visited.find(obj) != visited.end()) {
            return;
        }
        visited.insert(obj);
        obj->setMarked(true);
        for (ManagedObject* ref : obj->references) {
            mark(ref, visited);
        }
    }

    void handleCircularReferences() {
        std::lock_guard<std::mutex> lock(gcMutex);
        std::unordered_set<ManagedObject*> visited;
        std::vector<ManagedObject*> stack;
        for (ManagedObject* obj : allocatedObjects) {
            if (detectCycle(obj, visited, stack)) {
                obj->setMarked(false);
            }
        }
    }
};

#endif // __MARK_AND_SWEEP_GC__
