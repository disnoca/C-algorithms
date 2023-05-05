#ifndef ARRAY_LIST
#define ARRAY_LIST

typedef int data_type;

/* ---------------- Structs ---------------- */

typedef struct {
	data_type* arr;
	int size, capacity;
} ArrayList;

/* ---------------- Functions ---------------- */

ArrayList* al_create();

void al_add(ArrayList* list, data_type data);

void al_add_at(ArrayList* list, data_type data, int pos);

data_type al_remove_last(ArrayList* list);

data_type al_remove_at(ArrayList* list, int pos);

bool al_remove(ArrayList* list, data_type data);

data_type al_get(ArrayList* list, int pos);

data_type al_set(ArrayList* list, data_type data, int pos);

int al_index_of(ArrayList* list, data_type data);

bool al_contains(ArrayList* list, data_type data);

void al_clear(ArrayList* list);

void al_destroy(ArrayList* list);

#endif