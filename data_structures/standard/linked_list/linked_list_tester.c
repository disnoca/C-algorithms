#include <stdio.h>
#include <stdbool.h>
#include "linked_list.h"
#include "../../../wrapper_functions.h"

bool check_list_integrity(LinkedList* list) {
    if(list->size == 0) 
		return true;

    LLNode* curr_node = list->head;
    for(int i = 0; i < list->size; i++) {    // check all next links
        if(curr_node == NULL) {
            printf("Assuming list size (%d) is correct, next links incomplete.\n", list->size);
            return false;
        }
        curr_node = curr_node->next;
    }

    if(curr_node != NULL) {
        printf("Assuming list size (%d) is correct, extra elements after tail.\n", list->size);
        return false;
    } 
    
    curr_node = list->tail;
    for(int i = list->size-1; i >= 0; i--) { // check all prev links
        if(curr_node == NULL) {
            printf("Assuming list size (%d) is correct, prev links incomplete.\n", list->size);
            return false;
        }
        curr_node = curr_node->prev;
    }

    if(curr_node != NULL) {
        printf("Assuming list size (%d) is correct, extra elements before head.\n", list->size);
        return false;
    }

    return true;
}

void print_list_contents(LinkedList* list) {
    LLNode* curr_node = list->head;
    for(int i = 0; i < list->size; i++) {
        printf("%d ", curr_node->data);
        curr_node = curr_node->next;
    }
    printf("\n");
}

LinkedList* list_create_with_ascending_ints(int size) {
    LinkedList* list = ll_create();
    for(int i = 0; i < size; i++)
        ll_add(list, i);
    return list;
}

int test_add() {
    LinkedList* list = ll_create();
    ll_add(list, 1);
    ll_add(list, 2);
    ll_add(list, 3);
    ll_add_first(list, 0);

    int test_score = 0;
    test_score += list->head->data==0 ? 1 : 0;
    test_score += list->head->next->data==1 ? 10 : 0;
    test_score += list->tail->prev->data==2 ? 100 : 0;
    test_score += list->tail->data==3 ? 1000 : 0;
    test_score += list->size==4 ? 20000 : 0;
    
    check_list_integrity(list);
    ll_destroy(list);
    return test_score;
}

int test_add_at() {
    int a = 10, b = 20, c = 30, d = 40, e = 50, f=60;
    LinkedList* list = list_create_with_ascending_ints(10);
    ll_add_at(list, a, 0);
    ll_add_at(list, b, 1);
    ll_add_at(list, c, 5);
    ll_add_at(list, d, 11);
    ll_add_at(list, e, 14);
    ll_add_at(list, f, 1);

    int test_score = 0;
    test_score += a==ll_get(list, 0) ? 1 : 0;
    test_score += b==ll_get(list, 2) ? 10 : 0;
    test_score += c==ll_get(list, 6) ? 100 : 0;
    test_score += d==ll_get(list, 12) ? 1000 : 0;
    test_score += e==ll_get(list, 15) ? 10000 : 0;
    test_score += f==ll_get(list, 1) ? 100000 : 0;
    test_score += ll_get(list, 3)==0 ? 1000000 : 0;
    test_score += ll_get(list, 7)==3 ? 10000000 : 0;
    test_score += ll_get(list, 14)==9 ? 200000000 : 0;

    check_list_integrity(list);
    ll_destroy(list);
    return test_score;
}

int test_get() {
    LinkedList* list = list_create_with_ascending_ints(10);

    int test_score = 0;
    test_score += ll_get(list, 0)==0 ? 1 : 0;
    test_score += ll_get(list, 5)==5 ? 10 : 0;
    test_score += ll_get(list, 9)==9 ? 100 : 0;
    test_score += list->size==10 ? 2000 : 0;

    check_list_integrity(list);
    ll_destroy(list);
    return test_score;
}

int test_remove_at() {
    LinkedList* list = list_create_with_ascending_ints(10);

    int test_score = 0;
	test_score += ll_remove_at(list, 9)==9 ? 1 : 0;
    test_score += ll_remove_at(list, 5)==5 ? 10 : 0;
    test_score += ll_remove_at(list, 0)==0 ? 100 : 0;
    test_score += ll_get(list, 0)==1 ? 1000 : 0;
    test_score += ll_get(list, 4)==6 ? 10000 : 0;
    test_score += ll_get(list, 6)==8 ? 100000 : 0;
    test_score += ll_remove_first(list)==1 ? 1000000 : 0;
    test_score += ll_remove_last(list)==8 ? 10000000 : 0;
    test_score += list->size==5 ? 200000000 : 0;

    LinkedList* list2 = list_create_with_ascending_ints(0);
    ll_add(list2, 1);
    ll_remove_at(list2, 0);
    ll_add(list2, 1);
    ll_remove_first(list2);
    ll_add(list2, 1);
    ll_remove_last(list2);

    check_list_integrity(list);
    ll_destroy(list);
    ll_destroy(list2);
    return test_score;
}

int test_remove() {
    LinkedList* list = list_create_with_ascending_ints(10);

    int test_score = 0;
    test_score += ll_remove(list, 0) ? 1 : 0;
    test_score += !ll_remove(list, 10) ? 10 : 0;
    test_score += ll_get(list, 0)==1 ? 100 : 0;
    test_score += ll_remove(list, 5) ? 1000 : 0;
    test_score += !ll_remove(list, 5) ? 10000 : 0;
    test_score += ll_get(list, 5)==7 ? 100000 : 0;
    test_score += ll_remove(list, 9) ? 1000000 : 0;
    test_score += ll_get(list, 6)==8 ? 10000000 : 0;
    test_score += list->size==7 ? 200000000 : 0;

    LinkedList* list2 = list_create_with_ascending_ints(0);
    ll_add(list2, 1);
    ll_remove(list2, 1);

    check_list_integrity(list);
    ll_destroy(list);
    ll_destroy(list2);
    return test_score;
}

int test_set() {
    LinkedList* list = list_create_with_ascending_ints(10);
    
    int test_score = 0;
	test_score += ll_set(list, 10, 0)==0 ? 1 : 0;
    test_score += ll_set(list, 20, 5)==5 ? 10 : 0;
    test_score += ll_set(list, 30, 9)==9 ? 100 : 0;
    test_score += ll_get(list, 0)==10 ? 1000 : 0;
    test_score += ll_get(list, 5)==20 ? 10000 : 0;
    test_score += ll_get(list, 9)==30 ? 100000 : 0;
    test_score += ll_get(list, 1)==1 ? 1000000 : 0;
    test_score += ll_get(list, 8)==8 ? 20000000 : 0;

    check_list_integrity(list);
    ll_destroy(list);
    return test_score;
}

int test_index_of() {
    LinkedList* list = list_create_with_ascending_ints(10);

    int test_score = 0;
    test_score += ll_index_of(list, 0)==0 ? 1 : 0;
    test_score += ll_index_of(list, 1)==1 ? 10 : 0;
    test_score += ll_index_of(list, 4)==4 ? 100 : 0;
    test_score += ll_index_of(list, 5)==5 ? 1000 : 0;
    test_score += ll_index_of(list, 8)==8 ? 10000 : 0;
    test_score += ll_index_of(list, 9)==9 ? 200000 : 0;

    check_list_integrity(list);
    ll_destroy(list);
    return test_score;
}

int main() {
    printf("Test add: %d\n", test_add());
    printf("Test add_at: %d\n", test_add_at());
	printf("Test get: %d\n", test_get());
	printf("Test remove_at: %d\n", test_remove_at());
	printf("Test remove: %d\n", test_remove());
    printf("Test set: %d\n", test_set());
    printf("Test index_of: %d\n", test_index_of());

    printf("\nMalloc calls: %d\nFree calls: %d\n", malloc_calls, free_calls);

    return 0;
}