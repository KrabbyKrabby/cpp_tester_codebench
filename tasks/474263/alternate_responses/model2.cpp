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
        
        std::vector<ManagedObject*> liveObjects;
        // Collect live objects
        for (ManagedObject* obj : allocatedObjects) {
            if (obj->isMarked()) {
                liveObjects.push_back(obj);
            }
        }
        
        // Create new contiguous memory block
        allocatedObjects = liveObjects;
        
        // Update references
        for (ManagedObject* obj : allocatedObjects) {
            for (size_t i = 0; i < obj->references.size(); ++i) {
                auto it = std::find(liveObjects.begin(), liveObjects.end(), obj->references[i]);
                if (it != liveObjects.end()) {
                    obj->references[i] = *it;
                }
            }
        }
    }

    void markAll() {
        std::lock_guard<std::mutex> lock(gcMutex);
        std::queue<ManagedObject*> queue;
        
        // Start with root objects
        for (ManagedObject* root : roots) {
            if (!root->isMarked()) {
                queue.push(root);
                root->setMarked(true);
            }
        }
        
        // Iterative marking
        while (!queue.empty()) {
            ManagedObject* current = queue.front();
            queue.pop();
            
            for (ManagedObject* ref : current->references) {
                if (!ref->isMarked()) {
                    ref->setMarked(true);
                    queue.push(ref);
                }
            }
        }
    }

    void mark(ManagedObject* obj) {
        std::lock_guard<std::mutex> lock(gcMutex);
        if (!obj || obj->isMarked()) {
            return;
        }
        
        std::stack<ManagedObject*> stack;
        stack.push(obj);
        
        while (!stack.empty()) {
            ManagedObject* current = stack.top();
            stack.pop();
            
            if (!current->isMarked()) {
                current->setMarked(true);
                for (ManagedObject* ref : current->references) {
                    if (!ref->isMarked()) {
                        stack.push(ref);
                    }
                }
            }
        }
    }

    void handleCircularReferences() {
        std::lock_guard<std::mutex> lock(gcMutex);
        std::unordered_set<ManagedObject*> visited;
        std::vector<ManagedObject*> cycleStack;
        
        for (ManagedObject* obj : allocatedObjects) {
            if (!obj->isMarked()) {
                std::unordered_set<ManagedObject*> componentVisited;
                std::vector<ManagedObject*> componentStack;
                
                if (detectCycle(obj, componentVisited, componentStack)) {
                    // Break cycles by clearing references in the cycle
                    for (ManagedObject* cycleObj : componentStack) {
                        cycleObj->references.clear();
                    }
                }
            }
        }
    }
};

#endif // __MARK_AND_SWEEP_GC__
