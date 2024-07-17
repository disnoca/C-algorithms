#pragma once

#include <stddef.h>
#include <stdbool.h>


/* ---------------- Structs ---------------- */

typedef struct {
	void** arr;
	size_t size, capacity;
	int(*compare)(void* arg1, void* arg2);
} Heap;


/* ---------------- Functions ---------------- */

void heap_init(Heap* heap, size_t initial_capacity, int(*compare)(void*, void*));
void heap_free(Heap* heap, bool free_data);
Heap* heap_create(size_t initial_capacity, int(*compare)(void*, void*));
void heap_destroy(Heap* heap, bool free_data);

void heap_insert(Heap* heap, void* data);
void* heap_extract(Heap* heap);
void* heap_peek(Heap* heap);
void heap_clear(Heap* heap, bool free_data);
