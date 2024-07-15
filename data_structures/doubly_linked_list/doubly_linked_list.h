#pragma once

#include <stdbool.h>
#include <sys/types.h>


/* ---------------- Structs ---------------- */

typedef struct DLLNode DLLNode;

struct DLLNode {
    DLLNode *prev, *next;
    void* data;
};

typedef struct {
    DLLNode *head, *tail;
    size_t size;
} DoublyLinkedList;


/* ---------------- Functions ---------------- */

DoublyLinkedList* ll_create();
void ll_destroy(DoublyLinkedList* list);
void ll_clear(DoublyLinkedList* list);

void ll_add(DoublyLinkedList* list, void* data);
void ll_add_first(DoublyLinkedList* list, void* data);
void ll_add_at(DoublyLinkedList* list, void* data, size_t pos);

void* ll_remove_first(DoublyLinkedList* list);
void* ll_remove_last(DoublyLinkedList* list);
void* ll_remove_at(DoublyLinkedList* list, size_t pos);
bool ll_remove(DoublyLinkedList* list, void* data);

void* ll_get(DoublyLinkedList* list, size_t pos);
void* ll_set(DoublyLinkedList* list, void* data, size_t pos);

ssize_t ll_index_of(DoublyLinkedList* list, void* data);
bool ll_contains(DoublyLinkedList* list, void* data);

