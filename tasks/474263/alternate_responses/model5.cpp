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

    bool detectCycle(ManagedObject* obj, std::unordered_set<ManagedObject*>& visited, 
                    std::vector<ManagedObject*>& stack) {
        std::lock_guard<std::mutex> lock(gcMutex);
        
        visited.insert(obj);
        stack.push_back(obj);

        for (ManagedObject* ref : obj->references) {
            if (visited.find(ref) == visited.end()) {
                if (detectCycle(ref, visited, stack)) {
                    return true;
                }
            } else if (std::find(stack.begin(), stack.end(), ref) != stack.end()) {
                return true; // Cycle detected
            }
        }

        stack.pop_back();
        return false;
    }

    void compactMemory() {
        std::lock_guard<std::mutex> lock(gcMutex);
        
        std::vector<ManagedObject*> compactedObjects;
        std::unordered_map<ManagedObject*, ManagedObject*> addressMap;

        // First pass: Create new objects and build address mapping
        for (ManagedObject* obj : allocatedObjects) {
            if (obj->isMarked()) {
                ManagedObject* newObj = new ManagedObject(*obj);
                addressMap[obj] = newObj;
                compactedObjects.push_back(newObj);
            }
        }

        // Second pass: Update references using the address map
        for (ManagedObject* obj : compactedObjects) {
            for (auto& ref : obj->references) {
                auto it = addressMap.find(ref);
                if (it != addressMap.end()) {
                    ref = it->second;
                }
            }
        }

        // Clean up old objects and update allocated objects list
        for (ManagedObject* obj : allocatedObjects) {
            delete obj;
        }
        allocatedObjects = std::move(compactedObjects);
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
            if (!obj->isMarked() && visited.find(obj) == visited.end()) {
                std::unordered_set<ManagedObject*> cycleVisited;
                std::vector<ManagedObject*> cycleStack;
                
                if (detectCycle(obj, cycleVisited, cycleStack)) {
                    // Break cycles by nullifying references in the cycle
                    for (ManagedObject* cycleObj : cycleStack) {
                        cycleObj->references.clear();
                    }
                }
            }
        }
    }
};

#endif // __MARK_AND_SWEEP_GC__