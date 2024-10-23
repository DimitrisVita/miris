#include "myAllocation.h"

size_t allocatedBytes = 0;  // Global variable to keep track of the allocated bytes

// Allocate memory and keep track of the allocated bytes
void *myMalloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr != NULL)
        allocatedBytes += size;
    return ptr;
}

// Free memory and update the allocated bytes
void myFree(void *ptr, size_t size) {
    free(ptr);
    allocatedBytes -= size;
}