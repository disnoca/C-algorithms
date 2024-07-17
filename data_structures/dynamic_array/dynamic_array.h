#pragma once

#include <sys/types.h>


/* ---------------- Structs ---------------- */

typedef struct {
	void** arr;
	size_t size, capacity;
} DynamicArray;


/* ---------------- Functions ---------------- */

void da_init(DynamicArray* da, size_t initial_capacity);
void da_free(DynamicArray* da, bool free_data);
DynamicArray* da_create(size_t initial_capacity);
void da_destroy(DynamicArray* da, bool free_data);

void da_add(DynamicArray* da, void* data);
void da_add_at(DynamicArray* da, void* data, size_t pos);

void* da_remove_last(DynamicArray* da);
void* da_remove_at(DynamicArray* da, size_t pos);
bool da_remove(DynamicArray* da, void* data);

void* da_get(DynamicArray* da, size_t pos);
void* da_set(DynamicArray* da, void* data, size_t pos);

ssize_t da_index_of(DynamicArray* da, void* data);
bool da_contains(DynamicArray* da, void* data);

void da_clear(DynamicArray* da, bool free_data);
