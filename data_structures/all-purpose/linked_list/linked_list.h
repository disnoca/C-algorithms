#ifndef LINKED_LIST
#define LINKED_LIST

#include <stdlib.h>

typedef struct LLNode LLNode;

struct LLNode {
    LLNode *prev, *next;
    void* data;
};

typedef struct {
    LLNode *head, *tail;
    int size;
    size_t data_size;
} LinkedList;

LinkedList* ll_list_create(size_t data_size);

void ll_list_clear(LinkedList* list);
void ll_list_clear_and_free(LinkedList* list);

void ll_list_destroy(LinkedList* list);
void ll_list_destroy_and_free(LinkedList* list);

int ll_size(LinkedList* list);
int ll_is_empty(LinkedList* list);

void ll_add_by_value(LinkedList* list, void* data);
void ll_add_by_reference(LinkedList* list, void* data);
void ll_add_by_value_at(LinkedList* list, void* data, int index);
void ll_add_by_reference_at(LinkedList* list, void* data, int index);

void* ll_get(LinkedList* list, int index);

void* ll_set_by_value(LinkedList* list, void* data, int index);
void* ll_set_by_reference(LinkedList* list, void* data, int index);
void ll_set_by_value_and_free(LinkedList* list, void* data, int index);
void ll_set_by_reference_and_free(LinkedList* list, void* data, int index);

void* ll_remove(LinkedList* list, int index);
void ll_remove_and_free(LinkedList* list, int index);

void ll_remove_by_value(LinkedList* list, void* data);
void ll_remove_by_value_and_free(LinkedList* list, void* data);
void ll_remove_by_reference(LinkedList* list, void* data);
void ll_remove_by_reference_and_free(LinkedList* list, void* data);

int ll_index_of_value(LinkedList* list, void* data);
int ll_index_of_reference(LinkedList* list, void* data);
int ll_last_index_of_value(LinkedList* list, void* data);
int ll_last_index_of_reference(LinkedList* list, void* data);

int ll_contains_value(LinkedList* list, void* data);
int ll_contains_reference(LinkedList* list, void* data);
int ll_contains_all_values(LinkedList* list, LinkedList* to_comp_list);
int ll_contains_all_references(LinkedList* list, LinkedList* to_comp_list);

void ll_add_all_by_value(LinkedList* dest_list, LinkedList* source_list);
void ll_add_all_by_reference(LinkedList* dest_list, LinkedList* source_list);
void ll_add_all_by_value_at(LinkedList* dest_list, LinkedList* source_list, int index);
void ll_add_all_by_reference_at(LinkedList* dest_list, LinkedList* source_list, int index);

int ll_remove_all_by_value(LinkedList* dest_list, LinkedList* source_list);
int ll_remove_all_by_reference(LinkedList* dest_list, LinkedList* source_list);
int ll_remove_all_by_value_and_free(LinkedList* dest_list, LinkedList* source_list);
// it doesn't make sense for there to be a remove_all_by_reference_and_free because source_list's data would be freed alongside with it

int ll_retain_all_by_value(LinkedList* dest_list, LinkedList* source_list);
int ll_retain_all_by_reference(LinkedList* dest_list, LinkedList* source_list);
int ll_retain_all_by_value_and_free(LinkedList* dest_list, LinkedList* source_list);
int ll_retain_all_by_reference_and_free(LinkedList* dest_list, LinkedList* source_list);

// inclusive and exclusive indexes, respectively
LinkedList* ll_sublist_by_value(LinkedList* source_list, int from_index, int to_index);
LinkedList* ll_sublist_by_reference(LinkedList* source_list, int from_index, int to_index);

void* ll_to_array(LinkedList* list);

void ll_swap(LinkedList* list, int index1, int index2);

void ll_sort(LinkedList* list, int compare(void*, void*));

void ll_reverse(LinkedList* list);

void ll_for_each(LinkedList* list, void func(void*));

int ll_hash_code(LinkedList* list);

#endif