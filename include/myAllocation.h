#pragma once

#include <stdio.h>
#include <stdlib.h>

// Global variable to keep track of the allocated bytes
extern size_t allocatedBytes;

// Allocate memory and keep track of the allocated bytes
void *myMalloc(size_t size);

// Free memory and update the allocated bytes
void myFree(void *ptr, size_t size);