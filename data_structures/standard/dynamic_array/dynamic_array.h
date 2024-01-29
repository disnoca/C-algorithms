#ifndef DYNAMIC_ARRAY
#define DYNAMIC_ARRAY

typedef int data_type;

/* ---------------- Structs ---------------- */

typedef struct {
	data_type* arr;
	int size, capacity;
} DynamicArray;

/* ---------------- Functions ---------------- */

DynamicArray* da_create();

void da_add(DynamicArray* da, data_type data);

void da_add_at(DynamicArray* da, data_type data, int pos);

data_type da_remove_last(DynamicArray* da);

data_type da_remove_at(DynamicArray* da, int pos);

bool da_remove(DynamicArray* da, data_type data);

data_type da_get(DynamicArray* da, int pos);

data_type da_set(DynamicArray* da, data_type data, int pos);

int da_index_of(DynamicArray* da, data_type data);

bool da_contains(DynamicArray* da, data_type data);

void da_clear(DynamicArray* da);

void da_destroy(DynamicArray* da);

#endif
