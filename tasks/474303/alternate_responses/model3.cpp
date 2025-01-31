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

    static void* allocate(size_t size) {
        if (size == 0) return nullptr;

        std::lock_guard<std::mutex> lock(mutex);
        
        Header* header = getFreeBlock(size);
        if (header) {
            header->is_free = false;
            return (void*)(header + 1);
        }

        size_t total_size = sizeof(Header) + size;
        header = (Header*)malloc(total_size);
        
        if (!header) return nullptr;

        header->size = size;
        header->is_free = false;
        header->next = nullptr;

        if (!head) {
            head = tail = header;
        } else {
            tail->next = header;
            tail = header;
        }

        return (void*)(header + 1);
    }

    static void deallocate(void* block) {
        if (!block) return;

        std::lock_guard<std::mutex> lock(mutex);
        
        Header* header = ((Header*)block) - 1;
        header->is_free = true;

        // Merge adjacent free blocks
        Header* current = head;
        while (current && current->next) {
            if (current->is_free && current->next->is_free) {
                current->size += sizeof(Header) + current->next->size;
                current->next = current->next->next;
                if (current->next == nullptr) {
                    tail = current;
                }
            } else {
                current = current->next;
            }
        }
    }

    static void* reallocate(void* block, size_t new_size) {
        if (!block) return allocate(new_size);
        if (new_size == 0) {
            deallocate(block);
            return nullptr;
        }

        std::lock_guard<std::mutex> lock(mutex);
        
        Header* header = ((Header*)block) - 1;
        
        if (header->size >= new_size) return block;

        void* new_block = allocate(new_size);
        if (!new_block) return nullptr;

        memcpy(new_block, block, header->size);
        deallocate(block);
        
        return new_block;
    }

    static void* zeroInitialize(size_t num, size_t size) {
        size_t total_size = num * size;
        if (total_size == 0) return nullptr;
        
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