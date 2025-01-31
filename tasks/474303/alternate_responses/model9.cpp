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

    static void* allocateNewBlock(size_t size) {
        Header* new_block = static_cast<Header*>(HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size + sizeof(Header)));
        if (new_block == nullptr) {
            return nullptr;
        }
        new_block->size = size;
        new_block->is_free = false;
        new_block->next = nullptr;

        std::lock_guard<std::mutex> lock(mutex);
        if (head == nullptr) {
            head = new_block;
            tail = new_block;
        } else {
            tail->next = new_block;
            tail = new_block;
        }

        return static_cast<void*>(static_cast<char*>(new_block) + sizeof(Header));
    }

public:
    MemoryManager() = delete; // Disallow creating an instance

    static void reset() {
        std::lock_guard<std::mutex> lock(mutex);
        Header* curr = head;
        while (curr) {
            Header* next = curr->next;
            HeapFree(GetProcessHeap(), 0, curr);
            curr = next;
        }
        head = nullptr;
        tail = nullptr;
    }

    static void deallocate(void* block) {
        if (block == nullptr) {
            return;
        }

        std::lock_guard<std::mutex> lock(mutex);
        Header* header = static_cast<Header*>(static_cast<char*>(block) - sizeof(Header));
        header->is_free = true;
    }

    static void* allocate(size_t size) {
        Header* free_block = getFreeBlock(size);
        if (free_block) {
            std::lock_guard<std::mutex> lock(mutex);
            free_block->is_free = false;
            return static_cast<void*>(static_cast<char*>(free_block) + sizeof(Header));
        }
        return allocateNewBlock(size);
    }

    static void* reallocate(void* block, size_t size) {
        if (block == nullptr) {
            return allocate(size);
        }

        std::lock_guard<std::mutex> lock(mutex);
        Header* header = static_cast<Header*>(static_cast<char*>(block) - sizeof(Header));
        if (header->size >= size) {
            return block;
        }

        void* new_block = allocate(size);
        if (new_block == nullptr) {
            return nullptr;
        }

        memcpy(new_block, block, header->size);
        deallocate(block);
        return new_block;
    }

    static void* zeroInitialize(size_t num, size_t size) {
        size_t total_size = num * size;
        void* block = allocate(total_size);
        if (block != nullptr) {
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

std::mutex MemoryManager::mutex;
MemoryManager::Header* MemoryManager::head = nullptr;
MemoryManager::Header* MemoryManager::tail = nullptr;

#endif //__MEM_ALLOC__