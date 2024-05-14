#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "doubly_linked_list.h"
#include "../../wrapper_functions.h"

static bool check_list_integrity(DoublyLinkedList* list) {
    if (list->size == 0) 
		return true;

    DLLNode* curr_node = list->head;
    for(int i = 0; i < list->size; i++) {    // check all next links
        if (curr_node == NULL) {
            printf("Assuming list size (%d) is correct, next links incomplete.\n", list->size);
            return false;
        }
        curr_node = curr_node->next;
    }

    if (curr_node != NULL) {
        printf("Assuming list size (%d) is correct, extra elements after tail.\n", list->size);
        return false;
    } 
    
    curr_node = list->tail;
    for(int i = list->size-1; i >= 0; i--) { // check all prev links
        if (curr_node == NULL) {
            printf("Assuming list size (%d) is correct, prev links incomplete.\n", list->size);
            return false;
        }
        curr_node = curr_node->prev;
    }

    if (curr_node != NULL) {
        printf("Assuming list size (%d) is correct, extra elements before head.\n", list->size);
        return false;
    }

    return true;
}

__attribute__((unused)) static void print_list_contents(DoublyLinkedList* list) {
    DLLNode* curr_node = list->head;
    for(int i = 0; i < list->size; i++) {
        printf("%d ", curr_node->data);
        curr_node = curr_node->next;
    }
    printf("\n");
}

static DoublyLinkedList* list_create_with_ascending_ints(int size) {
    DoublyLinkedList* list = ll_create();
    for(int i = 0; i < size; i++)
        ll_add(list, i);
    return list;
}

static void test_add() {
    DoublyLinkedList* list = ll_create();
    ll_add(list, 1);
    ll_add(list, 2);
    ll_add(list, 3);
    ll_add_first(list, 0);

    assert(list->head->data==0);
    assert(list->head->next->data==1);
    assert(list->tail->prev->data==2);
    assert(list->tail->data==3);
    assert(list->size==4);
    
    assert(check_list_integrity(list));
    ll_destroy(list);
}

static void test_add_at() {
    int a = 10, b = 20, c = 30, d = 40, e = 50, f=60;
    DoublyLinkedList* list = list_create_with_ascending_ints(10);
    ll_add_at(list, a, 0);
    ll_add_at(list, b, 1);
    ll_add_at(list, c, 5);
    ll_add_at(list, d, 11);
    ll_add_at(list, e, 14);
    ll_add_at(list, f, 1);

    assert(a==ll_get(list, 0));
    assert(b==ll_get(list, 2));
    assert(c==ll_get(list, 6));
    assert(d==ll_get(list, 12));
    assert(e==ll_get(list, 15));
    assert(f==ll_get(list, 1));
    assert(ll_get(list, 3)==0);
    assert(ll_get(list, 7)==3);
    assert(ll_get(list, 14)==9);

    assert(check_list_integrity(list));
    ll_destroy(list);
}

static void test_get() {
    DoublyLinkedList* list = list_create_with_ascending_ints(10);

    assert(ll_get(list, 0)==0);
    assert(ll_get(list, 5)==5);
    assert(ll_get(list, 9)==9);
    assert(list->size==10);

    assert(check_list_integrity(list));
    ll_destroy(list);
}

static void test_remove_at() {
    DoublyLinkedList* list = list_create_with_ascending_ints(10);

	assert(ll_remove_at(list, 9)==9);
    assert(ll_remove_at(list, 5)==5);
    assert(ll_remove_at(list, 0)==0);
    assert(ll_get(list, 0)==1);
    assert(ll_get(list, 4)==6);
    assert(ll_get(list, 6)==8);
    assert(ll_remove_first(list)==1);
    assert(ll_remove_last(list)==8);
    assert(list->size==5);

    DoublyLinkedList* list2 = list_create_with_ascending_ints(0);
    ll_add(list2, 1);
    ll_remove_at(list2, 0);
    ll_add(list2, 1);
    ll_remove_first(list2);
    ll_add(list2, 1);
    ll_remove_last(list2);

    assert(check_list_integrity(list));
    ll_destroy(list);
    ll_destroy(list2);
}

static void test_remove() {
    DoublyLinkedList* list = list_create_with_ascending_ints(10);

    assert(ll_remove(list, 0));
    assert(!ll_remove(list, 10));
    assert(ll_get(list, 0)==1);
    assert(ll_remove(list, 5));
    assert(!ll_remove(list, 5));
    assert(ll_get(list, 5)==7);
    assert(ll_remove(list, 9));
    assert(ll_get(list, 6)==8);
    assert(list->size==7);

    DoublyLinkedList* list2 = list_create_with_ascending_ints(0);
    ll_add(list2, 1);
    ll_remove(list2, 1);

    assert(check_list_integrity(list));
    ll_destroy(list);
    ll_destroy(list2);
}

static void test_set() {
    DoublyLinkedList* list = list_create_with_ascending_ints(10);
    
	assert(ll_set(list, 10, 0)==0);
    assert(ll_set(list, 20, 5)==5);
    assert(ll_set(list, 30, 9)==9);
    assert(ll_get(list, 0)==10);
    assert(ll_get(list, 5)==20);
    assert(ll_get(list, 9)==30);
    assert(ll_get(list, 1)==1);
    assert(ll_get(list, 8)==8);

    assert(check_list_integrity(list));
    ll_destroy(list);
}

static void test_index_of() {
    DoublyLinkedList* list = list_create_with_ascending_ints(10);

    assert(ll_index_of(list, 0)==0);
    assert(ll_index_of(list, 1)==1);
    assert(ll_index_of(list, 4)==4);
    assert(ll_index_of(list, 5)==5);
    assert(ll_index_of(list, 8)==8);
    assert(ll_index_of(list, 9)==9);

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
