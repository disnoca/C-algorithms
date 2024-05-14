#pragma once

#include <stdbool.h>

typedef int data_type;

/* ---------------- Structs ---------------- */

typedef struct DLLNode DLLNode;

struct DLLNode {
    DLLNode *prev, *next;
    data_type data;
};

typedef struct {
    DLLNode *head, *tail;
    int size;
} DoublyLinkedList;

/* ---------------- Functions ---------------- */

DoublyLinkedList* ll_create();

void ll_add(DoublyLinkedList* list, data_type data);

void ll_add_first(DoublyLinkedList* list, data_type data);

void ll_add_at(DoublyLinkedList* list, data_type data, int pos);

data_type ll_remove_first(DoublyLinkedList* list);

data_type ll_remove_last(DoublyLinkedList* list);

data_type ll_remove_at(DoublyLinkedList* list, int pos);

bool ll_remove(DoublyLinkedList* list, data_type data);

data_type ll_get(DoublyLinkedList* list, int pos);

data_type ll_set(DoublyLinkedList* list, data_type data, int pos);

int ll_index_of(DoublyLinkedList* list, data_type data);

bool ll_contains(DoublyLinkedList* list, data_type data);

void ll_clear(DoublyLinkedList* list);

void ll_destroy(DoublyLinkedList* list);
