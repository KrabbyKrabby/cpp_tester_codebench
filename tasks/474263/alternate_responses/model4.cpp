#ifndef __MARK_AND_SWEEP_GC__
#define __MARK_AND_SWEEP_GC__
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <mutex>
#include <stack>
#include <queue>

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
private:
    std::vector<ManagedObject*> allocatedObjects;
    std::unordered_set<ManagedObject*> roots;
    std::mutex gcMutex; // Mutex for thread-safe operations

public:
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
        allocatedObjects.erase(
            std::remove(allocatedObjects.begin(), allocatedObjects.end(), ptr),
            allocatedObjects.end()
        );
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
        auto it = allocatedObjects.begin();
        while (it != allocatedObjects.end()) {
            ManagedObject* obj = *it;
            if (!obj->isMarked()) {
                delete obj;
                it = allocatedObjects.erase(it);
            } else {
                obj->setMarked(false); // Reset mark for next cycle
                ++it;
            }
        }
    }

    bool detectCycle(ManagedObject* obj, 
                    std::unordered_set<ManagedObject*>& visited,
                    std::vector<ManagedObject*>& stack) {
        std::lock_guard<std::mutex> lock(gcMutex);
        
        if (std::find(stack.begin(), stack.end(), obj) != stack.end()) {
            return true; // Cycle detected
        }

        if (visited.find(obj) != visited.end()) {
            return false; // Already visited, but not in current path
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
        
        std::vector<ManagedObject*> compactedObjects;
        for (ManagedObject* obj : allocatedObjects) {
            if (obj->isMarked()) {
                compactedObjects.push_back(obj);
            }
        }

        // Update references to maintain object relationships
        for (ManagedObject* obj : compactedObjects) {
            std::vector<ManagedObject*> updatedRefs;
            for (ManagedObject* ref : obj->references) {
                if (std::find(compactedObjects.begin(), 
                            compactedObjects.end(), 
                            ref) != compactedObjects.end()) {
                    updatedRefs.push_back(ref);
                }
            }
            obj->references = updatedRefs;
        }

        allocatedObjects = compactedObjects;
    }

    void markAll() {
        std::lock_guard<std::mutex> lock(gcMutex);
        for (ManagedObject* root : roots) {
            if (!root->isMarked()) {
                mark(root);
            }
        }
    }

    void mark(ManagedObject* obj) {
        if (!obj || obj->isMarked()) {
            return;
        }

        std::queue<ManagedObject*> queue;
        queue.push(obj);

        while (!queue.empty()) {
            ManagedObject* current = queue.front();
            queue.pop();

            if (!current->isMarked()) {
                current->setMarked(true);
                for (ManagedObject* ref : current->references) {
                    if (!ref->isMarked()) {
                        queue.push(ref);
                    }
                }
            }
        }
    }

    void handleCircularReferences() {
        std::lock_guard<std::mutex> lock(gcMutex);
        std::unordered_set<ManagedObject*> visited;
        std::vector<ManagedObject*> stack;

        for (ManagedObject* obj : allocatedObjects) {
            if (!obj->isMarked()) {
                std::unordered_set<ManagedObject*> cycleGroup;
                if (detectCycle(obj, visited, stack)) {
                    // Handle the cycle by breaking references
                    for (ManagedObject* cycleObj : stack) {
                        cycleObj->references.clear();
                    }
                }
            }
        }
    }
};

#endif // __MARK_AND_SWEEP_GC__
