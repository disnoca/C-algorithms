/**
 * Linked List implementation.
 * 
 * @author Samuel Pires
*/

#include <stdlib.h>
#include "doubly_linked_list.h"
#include "../../wrapper_functions.h"

/* ---------------- Helper Functions ---------------- */

static DLLNode* create_node(void* data)
{
	DLLNode* node = (DLLNode*) Calloc(1, sizeof(DLLNode));
	node->data = data;
	return node;
}

static DLLNode* get_node(DoublyLinkedList* list, size_t pos)
{
	DLLNode* curr_node;

	if (pos <= list->size/2) {
		curr_node = list->head;

		for(size_t i = 0; i < pos; i++)
			curr_node = curr_node->next;
		
		return curr_node;
	}
	else {
		curr_node = list->tail;

		for(size_t i = list->size-1; i > pos; i--)
			curr_node = curr_node->prev;
		
		return curr_node;
	}
}

static void link_nodes(DLLNode* node1, DLLNode* node2)
{
	node1->next = node2;
	node2->prev = node1;
}


/* ---------------- Header Implementation ---------------- */

void ll_init(DoublyLinkedList* list)
{
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}

DoublyLinkedList* ll_create(void)
{
	DoublyLinkedList* list = (DoublyLinkedList*) Malloc(sizeof(DoublyLinkedList));
	ll_init(list);
	return list;
}

void ll_add(DoublyLinkedList* list, void* data)
{
	DLLNode* new_node = create_node(data);

	if (list->size == 0) {
		list->head = new_node;
		list->tail = new_node;
	}
	else {
		link_nodes(list->tail, new_node);
		list->tail = new_node;
	}

	list->size++;
}

void ll_add_first(DoublyLinkedList* list, void* data)
{
	if (list->size == 0) {
		ll_add(list, data);
		return;
	}

	DLLNode* new_node = create_node(data);

	link_nodes(new_node, list->head);
	list->head = new_node;
	list->size++;
}

void ll_add_at(DoublyLinkedList* list, void* data, size_t pos)
{
	if (list->size == 0 || pos == list->size) {
		ll_add(list, data);
		return;
	}

	if (pos == 0) {
		ll_add_first(list, data);
		return;
	}

	DLLNode* new_node = create_node(data);
	DLLNode* next_node = get_node(list, pos);
	DLLNode* prev_node = next_node->prev;

	link_nodes(prev_node, new_node);
	link_nodes(new_node, next_node);
	list->size++;
}

void* ll_remove_first(DoublyLinkedList* list)
{
	DLLNode* node = list->head;
	void* data = node->data;

	if (list->size > 1) {
		list->head = node->next;
		list->head->prev = NULL;
	}

	list->size--;
	Free(node);
	return data;
}

void* ll_remove_last(DoublyLinkedList* list)
{
	DLLNode* node = list->tail;
	void* data = node->data;

	if (list->size > 1) {
		list->tail = node->prev;
		list->tail->next = NULL;
	}

	list->size--;
	Free(node);
	return data;
}

void* ll_remove_at(DoublyLinkedList* list, size_t pos)
{
	if (pos == 0) 			
		return ll_remove_first(list);

	if (pos == list->size-1)	
		return ll_remove_last(list);

	DLLNode* node = get_node(list, pos);
	void* data = node->data;

	link_nodes(node->prev, node->next);
	Free(node);
	list->size--;

	return data;
}

bool ll_remove(DoublyLinkedList* list, void* data)
{
	DLLNode* curr_node = list->head;

	for(size_t i = 0; i < list->size; i++) {
		if (curr_node->data == data) {
			if (i == 0)
				ll_remove_first(list);
			else if (i == list->size-1)
				ll_remove_last(list);
			else {
				link_nodes(curr_node->prev, curr_node->next);
				Free(curr_node);
				list->size--;
			}

			return true;
		}
		curr_node = curr_node->next;
	}

	return false;
}

void* ll_get(DoublyLinkedList* list, size_t pos)
{
	return get_node(list, pos)->data;
}

void* ll_set(DoublyLinkedList* list, void* data, size_t pos)
{
	DLLNode* node = get_node(list, pos);
	void* old_data = node->data;

	node->data = data;

	return old_data;
}

ssize_t ll_index_of(DoublyLinkedList* list, void* data)
{
	DLLNode* curr_node = list->head;

	for(size_t i = 0; i < list->size; i++) {
		if (curr_node->data == data)
			return (ssize_t) i;

		curr_node = curr_node->next;
	}

	return -1;
}

bool ll_contains(DoublyLinkedList* list, void* data)
{
	return ll_index_of(list, data) != -1;
}

void ll_clear(DoublyLinkedList* list)
{
	DLLNode *prev_node, *curr_node = list->head;

	for(size_t i = 0; i < list->size; i++) {
		prev_node = curr_node;
		curr_node = curr_node->next;
		Free(prev_node);
	}

	list->size = 0;
}

void ll_free(DoublyLinkedList* list)
{
	ll_clear(list);
}

void ll_destroy(DoublyLinkedList* list)
{
	ll_free(list);
	Free(list);
}
