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
    }

    static void deallocate(void* block) {
        if (!block) return;

        std::lock_guard<std::mutex> lock(mutex);
        
        // Get the header of the block
        Header* header = reinterpret_cast<Header*>(
            reinterpret_cast<char*>(block) - sizeof(Header)
        );
        
        header->is_free = true;

        // Attempt to coalesce with next block if it's free
        if (header->next && header->next->is_free) {
            header->size += sizeof(Header) + header->next->size;
            header->next = header->next->next;
            if (!header->next) {
                tail = header;
            }
        }

        // Attempt to coalesce with previous block if it's free
        Header* current = head;
        while (current && current->next != header) {
            current = current->next;
        }
        
        if (current && current->is_free) {
            current->size += sizeof(Header) + header->size;
            current->next = header->next;
            if (!current->next) {
                tail = current;
            }
        }
    }

    static void* allocate(size_t size) {
        if (size == 0) return nullptr;

        std::lock_guard<std::mutex> lock(mutex);
        
        Header* free_block = getFreeBlock(size);
        if (free_block) {
            free_block->is_free = false;
            return reinterpret_cast<char*>(free_block) + sizeof(Header);
        }

        // Allocate new block
        size_t total_size = sizeof(Header) + size;
        Header* new_block = reinterpret_cast<Header*>(malloc(total_size));
        
        if (!new_block) return nullptr;

        new_block->size = size;
        new_block->is_free = false;
        new_block->next = nullptr;

        if (!head) {
            head = tail = new_block;
        } else {
            tail->next = new_block;
            tail = new_block;
        }

        return reinterpret_cast<char*>(new_block) + sizeof(Header);
    }

    static void* reallocate(void* block, size_t size) {
        if (!block) return allocate(size);
        if (size == 0) {
            deallocate(block);
            return nullptr;
        }

        std::lock_guard<std::mutex> lock(mutex);
        
        Header* header = reinterpret_cast<Header*>(
            reinterpret_cast<char*>(block) - sizeof(Header)
        );

        // If new size is smaller, simply update the size
        if (size <= header->size) {
            header->size = size;
            return block;
        }

        // Check if we can expand into the next block
        if (header->next && header->next->is_free && 
            (header->size + sizeof(Header) + header->next->size >= size)) {
            header->size = size;
            header->next = header->next->next;
            if (!header->next) {
                tail = header;
            }
            return block;
        }

        // Allocate new block and copy data
        void* new_block = allocate(size);
        if (!new_block) return nullptr;

        memcpy(new_block, block, header->size);
        deallocate(block);
        return new_block;
    }

    static void* zeroInitialize(size_t num, size_t size) {
        size_t total_size = num * size;
        if (total_size / size != num) return nullptr; // Check for overflow
        
        void* block = allocate(total_size);
        if (block) {
            memset(block, 0, total_size);
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

// Define static members
std::mutex MemoryManager::mutex;
Header* MemoryManager::head = nullptr;
Header* MemoryManager::tail = nullptr;

#endif //__MEM_ALLOC__