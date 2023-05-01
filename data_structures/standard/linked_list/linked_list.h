#ifndef LINKED_LIST
#define LINKED_LIST

#include <stdbool.h>

typedef int data_type;

/* ---------------- Structs ---------------- */

typedef struct LLNode LLNode;

struct LLNode {
    LLNode *prev, *next;
    data_type data;
};

typedef struct {
    LLNode *head, *tail;
    int size;
} LinkedList;

/* ---------------- Functions ---------------- */

LinkedList* ll_create();

void ll_add(LinkedList* list, data_type data);

void ll_add_first(LinkedList* list, data_type data);

void ll_add_at(LinkedList* list, data_type data, int pos);

data_type ll_remove_first(LinkedList* list);

data_type ll_remove_last(LinkedList* list);

data_type ll_remove_at(LinkedList* list, int pos);

bool ll_remove(LinkedList* list, data_type data);

data_type ll_get(LinkedList* list, int pos);

data_type ll_set(LinkedList* list, data_type data, int pos);

int ll_index_of(LinkedList* list, data_type data);

bool ll_contains(LinkedList* list, data_type data);

void ll_clear(LinkedList* list);

void ll_destroy(LinkedList* list);

#endif