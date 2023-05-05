/**
 * Linked List implementation.
 * 
 * @author Samuel Pires
*/

#include <stdio.h>
#include <string.h>
#include "linked_list.h"
#include "../../../wrapper_functions.h"

static const int NO_FREE = 0;
static const int FREE = 1;
static const int BY_VALUE = 2;
static const int BY_REFERENCE = 3;


LinkedList* ll_list_create(size_t data_size) {
    LinkedList* list = (LinkedList*) Calloc(1, sizeof(LinkedList));
    list->data_size = data_size;
    return list;
}

static void list_clear_and_free_if(LinkedList* list, int to_free) {
    LLNode* curr_node = list->head;
    LLNode* prev_node;

    while(curr_node != NULL) {
        prev_node = curr_node;
        curr_node = curr_node->next;
        if(to_free)
            Free(prev_node->data);
        Free(prev_node);
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void ll_list_clear(LinkedList* list) {
    list_clear_and_free_if(list, NO_FREE);
}

void ll_list_clear_and_free(LinkedList* list) {
    list_clear_and_free_if(list, FREE);
}

static void list_destroy_and_free_if(LinkedList* list, int to_free) {
    list_clear_and_free_if(list, to_free);
    Free(list);
}

void ll_list_destroy(LinkedList* list) {
    list_destroy_and_free_if(list, NO_FREE);
}

void ll_list_destroy_and_free(LinkedList* list) {
    list_destroy_and_free_if(list, FREE);
}

int ll_size(LinkedList* list) {
    return list->size;
}

int ll_is_empty(LinkedList* list) {
    return list->size == 0;
}

static void link_nodes(LLNode* node1, LLNode* node2) {
    node1->next = node2;
    node2->prev = node1;
}

static LLNode* create_node_by(void* data, int data_size, int data_type) {
    void* node_data = NULL;
    if(data_type == BY_VALUE) {
        node_data = Malloc(data_size);
        memcpy(node_data, data, data_size);
    }
    else if (data_type == BY_REFERENCE)
        node_data = data;
    
    LLNode node = {NULL, NULL, node_data};
    LLNode* nodep = (LLNode*) Malloc(sizeof(LLNode));
    *nodep = node;
    return nodep;
}

static LLNode* get_node(LinkedList* list, int index) {
    int list_size = list->size;
    if(index >= list->size) return NULL;

    LLNode* curr_node;
    if(index < list_size/2) {
        curr_node = list->head;
        for(int i = 0; i < index; i++)
            curr_node = curr_node->next;
    }
    else {
        curr_node = list->tail;
        index = list_size-index-1;
        for(int i = 0; i < index; i++)
            curr_node = curr_node->prev;
    }

    return curr_node;
}

static void add_node(LinkedList* list, LLNode* node) {
    if(ll_is_empty(list))
        list->head = node;
    else
        link_nodes(list->tail, node);

    list->tail = node;
    list->size++;
}

// crashes the program if trying to add to an out of bounds position
static void add_node_at(LinkedList* list, LLNode* node, int index) {
    int list_size = list->size;
    
    if(index == list_size) {
        add_node(list, node);
        return;
    }

    if(index == 0) {
        link_nodes(node, list->head);
        list->head = node;
    }
    else {
        LLNode* target_node = get_node(list, index);
        link_nodes(target_node->prev, node);
        link_nodes(node, target_node);
    }

    list->size++;
}

// crashes the program if trying to add to an out of bounds position
static LLNode* remove_node(LinkedList* list, int index) {
    LLNode* node = get_node(list, index);

    if(index == 0) {
        list->head = node->next;
        list->head->prev = NULL;
    }
    else if(index == list->size-1) {
        list->tail = node->prev;
        list->tail->next = NULL;
    }
    else
        link_nodes(node->prev, node->next);

    list->size--;
    return node;
}

// crashes the program if trying to add to an out of bounds position
static LLNode* set_node(LinkedList* list, LLNode* node, int index) {
    int list_size = list->size;
    if(index == list_size) {
        add_node(list, node);
        return NULL;
    }

    LLNode* target_node;

    if(index == 0) {
        target_node = list->head;
        link_nodes(node, target_node->next);
        list->head = node;
    }
    else if(index == list_size-1) {
        target_node = list->tail;
        link_nodes(target_node->prev, node);
        list->tail = node;
    }
    else {
        target_node = get_node(list, index);
        link_nodes(target_node->prev, node);
        link_nodes(node, target_node->next);
    }

    return target_node;
}

static void add_by(LinkedList* list, void* data, int data_type) {
    LLNode* node = create_node_by(data, list->data_size, data_type);
    node->prev = list->tail;
    add_node(list, node);
}

void ll_add_by_value(LinkedList* list, void* data) {
    add_by(list, data, BY_VALUE);
}

void ll_add_by_reference(LinkedList* list, void* data) {
    add_by(list, data, BY_REFERENCE);
}

static void add_by_at(LinkedList* list, void* data, int index, int data_type) {
    LLNode* node = create_node_by(data, list->data_size, data_type);
    add_node_at(list, node, index);
}

void ll_add_by_value_at(LinkedList* list, void* data, int index) {
    add_by_at(list, data, index, BY_VALUE);
}

void ll_add_by_reference_at(LinkedList* list, void* data, int index) {
    add_by_at(list, data, index, BY_REFERENCE);
}

static void* set_by_and_free_if(LinkedList* list, void* data, int index, int data_type, int to_free) {
    LLNode* new_node = create_node_by(data, list->data_size, data_type);
    LLNode* old_node = set_node(list, new_node, index);

    // is only true if index was the list's size, meaning the set acted as an append
    if(old_node == NULL) return NULL;    

    void* old_data = old_node->data;
    Free(old_node);

    if(to_free) {
        Free(old_data);
        return NULL;
    }
    else
        return old_data;
}

void* ll_set_by_value(LinkedList* list, void* data, int index) {
    return set_by_and_free_if(list, data, index, BY_VALUE, NO_FREE);
}

void* ll_set_by_reference(LinkedList* list, void* data, int index) {
    return set_by_and_free_if(list, data, index, BY_REFERENCE, NO_FREE);
}

void ll_set_by_value_and_free(LinkedList* list, void* data, int index) {
    set_by_and_free_if(list, data, index, BY_VALUE, FREE);
}

void ll_set_by_reference_and_free(LinkedList* list, void* data, int index) {
    set_by_and_free_if(list, data, index, BY_REFERENCE, FREE);
}

void* ll_get(LinkedList* list, int index) {
    if(index >= list->size) return NULL;
    return get_node(list, index)->data;
}

static int index_of(LinkedList* list, void* data, int data_type) {
    int list_size = list->size;
    LLNode* node = list->head;

    if(data_type == BY_VALUE) {
        int data_size = list->data_size;
        for(int i = 0; i < list_size; i++) {
            if(!memcmp(node->data, data, data_size))
                return i;
            node = node->next;
        }
    }
    else if (data_type == BY_REFERENCE)
        for(int i = 0; i < list_size; i++) {
            if(node->data == data)
                return i;
            node = node->next;
        }

    return -1;
}

int ll_index_of_value(LinkedList* list, void* data) {
    return index_of(list, data, BY_VALUE);
}

int ll_index_of_reference(LinkedList* list, void* data) {
    return index_of(list, data, BY_REFERENCE);
}

static int last_index_of(LinkedList* list, void* data, int data_type) {
    int list_size = list->size;
    LLNode* node = list->tail;

    if(data_type == BY_VALUE) {
        int data_size = list->data_size;
        for(int i = 0; i < list_size; i++) {
            if(!memcmp(node->data, data, data_size))
                return list_size-i-1;
            node = node->prev;
        }
    }
    else if (data_type == BY_REFERENCE)
        for(int i = 0; i < list_size; i++) {
            if(node->data == data)
                return list_size-i-1;
            node = node->prev;
        }

    return -1;
}

int ll_last_index_of_value(LinkedList* list, void* data) {
    return last_index_of(list, data, BY_VALUE);
}

int ll_last_index_of_reference(LinkedList* list, void* data) {
    return last_index_of(list, data, BY_REFERENCE);
}

static void* remove_and_free_if(LinkedList* list, int index, int to_free) {
    LLNode* node = remove_node(list, index);
    void* data = node->data;
    Free(node);

    if(to_free) {
        Free(data);
        return NULL;
    }
    else
        return data;
}

void* ll_remove(LinkedList* list, int index) {
    return remove_and_free_if(list, index, NO_FREE);
}

void ll_remove_and_free(LinkedList* list, int index) {
    remove_and_free_if(list, index, FREE);
}

static void remove_by_and_free_if(LinkedList* list, void* data, int data_type, int to_free) {
    int index = index_of(list, data, data_type);
    if(index < 0) return;
    remove_and_free_if(list, index, to_free);
}

void ll_remove_by_value(LinkedList* list, void* data) {
    remove_by_and_free_if(list, data, BY_VALUE, NO_FREE);
}

void ll_remove_by_value_and_free(LinkedList* list, void* data) {
    remove_by_and_free_if(list, data, BY_VALUE, FREE);
}

void ll_remove_by_reference(LinkedList* list, void* data) {
    remove_by_and_free_if(list, data, BY_REFERENCE, NO_FREE);
}

void ll_remove_by_reference_and_free(LinkedList* list, void* data) {
    remove_by_and_free_if(list, data, BY_REFERENCE, FREE);
}

static int contains(LinkedList* list, void* data, int data_type) {
    return index_of(list, data, data_type) >= 0;
}

int ll_contains_value(LinkedList* list, void* data) {
    return index_of(list, data, BY_VALUE) >= 0;
}

int ll_contains_reference(LinkedList* list, void* data) {
    return index_of(list, data, BY_REFERENCE) >= 0;
}

static int contains_all(LinkedList* list, LinkedList* comp_list, int data_type) {
    int comp_list_size = comp_list->size;

    if(comp_list_size == 0) return 1;
    if(list->size == 0) return 0;

    LLNode* comp_node = comp_list->head;
    for(int i = 0; i < comp_list_size; i++) {
        if(!contains(list, comp_node->data, data_type))
            return 0;
        comp_node = comp_node->next;
    }
    return 1;
}

int ll_contains_all_values(LinkedList* list, LinkedList* comp_list) {
    return contains_all(list, comp_list, BY_VALUE);
}

int ll_contains_all_references(LinkedList* list, LinkedList* comp_list) {
    return contains_all(list, comp_list, BY_REFERENCE);
}

static void add_all_by(LinkedList* dest_list, LinkedList* source_list, int data_type) {
    if(source_list->size == 0) return;

    int data_size = source_list->data_size;

    // first iteration must be done outside for this algorithm to work in case dest_list is empty
    LLNode* start_node = create_node_by(source_list->head->data, data_size, data_type);
    if(ll_is_empty(dest_list))
        dest_list->head = start_node;
    else
        link_nodes(dest_list->tail, start_node);
    dest_list->tail = start_node;

    int source_list_size = source_list->size;
    LLNode* curr_node = source_list->head->next;
    for(int i = 1; i < source_list_size; i++) {
        LLNode* node = create_node_by(curr_node->data, data_size, data_type);
        link_nodes(dest_list->tail, node);
        dest_list->tail = node;
        curr_node = curr_node->next;
    }

    dest_list->size += source_list->size;
}

void ll_add_all_by_value(LinkedList* dest_list, LinkedList* source_list) {
    add_all_by(dest_list, source_list, BY_VALUE);
}

void ll_add_all_by_reference(LinkedList* dest_list, LinkedList* source_list) {
    add_all_by(dest_list, source_list, BY_REFERENCE);
}

static void add_all_by_at(LinkedList* dest_list, LinkedList* source_list, int index, int data_type) {
    int source_list_size = source_list->size;
    if(ll_is_empty(source_list)) return;
    if(ll_is_empty(dest_list) || index == source_list_size) {
        add_all_by(dest_list, source_list, data_type);
        return;
    }

    // first iteration must be done outside for this algorithm to work in case index is 0
    int data_size = source_list->data_size;
    LLNode* next_node = get_node(dest_list, index);
    LLNode* prev_node = next_node->prev;
    LLNode* first_node = create_node_by(source_list->head->data, data_size, data_type);
    if(index == 0) {
        link_nodes(first_node, dest_list->head);
        dest_list->head = first_node;
    }
    else
        link_nodes(prev_node, first_node);
    prev_node = first_node;

    LLNode* curr_node = source_list->head->next;
    for(int i = 1; i < source_list_size; i++) {
        LLNode* node = create_node_by(curr_node->data, data_size, data_type);
        link_nodes(prev_node, node);
        prev_node = node;
        curr_node = curr_node->next;
    }
    link_nodes(prev_node, next_node);
    dest_list->size += source_list->size;
}

void ll_add_all_by_value_at(LinkedList* dest_list, LinkedList* source_list, int index) {
    add_all_by_at(dest_list, source_list, index, BY_VALUE);
}

void ll_add_all_by_reference_at(LinkedList* dest_list, LinkedList* source_list, int index) {
    add_all_by_at(dest_list, source_list, index, BY_REFERENCE);
}

static int remove_all_by_and_free_if(LinkedList* dest_list, LinkedList* source_list, int data_type, int to_free) {
    int original_dest_list_size = dest_list->size;
    int source_list_size = source_list->size;

    if(original_dest_list_size == 0 || source_list_size == 0) return 0;

    LLNode* node = source_list->head;
    for(int i = 0; i < source_list_size; i++) {
        remove_by_and_free_if(dest_list, node->data, data_type, to_free);
        node = node->next;
    }

    return dest_list->size < original_dest_list_size;
}

int ll_remove_all_by_value(LinkedList* dest_list, LinkedList* source_list) {
    return remove_all_by_and_free_if(dest_list, source_list, BY_VALUE, NO_FREE);
}

int ll_remove_all_by_reference(LinkedList* dest_list, LinkedList* source_list) {
    return remove_all_by_and_free_if(dest_list, source_list, BY_REFERENCE, NO_FREE);
}

int ll_remove_all_by_value_and_free(LinkedList* dest_list, LinkedList* source_list) {
    return remove_all_by_and_free_if(dest_list, source_list, BY_VALUE, FREE);
}

static int retain_all_by_and_free_if(LinkedList* dest_list, LinkedList* source_list, int data_type, int to_free) {
    int original_dest_list_size = dest_list->size;

    if(original_dest_list_size == 0) return 0;
    if(source_list->size == 0) {
        list_clear_and_free_if(dest_list, to_free);
        return 1;
    }

    LLNode* node = dest_list->tail;
    for(int i = original_dest_list_size-1; i >= 0; i--) {
        if(!contains(source_list, node->data, data_type))
            remove_and_free_if(dest_list, i, to_free);
        node = node->prev;
    }

    return dest_list->size < original_dest_list_size;
}

int ll_retain_all_by_value(LinkedList* dest_list, LinkedList* source_list) {
    return retain_all_by_and_free_if(dest_list, source_list, BY_VALUE, NO_FREE);
}

int ll_retain_all_by_reference(LinkedList* dest_list, LinkedList* source_list) {
    return retain_all_by_and_free_if(dest_list, source_list, BY_REFERENCE, NO_FREE);
}

int ll_retain_all_by_value_and_free(LinkedList* dest_list, LinkedList* source_list) {
    return retain_all_by_and_free_if(dest_list, source_list, BY_VALUE, FREE);
}

int ll_retain_all_by_reference_and_free(LinkedList* dest_list, LinkedList* source_list) {
    return retain_all_by_and_free_if(dest_list, source_list, BY_REFERENCE, FREE);
}

static LinkedList* sublist(LinkedList* source_list, int from_index, int to_index, int data_type) {
    int source_list_size = source_list->size;
    if(from_index < 0 || to_index > source_list_size || from_index > to_index) return NULL;

    LinkedList* sublist = ll_list_create(source_list->data_size);

    LLNode* curr_node = get_node(source_list, from_index);
    for(int i = from_index; i < to_index; i++) {
        add_by(sublist, curr_node->data, data_type);
        curr_node = curr_node->next;
    }

    return sublist;
}

LinkedList* ll_sublist_by_value(LinkedList* source_list, int from_index, int to_index) {
    return sublist(source_list, from_index, to_index, BY_VALUE);
}

LinkedList* ll_sublist_by_reference(LinkedList* source_list, int from_index, int to_index) {
    return sublist(source_list, from_index, to_index, BY_REFERENCE);
}

void* ll_to_array(LinkedList* list) {
    int list_size = list->size;
    int data_size = list->data_size;
    if(list_size == 0) return NULL;

    void* array_head = Malloc(data_size * list_size);

    LLNode* curr_node = list->head;
    for(int i = 0; i < list_size; i++) {
        memcpy(array_head + i*data_size, curr_node->data, data_size);
        curr_node = curr_node->next;
    }

    return array_head;
}

static void swap_nodes(LLNode* node1, LLNode* node2) {
    void* temp_data = node1->data;
    node1->data = node2->data;
    node2->data = temp_data;
}

void ll_swap(LinkedList* list, int index1, int index2) {
    LLNode* node1 = get_node(list, index1);
    LLNode* node2 = get_node(list, index2);
    swap_nodes(node1, node2);
}

// bubblesort is used for simplicity's sake and because of the fact that this is a linked list and data can't be accessed with constant time as with arrays
void ll_sort(LinkedList* list, int compare(void*, void*)) {
    int list_size = list->size;
    if(list_size <= 1) return;

    LLNode* curr_node;
    for(int i = 0; i < list_size; i++) {
        curr_node = list->tail;
		for(int j = list_size-1; j > i; j--) {
			if(compare(curr_node->data, curr_node->prev->data) < 0)
                swap_nodes(curr_node->prev, curr_node);
            curr_node = curr_node->prev;
        }
    }
}

void ll_reverse(LinkedList* list) {
    int half_list_size = list->size/2;

    LLNode* node1 = list->head;
    LLNode* node2 = list->tail;
    for(int i = 0; i < half_list_size; i++) {
        swap_nodes(node1, node2);
        node1 = node1->next;
        node2 = node2->prev;
    }
}

void ll_for_each(LinkedList* list, void func(void*)) {
    int list_size = list->size;

    LLNode* curr_node = list->head;
    for(int i = 0; i < list_size; i++) {
        func(curr_node->data);
        curr_node = curr_node->next;
    }
}

int ll_hash_code(LinkedList* list) {
    int hash_code = 0;
    int list_size = list->size;
    
    LLNode* curr_node = list->head;
    for(int i = 0; i < list_size; i++) {
        hash_code += (intptr_t) curr_node->data;
        curr_node = curr_node->next;
    }

    return hash_code;
}