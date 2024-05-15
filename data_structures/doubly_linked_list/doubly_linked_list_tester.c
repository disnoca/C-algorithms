#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "doubly_linked_list.h"
#include "../../wrapper_functions.h"


int values[1024];
int curr_val;

#define REF_OF(x)   ({ values[curr_val] = (x); values + curr_val++; })
#define DEREF(p)    (*((int*)(p)))


static bool check_list_integrity(DoublyLinkedList* list) {
    if (list->size == 0) 
		return true;

    DLLNode* curr_node = list->head;
    for(size_t i = 0; i < list->size; i++) {    // check all next links
        if (curr_node == NULL) {
            printf("Assuming list size (%ld) is correct, next links incomplete.\n", list->size);
            return false;
        }
        curr_node = curr_node->next;
    }

    if (curr_node != NULL) {
        printf("Assuming list size (%ld) is correct, extra elements after tail.\n", list->size);
        return false;
    } 
    
    curr_node = list->tail;
    for(size_t i = 0; i < list->size; i++) { // check all prev links
        if (curr_node == NULL) {
            printf("Assuming list size (%ld) is correct, prev links incomplete.\n", list->size);
            return false;
        }
        curr_node = curr_node->prev;
    }

    if (curr_node != NULL) {
        printf("Assuming list size (%ld) is correct, extra elements before head.\n", list->size);
        return false;
    }

    return true;
}

__attribute__((unused)) static void print_list_contents(DoublyLinkedList* list) {
    DLLNode* curr_node = list->head;
    for(size_t i = 0; i < list->size; i++) {
        printf("%d ", DEREF(curr_node->data));
        curr_node = curr_node->next;
    }
    printf("\n");
}

static DoublyLinkedList* list_create_with_ascending_ints(int size) {
    DoublyLinkedList* list = ll_create();
    for(int i = 0; i < size; i++)
        ll_add(list, REF_OF(i));
    return list;
}

static void test_add() {
    DoublyLinkedList* list = ll_create();
    ll_add(list, REF_OF(1));
    ll_add(list, REF_OF(2));
    ll_add(list, REF_OF(3));
    ll_add_first(list, REF_OF(0));

    assert(DEREF(list->head->data) == 0);
    assert(DEREF(list->head->next->data) == 1);
    assert(DEREF(list->tail->prev->data) == 2);
    assert(DEREF(list->tail->data) == 3);
    assert(list->size == 4);
    
    assert(check_list_integrity(list));
    ll_destroy(list);
}

static void test_add_at() {
    int a = 10, b = 20, c = 30, d = 40, e = 50, f=60;
    DoublyLinkedList* list = list_create_with_ascending_ints(10);
    ll_add_at(list, &a, 0);
    ll_add_at(list, &b, 1);
    ll_add_at(list, &c, 5);
    ll_add_at(list, &d, 11);
    ll_add_at(list, &e, 14);
    ll_add_at(list, &f, 1);

    assert(ll_get(list, 0) == &a);
    assert(ll_get(list, 2) == &b);
    assert(ll_get(list, 6) == &c);
    assert(ll_get(list, 12) == &d);
    assert(ll_get(list, 15) == &e);
    assert(ll_get(list, 1) == &f);
    assert(DEREF(ll_get(list, 3)) == 0);
    assert(DEREF(ll_get(list, 7)) == 3);
    assert(DEREF(ll_get(list, 14)) == 9);

    assert(check_list_integrity(list));
    ll_destroy(list);
}

static void test_get() {
    DoublyLinkedList* list = list_create_with_ascending_ints(10);

    assert(DEREF(ll_get(list, 0)) == 0);
    assert(DEREF(ll_get(list, 5)) == 5);
    assert(DEREF(ll_get(list, 9)) == 9);
    assert(list->size==10);

    assert(check_list_integrity(list));
    ll_destroy(list);
}

static void test_remove_at() {
    DoublyLinkedList* list = list_create_with_ascending_ints(10);

	assert(DEREF(ll_remove_at(list, 9)) == 9);
    assert(DEREF(ll_remove_at(list, 5)) == 5);
    assert(DEREF(ll_remove_at(list, 0)) == 0);
    assert(DEREF(ll_get(list, 0)) == 1);
    assert(DEREF(ll_get(list, 4)) == 6);
    assert(DEREF(ll_get(list, 6)) == 8);
    assert(DEREF(ll_remove_first(list)) == 1);
    assert(DEREF(ll_remove_last(list)) == 8);
    assert(list->size==5);

    assert(check_list_integrity(list));
    ll_destroy(list);
}

static void test_remove() {
    DoublyLinkedList* list = list_create_with_ascending_ints(10);

    assert(ll_remove(list, ll_get(list, 0)));
    assert(!ll_remove(list, REF_OF(10)));
    assert(DEREF(ll_get(list, 0)) == 1);
    assert(ll_remove(list, ll_get(list, 5)));
    assert(!ll_remove(list, REF_OF(5)));
    assert(DEREF(ll_get(list, 5)) == 7);
    assert(ll_remove(list, ll_get(list, 9)));
    assert(DEREF(ll_get(list, 6)) == 8);
    assert(list->size==7);

    assert(check_list_integrity(list));
    ll_destroy(list);
}

static void test_set() {
    DoublyLinkedList* list = list_create_with_ascending_ints(10);
    
	assert(DEREF(ll_set(list, REF_OF(10), 0)) == 0);
    assert(DEREF(ll_set(list, REF_OF(20), 5)) == 5);
    assert(DEREF(ll_set(list, REF_OF(30), 9)) == 9);
    assert(DEREF(ll_get(list, 0)) == 10);
    assert(DEREF(ll_get(list, 5)) == 20);
    assert(DEREF(ll_get(list, 9)) == 30);
    assert(DEREF(ll_get(list, 1)) == 1);
    assert(DEREF(ll_get(list, 8)) == 8);

    assert(check_list_integrity(list));
    ll_destroy(list);
}

static void test_index_of() {
    DoublyLinkedList* list = list_create_with_ascending_ints(10);

    assert(ll_index_of(list, ll_get(list, 0)) == 0);
    assert(ll_index_of(list, ll_get(list, 1)) == 1);
    assert(ll_index_of(list, ll_get(list, 4)) == 4);
    assert(ll_index_of(list, ll_get(list, 5)) == 5);
    assert(ll_index_of(list, ll_get(list, 8)) == 8);
    assert(ll_index_of(list, ll_get(list, 9)) == 9);

    assert(check_list_integrity(list));
    ll_destroy(list);
}

int main() {
    test_add();
    test_add_at();
	test_get();
    test_remove_at();
	test_remove();
    test_set();
    test_index_of();

    assert(malloc_calls == free_calls);

    printf("All tests passed.\n");
    return 0;
}
