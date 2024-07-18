#pragma once

#include <stdlib.h>


extern unsigned long malloc_calls;
extern unsigned long free_calls;


void exit_with_error(const char* format, ...);

void* Malloc(size_t size);
void* Calloc(size_t nitems, size_t size);
void Free(void* ptr);
void* Realloc(void* ptr, size_t size);
