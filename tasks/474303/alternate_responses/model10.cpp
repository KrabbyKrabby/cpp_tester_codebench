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

        std::lock_guard<std::mutex> lock(mutex);

        // Get the header for the block
        Header* header = reinterpret_cast<Header*>(reinterpret_cast<char*>(block) - sizeof(Header));

        if (!header || !header->is_free) {
            std::cerr << "Error: Attempt to deallocate an invalid block." << std::endl;
            return;
        }

        header->is_free = true;

        // Merge adjacent free blocks
        mergeFreeBlocks(header);
    }

    static void* allocate(size_t size) {
        if (size == 0) return nullptr;

        std::lock_guard<std::mutex> lock(mutex);

        Header* free_block = getFreeBlock(size);

        if (free_block) {
            // If we found a free block, use it
            free_block->is_free = false;

            // If the block is larger than requested, split it
            if (free_block->size > size) {
                splitBlock(free_block, size);
            }

            return reinterpret_cast<void*>(reinterpret_cast<char*>(free_block) + sizeof(Header));
        } else {
            // If no free block is available, allocate a new one
            Header* new_block = reinterpret_cast<Header*>(malloc(sizeof(Header) + size));

            if (!new_block) {
                std::cerr << "Error: Memory allocation failed." << std::endl;
                return nullptr;
            }

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

            return reinterpret_cast<void*>(reinterpret_cast<char*>(new_block) + sizeof(Header));
        }
    }

    static void* reallocate(void* block, size_t size) {
        if (!block || size == 0) return nullptr;

        std::lock_guard<std::mutex> lock(mutex);

        // Get the header for the block
        Header* header = reinterpret_cast<Header*>(reinterpret_cast<char*>(block) - sizeof(Header));

        if (!header || header->is_free) {
            std::cerr << "Error: Attempt to reallocate an invalid block." << std::endl;
            return nullptr;
        }

        if (size <= header->size) {
            // If the new size is smaller, just update the size
            header->size = size;
            return block;
        } else {
            // If the new size is larger, we need to allocate a new block
            void* new_block = allocate(size);

            if (!new_block) {
                std::cerr << "Error: Memory reallocation failed." << std::endl;
                return nullptr;
            }

            // Copy the data from the old block to the new one
            std::memcpy(new_block, block, header->size);

            // Deallocate the old block
            deallocate(block);

            return new_block;
        }
    }

    static void* zeroInitialize(size_t num, size_t size) {
        size_t total_size = num * size;

        if (total_size == 0) return nullptr;

        std::lock_guard<std::mutex> lock(mutex);

        void* block = allocate(total_size);

        if (!block) {
            std::cerr << "Error: Memory allocation failed." << std::endl;
            return nullptr;
        }

        // Zero-initialize the allocated block
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

    static void splitBlock(Header* block, size_t size) {
        // Calculate the remaining size
        size_t remaining_size = block->size - size;

        // Update the size of the current block
        block->size = size;

        // Create a new block for the remaining size
        Header* new_block = reinterpret_cast<Header*>(reinterpret_cast<char*>(block) + sizeof(Header) + size);

        new_block->size = remaining_size;
        new_block->is_free = true;
        new_block->next = block->next;

        // Update the next pointer of the current block
        block->next = new_block;

        // If the new block is at the end, update the tail
        if (new_block->next == nullptr) {
            tail = new_block;
        }
    }

    static void mergeFreeBlocks(Header* block) {
        // Check if the next block is also free
        if (block->next && block->next->is_free) {
            // Merge the two blocks
            block->size += block->next->size;
            block->next = block->next->next;

            // If the merged block is at the end, update the tail
            if (block->next == nullptr) {
                tail = block;
            }

            // Recursively merge adjacent free blocks
            mergeFreeBlocks(block);
        }
    }
};

std::mutex MemoryManager::mutex;
MemoryManager::Header* MemoryManager::head = nullptr;
MemoryManager::Header* MemoryManager::tail = nullptr;

#endif //__MEM_ALLOC__