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
        if (block == nullptr) {
            return; // Ignore nullptr
        }

        Header* header = static_cast<Header*>(static_cast<char*>(block) - sizeof(Header));
        header->is_free = true;

        // Merge adjacent free blocks
        mergeAdjacentFreeBlocks(header);
    }

    static void* allocate(size_t size) {
        std::lock_guard<std::mutex> lock(mutex);
        if (size == 0) {
            return nullptr; // Zero-size allocation is not allowed
        }

        Header* freeBlock = getFreeBlock(size);
        if (freeBlock) {
            // Split the free block if it's larger than the requested size
            splitFreeBlock(freeBlock, size);
            return static_cast<void*>(static_cast<char*>(freeBlock) + sizeof(Header));
        } else {
            // Allocate a new block if no free block is available
            return allocateNewBlock(size);
        }
    }

    static void* reallocate(void* block, size_t size) {
        std::lock_guard<std::mutex> lock(mutex);
        if (block == nullptr) {
            return allocate(size); // Reallocate nullptr as a new allocation
        }

        Header* header = static_cast<Header*>(static_cast<char*>(block) - sizeof(Header));
        if (header->size >= size) {
            // If the current block is large enough, split it if necessary
            splitFreeBlock(header, size);
            return block;
        } else {
            // Allocate a new block and copy data from the old block
            void* newBlock = allocate(size);
            if (newBlock) {
                std::memcpy(newBlock, block, header->size);
                deallocate(block);
            }
            return newBlock;
        }
    }

    static void* zeroInitialize(size_t num, size_t size) {
        std::lock_guard<std::mutex> lock(mutex);
        size_t totalSize = num * size;
        void* block = allocate(totalSize);
        if (block) {
            std::memset(block, 0, totalSize);
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

    static void mergeAdjacentFreeBlocks(Header* header) {
        if (header->next && header->next->is_free) {
            header->size += header->next->size;
            header->next = header->next->next;
            if (header->next == nullptr) {
                tail = header;
            }
            mergeAdjacentFreeBlocks(header); // Recursively merge adjacent free blocks
        }
    }

    static void splitFreeBlock(Header* header, size_t size) {
        if (header->size > size + sizeof(Header)) {
            Header* newHeader = static_cast<Header*>(static_cast<char*>(header) + sizeof(Header) + size);
            newHeader->size = header->size - size - sizeof(Header);
            newHeader->is_free = true;
            newHeader->next = header->next;
            header->size = size;
            header->next = newHeader;
            if (newHeader->next == nullptr) {
                tail = newHeader;
            }
        }
        header->is_free = false;
    }

    static void* allocateNewBlock(size_t size) {
        Header* newHeader = static_cast<Header*>(HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size + sizeof(Header)));
        if (newHeader) {
            newHeader->size = size;
            newHeader->is_free = false;
            newHeader->next = nullptr;
            if (head == nullptr) {
                head = newHeader;
            } else {
                tail->next = newHeader;
            }
            tail = newHeader;
            return static_cast<void*>(static_cast<char*>(newHeader) + sizeof(Header));
        }
        return nullptr;
    }
};

std::mutex MemoryManager::mutex;
MemoryManager::Header* MemoryManager::head = nullptr;
MemoryManager::Header* MemoryManager::tail = nullptr;

#endif //__MEM_ALLOC__