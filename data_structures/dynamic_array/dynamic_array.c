/**
 * Dynamic Array implementation
 * 
 * @author Samuel Pires
*/

#include <stdlib.h>
#include <stdbool.h>
#include "dynamic_array.h"
#include "../../wrapper_functions.h"

/* ---------------- Helper Functions ---------------- */

static void resize(DynamicArray* da)
{
	da->capacity *= 2;
	da->arr = Realloc(da->arr, sizeof(void*) * da->capacity);
}


/* ---------------- Header Implementation ---------------- */

void da_init(DynamicArray* da, size_t initial_capacity)
{
	da->arr = (void**) Malloc(sizeof(void*) * initial_capacity);
	da->size = 0;
	da->capacity = initial_capacity;

}

DynamicArray* da_create(size_t inital_capacity)
{
	DynamicArray* da = (DynamicArray*) Malloc(sizeof(DynamicArray));
	da_init(da, inital_capacity);
	return da;
}

void da_add(DynamicArray* da, void* data)
{
	if (da->size >= da->capacity)
		resize(da);

	da->arr[da->size++] = data;
}

void da_add_at(DynamicArray* da, void* data, size_t pos)
{
	if (pos == da->size) {
		da_add(da, data);
		return;
	}

	if (da->size == da->capacity)
		resize(da);

	for(size_t i = da->size; i > pos; i--)
		da->arr[i] = da->arr[i-1];

	da->arr[pos] = data;
	da->size++;
}

void* da_remove_last(DynamicArray* da)
{
	return da->arr[--da->size];
}

void* da_remove_at(DynamicArray* da, size_t pos)
{
	if (pos == da->size-1)
		return da_remove_last(da);

	void* data = da->arr[pos];

	da->size--;
	for(size_t i = pos; i < da->size; i++)
		da->arr[i] = da->arr[i+1];

	return data;
}

bool da_remove(DynamicArray* da, void* data)
{
	ssize_t pos = da_index_of(da, data);
	if (pos == -1)
		return false;

	da_remove_at(da, pos);
	return true;
}

void* da_get(DynamicArray* da, size_t pos)
{
	return da->arr[pos];
}

void* da_set(DynamicArray* da, void* data, size_t pos)
{
	void* old_data = da->arr[pos];
	da->arr[pos] = data;
	return old_data;
}

ssize_t da_index_of(DynamicArray* da, void* data)
{
	for(size_t i = 0; i < da->size; i++)
		if (da->arr[i] == data)
			return (ssize_t) i;

	return -1;
}

bool da_contains(DynamicArray* da, void* data)
{
	return da_index_of(da, data) != -1;
}

void da_clear(DynamicArray* da)
{
	da->size = 0;
}

void da_free(DynamicArray* da)
{
	Free(da->arr);
}

void da_destroy(DynamicArray* da)
{
	da_free(da);
	Free(da);
}
