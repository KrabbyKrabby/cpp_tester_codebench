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
    static const size_t ALIGNMENT = sizeof(void*);
    static const size_t HEADER_SIZE = sizeof(Header);

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

        std::lock_guard<std::mutex> lock(mutex);
        Header* header = reinterpret_cast<Header*>(static_cast<char*>(block) - HEADER_SIZE);
        header->is_free = true;

        // Coalesce with next block if it's free
        if (header->next && header->next->is_free) {
            header->size += header->next->size + HEADER_SIZE;
            header->next = header->next->next;
        }

        // Coalesce with previous block if it's free
        Header* current = head;
        while (current && current->next != header) {
            current = current->next;
        }
        if (current && current->is_free) {
            current->size += header->size + HEADER_SIZE;
            current->next = header->next;
        }
    }

    static void* allocate(size_t size) {
        if (size == 0) return nullptr;

        std::lock_guard<std::mutex> lock(mutex);
        size_t aligned_size = (size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
        
        // Try to find a free block
        Header* free_block = getFreeBlock(aligned_size);
        if (free_block) {
            free_block->is_free = false;
            return reinterpret_cast<char*>(free_block) + HEADER_SIZE;
        }

        // Allocate new block
        size_t total_size = HEADER_SIZE + aligned_size;
        Header* new_block = reinterpret_cast<Header*>(HeapAlloc(GetProcessHeap(), 0, total_size));
        if (!new_block) return nullptr;

        new_block->size = aligned_size;
        new_block->is_free = false;
        new_block->next = nullptr;

        // Update linked list
        if (!head) {
            head = new_block;
        } else {
            tail->next = new_block;
        }
        tail = new_block;

        return reinterpret_cast<char*>(new_block) + HEADER_SIZE;
    }

    static void* reallocate(void* block, size_t size) {
        if (!block) return allocate(size);
        if (size == 0) {
            deallocate(block);
            return nullptr;
        }

        std::lock_guard<std::mutex> lock(mutex);
        Header* header = reinterpret_cast<Header*>(static_cast<char*>(block) - HEADER_SIZE);
        
        // If current block is big enough, just return it
        if (header->size >= size) return block;

        // Allocate new block and copy data
        void* new_block = allocate(size);
        if (!new_block) return nullptr;

        std::memcpy(new_block, block, header->size);
        deallocate(block);
        return new_block;
    }

    static void* zeroInitialize(size_t num, size_t size) {
        size_t total_size = num * size;
        if (num != 0 && total_size / num != size) return nullptr; // Check for overflow

        void* block = allocate(total_size);
        if (block) {
            std::memset(block, 0, total_size);
        }
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

// Static member initialization
std::mutex MemoryManager::mutex;
Header* MemoryManager::head = nullptr;
Header* MemoryManager::tail = nullptr;

#endif //__MEM_ALLOC__