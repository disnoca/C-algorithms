/**
 * Dynamic Array implementation
 * 
 * @author Samuel Pires
*/

#include <stdlib.h>
#include <stdbool.h>
#include "dynamic_array.h"
#include "../../../wrapper_functions.h"

/* ---------------- Helper Functions ---------------- */

static void resize(DynamicArray* da) {
	int old_max_size = da->capacity;
	da->capacity *= 2;
	data_type* new_arr = (data_type*) Malloc(sizeof(data_type) * da->capacity);

	for(int i = 0; i < old_max_size; i++)
		new_arr[i] = da->arr[i];
	
	Free(da->arr);
	da->arr = new_arr;
}

/* ---------------- Header Implementation ---------------- */

DynamicArray* da_create(int size) {
	DynamicArray* da =  (DynamicArray*) Malloc(sizeof(DynamicArray));

	da->arr = (data_type*) Malloc(sizeof(data_type) * size);
	da->size = 0;
	da->capacity = size;

	return da;
}

void da_add(DynamicArray* da, data_type data) {
	if(da->size >= da->capacity)
		resize(da);

	da->arr[da->size++] = data;
}

void da_add_at(DynamicArray* da, data_type data, int pos) {
	if(pos == da->size) {
		da_add(da, data);
		return;
	}

	if(da->size == da->capacity)
		resize(da);

	for(int i = da->size; i > pos; i--)
		da->arr[i] = da->arr[i-1];

	da->arr[pos] = data;
	da->size++;
}

data_type da_remove_last(DynamicArray* da) {
	return da->arr[--da->size];
}

data_type da_remove_at(DynamicArray* da, int pos) {
	if(pos == da->size-1)
		return da_remove_last(da);

	data_type data = da->arr[pos];

	da->size--;
	for(int i = pos; i < da->size; i++)
		da->arr[i] = da->arr[i+1];

	return data;
}

bool da_remove(DynamicArray* da, data_type data) {
	int pos = da_index_of(da, data);
	if(pos == -1)
		return false;

	da_remove_at(da, pos);
	return true;
}

data_type da_get(DynamicArray* da, int pos) {
	return da->arr[pos];
}

data_type da_set(DynamicArray* da, data_type data, int pos) {
	data_type old_data = da->arr[pos];
	da->arr[pos] = data;
	return old_data;
}

int da_index_of(DynamicArray* da, data_type data) {
	for(int i = 0; i < da->size; i++)
		if(da->arr[i] == data)
			return i;

	return -1;
}

bool da_contains(DynamicArray* da, data_type data) {
	return da_index_of(da, data) != -1;
}

void da_clear(DynamicArray* da) {
	da->size = 0;
}

void da_destroy(DynamicArray* da) {
	Free(da->arr);
	Free(da);
}
