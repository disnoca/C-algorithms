#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include "wrapper_functions.h"


unsigned long malloc_calls = 0;
unsigned long free_calls = 0;


void _exit_with_error(const char* file, int line, const char* format, ...)
{
    va_list args;
    va_start(args, format);
	if (file)
    	fprintf(stderr, "%s:%d: error: ", file, line);
    vfprintf(stderr, format, args);
	if (format[strlen(format) - 1] != '\n')
   		fprintf(stderr, "\n");
    va_end(args);
    exit(1);
}

static void exit_with_error_simple(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    _exit_with_error(NULL, 0, format, args);
}


void* Malloc(size_t size)
{
	void* ptr = malloc(size);
	if(ptr == NULL) 
		exit_with_error_simple("malloc error %d: %s", errno, strerror(errno));
	malloc_calls++;
	return ptr;
}

void* Calloc(size_t size, size_t nmemb)
{
	void* ptr = calloc(nmemb, size);
	if(ptr == NULL) 
		exit_with_error_simple("calloc error %d: %s", errno, strerror(errno));
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
		exit_with_error_simple("realloc error %d: %s", errno, strerror(errno));
	if (!ptr)
		malloc_calls++;
	return new_ptr;
}

void* Aligned_alloc(size_t alignment, size_t size)
{
	void* ptr = aligned_alloc(alignment, size);
	if(ptr == NULL)
		exit_with_error_simple("aligned_alloc error %d: %s", errno, strerror(errno));
	malloc_calls++;
	return ptr;
}


FILE* Fopen(const char* path, const char* mode)
{
	FILE* fp = fopen(path, mode);
	if(fp == NULL)
		exit_with_error_simple("fopen error %d: %s", errno, strerror(errno));
	return fp;
}

void Fclose(FILE* fp)
{
	if (fclose(fp) == EOF)
		exit_with_error_simple("fclose error %d: %s", errno, strerror(errno));
}

size_t Fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream)
{
	size_t written = fwrite(ptr, size, nmemb, stream);
	if(written != nmemb)
		exit_with_error_simple("fwrite error %d: %s", errno, strerror(errno));
	return written;
}

size_t Fread(void* ptr, size_t size, size_t nmemb, FILE* stream)
{
	size_t read = fread(ptr, size, nmemb, stream);
	if (ferror(stream))
		exit_with_error_simple("fread error %d: %s", errno, strerror(errno));
	return read;
}

void Fseek(FILE* stream, long offset, int whence)
{
	if (fseek(stream, offset, whence) < 0)
		exit_with_error_simple("fseek error %d: %s", errno, strerror(errno));
}

long Ftell(FILE* stream)
{
	long offset = ftell(stream);
	if (offset < 0)
		exit_with_error_simple("ftell error %d: %s", errno, strerror(errno));
	return offset;
}
