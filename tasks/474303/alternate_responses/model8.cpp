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
        std::lock_guard<std::mutex> lock(mutex);
        if (!block) return; // Check for nullptr

        Header* header = static_cast<Header*>(static_cast<char*>(block) - sizeof(Header));
        header->is_free = true;
    }

    static void* allocate(size_t size) {
        std::lock_guard<std::mutex> lock(mutex);
        if (size == 0) return nullptr; // Check for invalid size

        Header* free_block = getFreeBlock(size);
        if (free_block) {
            free_block->is_free = false;
            return static_cast<void*>(static_cast<char*>(free_block) + sizeof(Header));
        }

        // Allocate new memory block
        Header* new_block = static_cast<Header*>(HeapAlloc(GetProcessHeap(), 0, size + sizeof(Header)));
        if (!new_block) return nullptr; // Allocation failed

        new_block->size = size;
        new_block->is_free = false;
        new_block->next = nullptr;

        if (!head) {
            head = new_block;
            tail = new_block;
        } else {
            tail->next = new_block;
            tail = new_block;
        }

        return static_cast<void*>(static_cast<char*>(new_block) + sizeof(Header));
    }

    static void* reallocate(void* block, size_t size) {
        std::lock_guard<std::mutex> lock(mutex);
        if (!block || size == 0) return nullptr; // Check for nullptr or invalid size

        Header* header = static_cast<Header*>(static_cast<char*>(block) - sizeof(Header));
        if (header->size >= size) {
            return block; // No need to reallocate
        }

        void* new_block = allocate(size);
        if (!new_block) return nullptr; // Allocation failed

        std::memcpy(new_block, block, header->size);
        deallocate(block);

        return new_block;
    }

    static void* zeroInitialize(size_t num, size_t size) {
        std::lock_guard<std::mutex> lock(mutex);
        if (num == 0 || size == 0) return nullptr; // Check for invalid parameters

        size_t total_size = num * size;
        void* block = allocate(total_size);
        if (!block) return nullptr; // Allocation failed

        std::memset(block, 0, total_size);
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

#endif //__MEM_ALLOC__