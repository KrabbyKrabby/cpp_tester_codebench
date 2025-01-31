#ifndef __MEM_ALLOC__
#define __MEM_ALLOC__

#include <windows.h>
#include <iostream>
#include <cstring>
#include <mutex>

class MemoryManager {
public:
    struct Header {
        size_t size;
        bool is_free;
        Header* next;
    };

private:
    static std::mutex mutex;
    static Header* head;
    static Header* tail;


public:
    MemoryManager() = delete; // Disallow creating an instance

    static void reset() {
        std::lock_guard<std::mutex> lock(mutex);
        head = nullptr;
        tail = nullptr;
        // Also consider releasing any allocated memory blocks if necessary
    }

    static void deallocate(void* block) {

    }


    static void* allocate(size_t size) {
         
    }

    static void* reallocate(void* block, size_t size) {
    
    }

    static void* zeroInitialize(size_t num, size_t size) {
         
    }

private:
    static Header* getFreeBlock(size_t size) {
        Header* curr = head;
        while (curr) {
            if (curr->is_free && curr->size >= size) {
                return curr;
            }
            curr = curr->next;
        }
        return nullptr;
    }
};


#endif //__MEM_ALLOC__