#ifndef __MARK_AND_SWEEP_GC__
#define __MARK_AND_SWEEP_GC__
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <mutex>
#include <queue>
#include <stack>

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

    std::unordered_set<ManagedObject*> roots;
    std::mutex gcMutex;
    std::recursive_mutex gcMutex_r;

public:
    std::vector<ManagedObject*> allocatedObjects;
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
            if (!(*it)->isMarked()) {
                delete *it;
                it = allocatedObjects.erase(it);
            } else {
                (*it)->setMarked(false); // Reset mark for next cycle
                ++it;
            }
        }
    }



    bool detectCycle(ManagedObject* obj, 
                 std::unordered_set<ManagedObject*>& visited,
                 std::vector<ManagedObject*>& stack) {
    std::lock_guard<std::recursive_mutex> lock(gcMutex_r);
    
    visited.insert(obj);
    stack.push_back(obj);

    for (ManagedObject* neighbor : obj->references) {
        if (visited.find(neighbor) == visited.end()) {
            if (detectCycle(neighbor, visited, stack)) {
                return true;
            }
        } else if (std::find(stack.begin(), stack.end(), neighbor) != stack.end()) {
            return true; // Cycle detected
        }
    }

    stack.pop_back();
    return false;
}

    void compactMemory() {
        std::lock_guard<std::mutex> lock(gcMutex);
        
        std::vector<ManagedObject*> liveObjects;
        // Collect live objects, After sweep, unmarked objects gets removed while marked objects get unmarked.
        for (ManagedObject* obj : allocatedObjects) {
            if (!obj->isMarked()) {
                liveObjects.push_back(obj);
            }
        }
        

        // Create new contiguous memory block
        allocatedObjects = liveObjects;
        std::sort(allocatedObjects.begin(), allocatedObjects.end());
        
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
        for (ManagedObject* root : roots) {
            if (!root->isMarked()) {
                mark(root);
            }
        }
    }


    void mark(ManagedObject* obj) {
        if (obj && !obj->isMarked()) {
            obj->setMarked(true);
            for (ManagedObject* neighbor : obj->references) {
                mark(neighbor);
            }
        }
    }


    void handleCircularReferences() {
        std::lock_guard<std::recursive_mutex> lock(gcMutex_r);
        std::unordered_set<ManagedObject*> visited;
        std::vector<ManagedObject*> stack;

        for (ManagedObject* obj : allocatedObjects) {
            if (visited.find(obj) == visited.end()) {
                std::unordered_set<ManagedObject*> cycleVisited;
                std::vector<ManagedObject*> cycleStack;

                if (detectCycle(obj, cycleVisited, cycleStack)) {
                    // Break cycles by nullifying references
                    for (ManagedObject* cycleObj : cycleStack) {
                        if (!cycleObj->isMarked()) {
                            cycleObj->references.clear();
                        }
                    }
                }
            }
        }
    }
};

#endif // __MARK_AND_SWEEP_GC__