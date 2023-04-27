#include <stdio.h>
#include "wrapper_functions.h"

int malloc_calls = 0;
int free_calls = 0;

void exit_with_error(char* msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

void* Malloc(size_t size) {
    malloc_calls++;
    void* ptr;
    if((ptr = malloc(size)) == NULL) 
        exit_with_error("Malloc error");
    return ptr;
}

void* Calloc(size_t nitems, size_t size) {
    malloc_calls++;
    void* ptr;
    if((ptr = calloc(nitems, size)) == NULL) 
        exit_with_error("Calloc error");
    return ptr;
}

void Free(void* ptr) {
    free_calls++;
    free(ptr);
    ptr = NULL;
}