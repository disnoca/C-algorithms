#include <stdlib.h>
#include <stdbool.h>
#include "array_list.h"
#include "../../../wrapper_functions.h"

/* Helper Functions */

static void resize(ArrayList* list) {
	int old_max_size = list->capacity;
	list->capacity *= 2;
	data_type* new_arr = (data_type*) Malloc(sizeof(data_type) * list->capacity);

	for(int i = 0; i < old_max_size; i++)
		new_arr[i] = list->arr[i];
	
	Free(list->arr);
	list->arr = new_arr;
}

/* Header Implementation */

ArrayList* al_create(int size) {
	ArrayList* list =  (ArrayList*) Malloc(sizeof(ArrayList));

	list->arr = (data_type*) Malloc(sizeof(data_type) * size);
	list->size = 0;
	list->capacity = size;

	return list;
}

void al_add(ArrayList* list, data_type data) {
	if(list->size >= list->capacity)
		resize(list);

	list->arr[list->size++] = data;
}

void al_add_at(ArrayList* list, data_type data, int pos) {
	if(pos == list->size) {
		al_add(list, data);
		return;
	}

	if(list->size == list->capacity)
		resize(list);

	for(int i = list->size; i > pos; i--)
		list->arr[i] = list->arr[i-1];

	list->arr[pos] = data;
	list->size++;
}

data_type al_remove_last(ArrayList* list) {
	return list->arr[--list->size];
}

data_type al_remove_at(ArrayList* list, int pos) {
	if(pos == list->size-1)
		return al_remove_last(list);

	data_type data = list->arr[pos];

	list->size--;
	for(int i = pos; i < list->size; i++)
		list->arr[i] = list->arr[i+1];

	return data;
}

bool al_remove(ArrayList* list, data_type data) {
	int pos = al_index_of(list, data);
	if(pos == -1)
		return false;

	al_remove_at(list, pos);
	return true;
}

data_type al_get(ArrayList* list, int pos) {
	return list->arr[pos];
}

data_type al_set(ArrayList* list, data_type data, int pos) {
	data_type old_data = list->arr[pos];
	list->arr[pos] = data;
	return old_data;
}

int al_index_of(ArrayList* list, data_type data) {
	for(int i = 0; i < list->size; i++)
		if(list->arr[i] == data)
			return i;

	return -1;
}

bool al_contains(ArrayList* list, data_type data) {
	return al_index_of(list, data) != -1;
}

void al_clear(ArrayList* list) {
	list->size = 0;
}

void al_destroy(ArrayList* list) {
	Free(list->arr);
	Free(list);
}