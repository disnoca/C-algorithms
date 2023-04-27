#ifndef WRAPPER_FUNCTIONS
#define WRAPPER_FUNCTIONS

#include <stdlib.h>

extern int malloc_calls;
extern int free_calls;

void exit_with_error(char* msg);
void* Malloc(size_t size);
void* Calloc(size_t nitems, size_t size);
void Free(void* ptr);

#endif