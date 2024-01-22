#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include "allocator.h"

typedef struct Block {
    size_t size;
    struct Block* next;
} Block;

// Pointer to the beginning of the allocated memory pool
static void* memory_pool = NULL;

// Pointer to the beginning of the free memory list
static Block* free_list = NULL;

// Function to initialize the memory allocator
void initialize_allocator(size_t pool_size) {
    // Use mmap to allocate a block of memory
    memory_pool = mmap(NULL, pool_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (memory_pool == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    // Initialize the free list with a single block representing the entire memory pool
    free_list = (Block*)memory_pool;
    free_list->size = pool_size - sizeof(Block);
    free_list->next = NULL;
}
// Function to allocate memory using mmap
void* alloc_mem(size_t size) {
    // Ensure that the allocator has been initialized
    if (memory_pool == NULL) {
        fprintf(stderr, "Allocator not initialized. Call initialize_allocator first.\n");
        return NULL;
    }

    // Find a suitable block in the free list
    Block* current = free_list;
    Block* previous = NULL;

    while (current != NULL) {
        if (current->size >= size) {
            // Allocate from this block
            if (current->size > size + sizeof(Block)) {
                // Split the block if it's larger than needed
                Block* new_block = (Block*)((char*)current + sizeof(Block) + size);
                new_block->size = current->size - size - sizeof(Block);
                new_block->next = current->next;
                current->size = size;
                current->next = new_block;
            }

            // Remove the allocated block from the free list
            if (previous != NULL) {
                previous->next = current->next;
            } else {
                free_list = current->next;
            }

            return (void*)((char*)current + sizeof(Block));
        }

        previous = current;
        current = current->next;
    }

    // No suitable block found
    return NULL;
}

// Function to free memory
void free_mem(void* ptr) {
    if (ptr == NULL) {
        return; // Do nothing for NULL pointers
    }

    // Add the freed block to the free list
    Block* block = (Block*)((char*)ptr - sizeof(Block));
    block->next = free_list;
    free_list = block;
}

// Function to deallocate the memory pool
void deallocate_allocator() {
    if (memory_pool != NULL) {
        // Use munmap to deallocate the memory pool
        munmap(memory_pool, free_list->size + sizeof(Block));
        memory_pool = NULL;
        free_list = NULL;
    }
}
