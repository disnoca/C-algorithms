#pragma once


typedef int data_type;

/* ---------------- Structs ---------------- */

typedef struct {
	data_type* arr;
	int size, capacity;
} Heap;

/* ---------------- Functions ---------------- */

Heap* heap_create(int initial_capacity);

void heap_insert(Heap* heap, data_type data);

data_type heap_extract(Heap* heap);

void heap_destroy(Heap* heap);
