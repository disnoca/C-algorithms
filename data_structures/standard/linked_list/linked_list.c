#include <stdlib.h>
#include "linked_list.h"
#include "../../../wrapper_functions.h"

/* ---------------- Helper Functions ---------------- */

static LLNode* create_node(data_type data) {
	LLNode* node = (LLNode*) Calloc(1, sizeof(LLNode));
	node->data = data;
	return node;
}

static LLNode* get_node(LinkedList* list, int pos) {
	LLNode* curr_node;

	if(pos <= list->size/2) {
		curr_node = list->head;

		for(int i = 0; i < pos; i++)
			curr_node = curr_node->next;
		
		return curr_node;
	}
	else {
		curr_node = list->tail;

		for(int i = list->size-1; i > pos; i--)
			curr_node = curr_node->prev;
		
		return curr_node;
	}
}

static void link_nodes(LLNode* node1, LLNode* node2) {
	node1->next = node2;
	node2->prev = node1;
}

/* ---------------- Header Implementation ---------------- */

LinkedList* ll_create() {
	return (LinkedList*) Calloc(1, sizeof(LinkedList));
}

void ll_add(LinkedList* list, data_type data) {
	LLNode* new_node = create_node(data);

	if(list->size == 0) {
		list->head = new_node;
		list->tail = new_node;
	}
	else {
		link_nodes(list->tail, new_node);
		list->tail = new_node;
	}

	list->size++;
}

void ll_add_first(LinkedList* list, data_type data) {
	if(list->size == 0) {
		ll_add(list, data);
		return;
	}

	LLNode* new_node = create_node(data);

	link_nodes(new_node, list->head);
	list->head = new_node;
	list->size++;
}

void ll_add_at(LinkedList* list, data_type data, int pos) {
	if(list->size == 0 || pos == list->size) {
		ll_add(list, data);
		return;
	}

	if(pos == 0) {
		ll_add_first(list, data);
		return;
	}

	LLNode* new_node = create_node(data);
	LLNode* next_node = get_node(list, pos);
	LLNode* prev_node = next_node->prev;

	link_nodes(prev_node, new_node);
	link_nodes(new_node, next_node);
	list->size++;
}

data_type ll_remove_first(LinkedList* list) {
	LLNode* node = list->head;
	data_type data = node->data;

	if(list->size > 1) {
		list->head = node->next;
		list->head->prev = NULL;
	}

	list->size--;
	Free(node);
	return data;
}

data_type ll_remove_last(LinkedList* list) {
	LLNode* node = list->tail;
	data_type data = node->data;

	if(list->size > 1) {
		list->tail = node->prev;
		list->tail->next = NULL;
	}

	list->size--;
	Free(node);
	return data;
}

data_type ll_remove_at(LinkedList* list, int pos) {
	if(pos == 0) 			
		return ll_remove_first(list);

	if(pos == list->size-1)	
		return ll_remove_last(list);

	LLNode* node = get_node(list, pos);
	data_type data = node->data;

	link_nodes(node->prev, node->next);
	Free(node);
	list->size--;

	return data;
}

bool ll_remove(LinkedList* list, data_type data) {
	LLNode* curr_node = list->head;

	for(int i = 0; i < list->size; i++) {
		if(curr_node->data == data) {
			if(i == 0)
				ll_remove_first(list);
			else if(i == list->size-1)
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

data_type ll_get(LinkedList* list, int pos) {
	return get_node(list, pos)->data;
}

data_type ll_set(LinkedList* list, data_type data, int pos) {
	LLNode* node = get_node(list, pos);
	data_type old_data = node->data;

	node->data = data;

	return old_data;
}

int ll_index_of(LinkedList* list, data_type data) {
	LLNode* curr_node = list->head;

	for(int i = 0; i < list->size; i++) {
		if(curr_node->data == data)
			return i;

		curr_node = curr_node->next;
	}

	return -1;
}

bool ll_contains(LinkedList* list, data_type data) {
	return ll_index_of(list, data) != -1;
}

void ll_clear(LinkedList* list) {
	LLNode *prev_node, *curr_node = list->head;

	for(int i = 0; i < list->size; i++) {
		prev_node = curr_node;
		curr_node = curr_node->next;
		Free(prev_node);
	}

	list->size = 0;
}

void ll_destroy(LinkedList* list) {
	ll_clear(list);
	Free(list);
}