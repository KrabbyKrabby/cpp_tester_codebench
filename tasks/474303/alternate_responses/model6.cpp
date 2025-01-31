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
        if (!block) return;

        Header* header = static_cast<Header*>(block) - 1;
        std::lock_guard<std::mutex> lock(mutex);
        header->is_free = true;
    }

    static void* allocate(size_t size) {
        std::lock_guard<std::mutex> lock(mutex);

        // Check if there's a free block that's large enough
        Header* freeBlock = getFreeBlock(size);
        if (freeBlock) {
            freeBlock->is_free = false;
            return static_cast<void*>(freeBlock + 1);
        }

        // If not, allocate a new block
        Header* newBlock = static_cast<Header*>(HeapAlloc(GetProcessHeap(), 0, size + sizeof(Header)));
        if (!newBlock) return nullptr; // Allocation failed

        newBlock->size = size;
        newBlock->is_free = false;
        newBlock->next = nullptr;

        if (!head) {
            head = newBlock;
            tail = newBlock;
        } else {
            tail->next = newBlock;
            tail = newBlock;
        }

        return static_cast<void*>(newBlock + 1);
    }

    static void* reallocate(void* block, size_t size) {
        if (!block) return allocate(size);

        Header* header = static_cast<Header*>(block) - 1;
        std::lock_guard<std::mutex> lock(mutex);

        if (header->size >= size) {
            // The current block is large enough, just return it
            return block;
        }

        // Allocate a new block of the requested size
        void* newBlock = allocate(size);
        if (!newBlock) return nullptr; // Allocation failed

        // Copy data from the old block to the new block
        memcpy(newBlock, block, header->size);

        // Free the old block
        deallocate(block);

        return newBlock;
    }

    static void* zeroInitialize(size_t num, size_t size) {
        size_t totalSize = num * size;
        void* block = allocate(totalSize);
        if (!block) return nullptr; // Allocation failed

        // Zero-initialize the allocated block
        std::memset(block, 0, totalSize);

        return block;
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

// Initialize static members
std::mutex MemoryManager::mutex;
MemoryManager::Header* MemoryManager::head = nullptr;
MemoryManager::Header* MemoryManager::tail = nullptr;

#endif //__MEM_ALLOC__