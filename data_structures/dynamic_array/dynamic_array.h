#pragma once

#include <sys/types.h>


/* ---------------- Structs ---------------- */

typedef struct {
	void** arr;
	size_t size, capacity;
} DynamicArray;

/* ---------------- Functions ---------------- */

DynamicArray* da_create(size_t initial_capacity);

void da_add(DynamicArray* da, void* data);

void da_add_at(DynamicArray* da, void* data, size_t pos);

void* da_remove_last(DynamicArray* da);

void* da_remove_at(DynamicArray* da, size_t pos);

bool da_remove(DynamicArray* da, void* data);

void* da_get(DynamicArray* da, size_t pos);

void* da_set(DynamicArray* da, void* data, size_t pos);

ssize_t da_index_of(DynamicArray* da, void* data);

bool da_contains(DynamicArray* da, void* data);

void da_clear(DynamicArray* da);

void da_destroy(DynamicArray* da);
