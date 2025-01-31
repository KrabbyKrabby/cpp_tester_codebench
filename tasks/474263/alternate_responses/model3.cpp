#ifndef __MARK_AND_SWEEP_GC__
#define __MARK_AND_SWEEP_GC__
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <mutex>
#include <stack>
#include <queue>

// [Previous ManagedObject class implementation remains unchanged]

template<typename T>
class GarbageCollector {
    // [Previous member declarations remain unchanged]

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
        for (ManagedObject* obj : allocatedObjects) {
            if (obj->isMarked()) {
                liveObjects.push_back(obj);
            }
        }

        // Create a new contiguous block of memory
        std::vector<ManagedObject*> compactedMemory;
        compactedMemory.reserve(liveObjects.size());

        // Move objects to new locations
        for (ManagedObject* obj : liveObjects) {
            ManagedObject* newLocation = new ManagedObject(*obj);
            compactedMemory.push_back(newLocation);
        }

        // Update references
        for (size_t i = 0; i < compactedMemory.size(); ++i) {
            for (ManagedObject*& ref : compactedMemory[i]->references) {
                auto it = std::find(liveObjects.begin(), liveObjects.end(), ref);
                if (it != liveObjects.end()) {
                    size_t index = std::distance(liveObjects.begin(), it);
                    ref = compactedMemory[index];
                }
            }
        }

        // Clean up old objects
        for (ManagedObject* obj : allocatedObjects) {
            delete obj;
        }

        allocatedObjects = compactedMemory;
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

        // Iterative marking to avoid stack overflow
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
        std::vector<ManagedObject*> cyclicGroups;

        for (ManagedObject* obj : allocatedObjects) {
            if (visited.find(obj) == visited.end()) {
                std::vector<ManagedObject*> stack;
                if (detectCycle(obj, visited, stack)) {
                    // Found a cycle, check if any object in the cycle has external references
                    bool hasExternalRefs = false;
                    for (ManagedObject* cycleObj : stack) {
                        if (roots.find(cycleObj) != roots.end()) {
                            hasExternalRefs = true;
                            break;
                        }
                    }
                    
                    if (!hasExternalRefs) {
                        cyclicGroups.insert(cyclicGroups.end(), stack.begin(), stack.end());
                    }
                }
            }
        }

        // Clean up cyclic references without external references
        for (ManagedObject* obj : cyclicGroups) {
            obj->references.clear();
            obj->setMarked(false);
        }
    }
};

#endif // __MARK_AND_SWEEP_GC__