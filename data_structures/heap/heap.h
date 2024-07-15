#pragma once

#include <stddef.h>


/* ---------------- Structs ---------------- */

typedef struct {
	void** arr;
	size_t size, capacity;
	int(*compare)(void* arg1, void* arg2);
} Heap;

/* ---------------- Functions ---------------- */

Heap* heap_create(size_t initial_capacity, int(*compare)(void*, void*));
void heap_destroy(Heap* heap);

void heap_insert(Heap* heap, void* data);
void* heap_extract(Heap* heap);

