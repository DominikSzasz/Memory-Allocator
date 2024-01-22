#include <stdio.h>
#include "allocator.h"
// Define a block structure to represent allocated memory blocks

int main() {
    // Initialize the allocator with a memory pool size of 10000 bytes
    initialize_allocator(10000);

    // Allocate and use memory
    int* array = (int*)alloc_mem(5 * sizeof(int));
    if (array != NULL) {
        for (int i = 0; i < 5; ++i) {
            array[i] = i;
        }

        // Print all values in the array
        for (int i = 0; i < 5; ++i) {
            printf("%d ", array[i]);
        }

        // Free the memory after using it
        free_mem(array);
    }

    // Deallocate the memory pool
    deallocate_allocator();

    return 0;
}