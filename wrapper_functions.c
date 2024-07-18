#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include "wrapper_functions.h"


unsigned long malloc_calls = 0;
unsigned long free_calls = 0;


void exit_with_error(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	exit(1);
}

void* Malloc(size_t size)
{
	void* ptr = malloc(size);
	if(ptr == NULL) 
		exit_with_error("Malloc error: %s", strerror(errno));
	malloc_calls++;
	return ptr;
}

void* Calloc(size_t nitems, size_t size)
{
	void* ptr = calloc(nitems, size);
	if(ptr == NULL) 
		exit_with_error("Calloc error: %s", strerror(errno));
	malloc_calls++;
	return ptr;
}

void Free(void* ptr)
{
	free_calls++;
	free(ptr);
}

void* Realloc(void* ptr, size_t size)
{
	void* new_ptr = realloc(ptr, size);
	if(new_ptr == NULL)
		exit_with_error("Realloc error: %s", strerror(errno));
	if (!ptr)
		malloc_calls++;
	return new_ptr;
}
