#pragma once

#include <stdlib.h>
#include <stdio.h>


extern unsigned long malloc_calls;
extern unsigned long free_calls;


#define exit_with_error(format, ...) _exit_with_error(__FILE__, __LINE__, format, ##__VA_ARGS__)
void _exit_with_error(const char* file, int line, const char* format, ...);


void* Malloc(size_t size);
void* Calloc(size_t nitems, size_t size);
void Free(void* ptr);
void* Realloc(void* ptr, size_t size);
void* Aligned_alloc(size_t alignment, size_t size);

FILE* Fopen(const char* path, const char* mode);
void Fclose(FILE* fp);
size_t Fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream);
size_t Fread(void* ptr, size_t size, size_t nmemb, FILE* stream);
void Fseek(FILE* stream, long offset, int whence);
long Ftell(FILE* stream);
