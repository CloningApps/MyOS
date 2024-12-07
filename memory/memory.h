#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

void init_memory();
void* allocate(size_t size);
void free(void* ptr);

#endif

