#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linked_list.h"
#include "../../wrapper_functions.h"

#define INT_CAST(x) *((int*)x)

int check_list_integrity(LinkedList* list) {
    if(ll_is_empty(list)) return 1;

    int list_size = list->size;
    Node* node = list->head;
    for(int i = 0; i < list_size; i++) {    // check all next links
        if(node == NULL) {
            printf("assuming list size (%d) is correct, next links incomplete\n", list_size);
            return 0;
        }
        node = node->next;
    }

    if(node != NULL) {
        printf("assuming list size (%d) is correct, extra elements after tail\n", list_size);
        return 0;
    } 
    
    node = list->tail;
    for(int i = list_size-1; i >= 0; i--) { // check all prev links
        if(node == NULL) {
            printf("assuming list size (%d) is correct, prev links incomplete\n", list_size);
            return 0;
        }
        node = node->prev;
    }

    if(node != NULL) {
        printf("assuming list size (%d) is correct, extra elements before head\n", list_size);
        return 0;
    }

    return 1;
}

void print_list_contents(LinkedList* list) {
    int list_size = list->size;
    Node* node = list->head;
    for(int i = 0; i < list_size; i++) {
        printf("%d ", INT_CAST(node->data));
        node = node->next;
    }
    printf("\n");
}

LinkedList* list_create_with_ascending_ints(int size) {   // dependent on add()'s functionality
    LinkedList* list = ll_list_create(sizeof(int));
    for(int i = 0; i < size; i++)
        ll_add_by_value(list, (void*) &i);
    return list;
}

int test_list_clear() {
    LinkedList* list1 = list_create_with_ascending_ints(1);
    LinkedList* list2 = list_create_with_ascending_ints(10);
    LinkedList* empty_list = ll_list_create(sizeof(int));
    LinkedList* empty_list2 = ll_list_create(sizeof(int));
    ll_list_clear_and_free(list1);
    ll_list_clear_and_free(list2);
    ll_list_clear_and_free(empty_list2);

    int test_score = 0;
    test_score += !memcmp(list1, empty_list, sizeof(LinkedList)) ? 1 : 0;
    test_score += !memcmp(list2, empty_list, sizeof(LinkedList)) ? 10 : 0;
    test_score += !memcmp(empty_list2, empty_list, sizeof(LinkedList)) ? 200 : 0;
    
    ll_list_destroy_and_free(list1);
    ll_list_destroy_and_free(list2);
    ll_list_destroy_and_free(empty_list);
    ll_list_destroy_and_free(empty_list2);
    return test_score;
}

int test_add() {
    int a = 1, b = 2, c = 3; 
    LinkedList* list = ll_list_create(sizeof(int));
    ll_add_by_value(list, &a);
    ll_add_by_value(list, &b);
    ll_add_by_value(list, &c);

    int* ap = list->head->data;
    int* bp1 = list->head->next->data;
    int* bp2 = list->tail->prev->data;
    int* cp = list->tail->data;
    int size = list->size;

    int test_score = 0;
    test_score += a==*ap ? 1 : 0;
    test_score += b==*bp1 ? 10 : 0;
    test_score += b==*bp2 ? 100 : 0;
    test_score += c==*cp ? 1000 : 0;
    test_score += size==3 ? 10000 : 0;
    
    LinkedList* list2 = ll_list_create(sizeof(int));
    ll_add_by_value(list2, &a);

    int* ap1 = list2->head->data;
    int* ap2 = list2->tail->data;
    int size2 = list2->size;

    test_score += a==*ap1 ? 100000 : 0;
    test_score += a==*ap2 ? 1000000 : 0;
    test_score += size2==1 ? 20000000 : 0;
    
    check_list_integrity(list);
    check_list_integrity(list2);
    ll_list_destroy_and_free(list);
    ll_list_destroy_and_free(list2);
    return test_score;
}

int test_add_at() {
    int a = 10, b = 20, c = 30, d = 40, e = 50, f=60;
    LinkedList* list = list_create_with_ascending_ints(10);
    ll_add_by_value_at(list, &a, 0);
    ll_add_by_value_at(list, &b, 1);
    ll_add_by_value_at(list, &c, 5);
    ll_add_by_value_at(list, &d, 11);
    ll_add_by_value_at(list, &e, 14);
    ll_add_by_value_at(list, &f, 1);

    int* ap = (int*) ll_get(list, 0);
    int* bp = (int*) ll_get(list, 2);
    int* cp = (int*) ll_get(list, 6);
    int* dp = (int*) ll_get(list, 12);
    int* ep = (int*) ll_get(list, 15);
    int* fp = (int*) ll_get(list, 1);

    int* p1 = (int*) ll_get(list, 3);
    int* p2 = (int*) ll_get(list, 7);
    int* p3 = (int*) ll_get(list, 14);

    int test_score = 0;
    test_score += a==*ap ? 1 : 0;
    test_score += b==*bp ? 10 : 0;
    test_score += c==*cp ? 100 : 0;
    test_score += d==*dp ? 1000 : 0;
    test_score += e==*ep ? 10000 : 0;
    test_score += f==*fp ? 100000 : 0;
    test_score += *p1==0 ? 1000000 : 0;
    test_score += *p2==3 ? 10000000 : 0;
    test_score += *p3==9 ? 200000000 : 0;

    check_list_integrity(list);
    ll_list_destroy_and_free(list);
    return test_score;
}

int test_get() {
    LinkedList* list = list_create_with_ascending_ints(10);
    int* ap = (int*) ll_get(list, 0);
    int* bp = (int*) ll_get(list, 5);
    int* cp = (int*) ll_get(list, 9);
    int size = list->size;

    int test_score = 0;
    test_score += *ap==0 ? 1 : 0;
    test_score += *bp==5 ? 10 : 0;
    test_score += *cp==9 ? 100 : 0;
    test_score += size==10 ? 2000 : 0;

    check_list_integrity(list);
    ll_list_destroy_and_free(list);
    return test_score;
}

int test_remove() {
    LinkedList* list = list_create_with_ascending_ints(10);
    ll_remove_and_free(list, 9);
    ll_remove_and_free(list, 5);
    ll_remove_and_free(list, 0);

    int* p1 = ll_get(list, 0);
    int* p2 = ll_get(list, 4);
    int* p3 = ll_get(list, 6);

    int test_score = 0;
    test_score += *p1==1 ? 1 : 0;
    test_score += *p2==6 ? 10 : 0;
    test_score += *p3==8 ? 200 : 0;

    check_list_integrity(list);
    ll_list_destroy_and_free(list);
    return test_score;
}

int test_set() {
    LinkedList* list = list_create_with_ascending_ints(10);

    int a = 10;
    int b = 20;
    int c = 30;
    int d = 40;
    
    ll_set_by_value_and_free(list, &a, 0);
    ll_set_by_value_and_free(list, &b, 5);
    ll_set_by_value_and_free(list, &c, 9);
    ll_set_by_value_and_free(list, &d, 10);

    int* ap = ll_get(list, 0);
    int* bp = ll_get(list, 5);
    int* cp = ll_get(list, 9);
    int* dp = ll_get(list, 10);
    int* ep = ll_get(list, 1);
    int* fp = ll_get(list, 8);

    int test_score = 0;
    test_score += *ap==10 ? 1 : 0;
    test_score += *bp==20 ? 10 : 0;
    test_score += *cp==30 ? 100 : 0;
    test_score += *dp==40 ? 1000 : 0;
    test_score += *ep==1 ? 10000 : 0;
    test_score += *fp==8 ? 100000 : 0;
    test_score += list->size==11 ? 2000000 : 0;

    check_list_integrity(list);
    ll_list_destroy_and_free(list);
    return test_score;
}

int test_add_all() {
    LinkedList* list1 = list_create_with_ascending_ints(10);
    LinkedList* list2 = list_create_with_ascending_ints(5);
    ll_add_all_by_value(list1, list2);
    
    int* p1_1 = list1->tail->data;
    int* p1_2 = list2->tail->data;
    int* p2_1 = ll_get(list1, 10);
    int* p2_2 = list2->head->data;
    int* p3 = ll_get(list1, 9);

    int test_score = 0;
    test_score += list1->size==15 ? 1 : 0;
    test_score += list2->size==5 ? 10 : 0;
    test_score += *p1_1==*p1_2 ? 100 : 0;
    test_score += *p2_1==*p2_2 ? 1000 : 0;
    test_score += *p3==9 ? 20000 : 0;

    check_list_integrity(list1);
    check_list_integrity(list2);
    ll_list_destroy_and_free(list1);
    ll_list_destroy_and_free(list2);
    return test_score;
}

int test_add_all_at() {
    LinkedList* list1 = list_create_with_ascending_ints(10);
    LinkedList* list2 = list_create_with_ascending_ints(5);
    int a = 10;
    ll_add_by_value_at(list2, &a, 0);
    ll_add_all_by_value_at(list1, list2, 0);
    
    int* p1_1 = list1->head->data;
    int* p1_2 = list2->head->data;
    int* p2_1 = ll_get(list1, 5);
    int* p2_2 = list2->tail->data;
    int* p3 = list1->tail->data;

    int test_score = 0;
    test_score += list1->size==16 ? 1 : 0;
    test_score += list2->size==6 ? 10 : 0;
    test_score += *p1_1==*p1_2 ? 100 : 0;
    test_score += *p2_1==*p2_2 ? 1000 : 0;
    test_score += *p3==9 ? 10000 : 0;

    ll_add_all_by_value_at(list1, list2, 10);

    int* p4 = ll_get(list1, 10);
    int* p5 = ll_get(list1, 15);
    int* p6 = ll_get(list1, 16);

    test_score += list1->size==22 ? 100000 : 0;
    test_score += *p4==*p1_2 ? 1000000 : 0;
    test_score += *p5==*p2_2 ? 10000000 : 0;
    test_score += *p6==4 ? 200000000 : 0;

    check_list_integrity(list1);
    check_list_integrity(list2);
    ll_list_destroy_and_free(list1);
    ll_list_destroy_and_free(list2);
    return test_score;
}

int test_index_of() {
    LinkedList* list = list_create_with_ascending_ints(10);

    int test_score = 0;
    ll_add_by_value(list, &test_score);
    test_score += ll_index_of_value(list, &test_score)==0 ? 1 : 0;
    
    int a = 5;
    int b = 10;

    test_score += ll_index_of_reference(list, &a)==-1 ? 10 : 0;
    test_score += ll_index_of_value(list, &b)==-1 ? 100 : 0;

    ll_add_by_reference(list, &a);
    ll_add_by_reference(list, &b);

    test_score += ll_index_of_value(list, &a)==5 ? 1000 : 0;
    test_score += ll_index_of_value(list, &b)==12 ? 10000 : 0;
    test_score += ll_index_of_reference(list, &a)==11 ? 100000 : 0;
    test_score += ll_index_of_reference(list, &b)==12 ? 2000000 : 0;

    check_list_integrity(list);
    ll_remove_by_reference(list, &a);
    ll_remove_by_reference(list, &b);
    ll_list_destroy_and_free(list);
    return test_score;
}

int test_last_index_of() {
    LinkedList* list = list_create_with_ascending_ints(10);

    int test_score = 0;
    ll_add_by_value(list, &test_score);
    test_score += ll_last_index_of_value(list, &test_score)==10 ? 1 : 0;
    
    int a = 5;
    int b = 2;

    test_score += ll_last_index_of_reference(list, &a)==-1 ? 10 : 0;
    test_score += ll_last_index_of_value(list, &b)==2 ? 100 : 0;

    ll_add_by_reference(list, &a);
    ll_add_by_reference(list, &b);

    test_score += ll_last_index_of_value(list, &a)==11 ? 1000 : 0;
    test_score += ll_last_index_of_value(list, &b)==12 ? 10000 : 0;
    test_score += ll_last_index_of_reference(list, &a)==11 ? 100000 : 0;
    test_score += ll_last_index_of_reference(list, &b)==12 ? 2000000 : 0;

    check_list_integrity(list);
    ll_remove_by_reference(list, &a);
    ll_remove_by_reference(list, &b);
    ll_list_destroy_and_free(list);
    return test_score;
}

int test_remove_by() {
    LinkedList* list = list_create_with_ascending_ints(10);

    int a = 0, b = 4, c = 6, d = 9;
    ll_remove_by_value_and_free(list, (void*) &a);
    ll_remove_by_value_and_free(list, (void*) &b);
    ll_remove_by_reference_and_free(list, (void*) &c);
    ll_remove_by_value_and_free(list, (void*) &d);

    int test_score = 0;
    test_score += !ll_contains_value(list, (void*) &a) ? 1 : 0;
    test_score += !ll_contains_value(list, (void*) &b) ? 10 : 0;
    test_score += ll_contains_value(list, (void*) &c) ? 100 : 0;
    test_score += !ll_contains_value(list, (void*) &d) ? 2000 : 0;
    
    check_list_integrity(list);
    ll_list_destroy_and_free(list);
    return test_score;
}

int test_contains_all() {
    LinkedList* list1 = list_create_with_ascending_ints(10);
    LinkedList* list2 = list_create_with_ascending_ints(10);

    int test_score = 0;
    test_score += ll_contains_all_values(list1, list2) ? 1 : 0;
    test_score += !ll_contains_all_references(list1, list2) ? 10 : 0;
    
    int a = 10;
    ll_add_by_value(list2, &a);

    test_score += !ll_contains_all_values(list1, list2) ? 100 : 0;

    LinkedList* list3 = ll_list_create(sizeof(int));
    ll_add_all_by_reference(list3, list1);
    
    test_score += ll_contains_all_references(list1, list3) ? 2000 : 0;

    check_list_integrity(list1);
    check_list_integrity(list2);
    check_list_integrity(list3);
    ll_remove_by_reference(list2, &a);
    ll_list_destroy_and_free(list1);
    ll_list_destroy_and_free(list2);
    ll_list_destroy(list3);
    return test_score;
}

int test_remove_all() {
    LinkedList* list1 = list_create_with_ascending_ints(10);
    int a = 10;
    ll_add_by_value(list1, &a);
    LinkedList* list2 = list_create_with_ascending_ints(10);
    LinkedList* list3 = list_create_with_ascending_ints(5);
    LinkedList* list4 = ll_list_create(sizeof(int));

    int test_score = 0;
    test_score += !ll_remove_all_by_value_and_free(list1, list4) ? 1 : 0;
    test_score += list1->size==11 ? 10 : 0;
    test_score += ll_remove_all_by_value_and_free(list1, list3) ? 100 : 0;
    test_score += *((int*) (list1->head->data))==5 ? 1000 : 0;
    test_score += *((int*) (list1->tail->data))==10 ? 10000 : 0;
    test_score += list1->size==6 ? 100000 : 0;
    test_score += ll_remove_all_by_value_and_free(list1, list2) ? 1000000 : 0;

    int b = 1;
    ll_add_by_value(list4, &b);

    test_score += !ll_remove_all_by_value_and_free(list1, list4) ? 10000000 : 0;
    test_score += list1->size==1 ? 200000000 : 0;

    check_list_integrity(list1);
    check_list_integrity(list2);
    check_list_integrity(list3);
    check_list_integrity(list4);
    ll_list_destroy_and_free(list1);
    ll_list_destroy_and_free(list2);
    ll_list_destroy_and_free(list3);
    ll_list_destroy_and_free(list4);
    return test_score;
}

int test_retain_all() {
    LinkedList* list1 = list_create_with_ascending_ints(10);
    LinkedList* list2 = list_create_with_ascending_ints(10);
    LinkedList* list3 = list_create_with_ascending_ints(5);
    LinkedList* list4 = ll_list_create(sizeof(int));

    int test_score = 0;
    test_score += !ll_retain_all_by_value_and_free(list1, list2) ? 1 : 0;
    test_score += list1->size==10 ? 10 : 0;
    test_score += ll_retain_all_by_value_and_free(list1, list3) ? 100 : 0;
    test_score += *((int*) (list1->head->data))==0 ? 1000 : 0;
    test_score += *((int*) (list1->tail->data))==4 ? 10000 : 0;
    test_score += list1->size==5 ? 100000 : 0;
    test_score += ll_retain_all_by_value_and_free(list1, list4) ? 1000000 : 0;
    test_score += list1->size==0 ? 20000000 : 0;

    check_list_integrity(list1);
    check_list_integrity(list2);
    check_list_integrity(list3);
    check_list_integrity(list4);
    ll_list_destroy_and_free(list1);
    ll_list_destroy_and_free(list2);
    ll_list_destroy_and_free(list3);
    ll_list_destroy_and_free(list4);
    return test_score;
}

int test_sublist() {
    LinkedList* list1 = list_create_with_ascending_ints(10);
    LinkedList* list2 = ll_sublist_by_value(list1, 0, 0);
    LinkedList* list3 = ll_sublist_by_value(list1, 0, 10);
    LinkedList* list4 = ll_sublist_by_value(list1, 3, 7);

    int* ap = ll_get(list3, 5);
    int* bp = ll_get(list3, 9);
    int* cp = ll_get(list4, 0);
    int* dp = ll_get(list4, 3);
    int* ep = ll_get(list4, 2);

    int test_score = 0;
    test_score += list2->size==0 ? 1 : 0;
    test_score += list3->size==10 ? 10 : 0;
    test_score += list4->size==4 ? 100 : 0;
    test_score += *ap==5 ? 1000 : 0;
    test_score += *bp==9 ? 10000 : 0;
    test_score += *cp==3 ? 100000 : 0;
    test_score += *dp==6 ? 1000000 : 0;
    test_score += *ep==5 ? 20000000 : 0;

    check_list_integrity(list1);
    check_list_integrity(list2);
    check_list_integrity(list3);
    check_list_integrity(list4);
    ll_list_destroy_and_free(list1);
    ll_list_destroy_and_free(list2);
    ll_list_destroy_and_free(list3);
    ll_list_destroy_and_free(list4);
    return test_score;
}

int test_to_array() {
    LinkedList* list = list_create_with_ascending_ints(10);
    int* array =  ll_to_array(list);

    int test_score = 0;
    test_score += array[0]==0 ? 1 : 0;
    test_score += array[5]==5 ? 10 : 0;
    test_score += array[9]==9 ? 200 : 0;

    check_list_integrity(list);
    ll_list_destroy_and_free(list);
    Free(array);
    return test_score;
}

int test_swap() {
    LinkedList* list = list_create_with_ascending_ints(10);

    int* ap = (int*) ll_get(list, 3);
    ll_swap(list, 0, 9);
    ll_swap(list, 7, 3);

    int test_score = 0;
    test_score += *((int*) ll_get(list, 0))==9 ? 1 : 0;
    test_score += *((int*) ll_get(list, 9))==0 ? 10 : 0;
    test_score += *((int*) ll_get(list, 3))==7 ? 100 : 0;
    test_score += *((int*) ll_get(list, 7))==3 ? 1000 : 0;
    test_score += *ap==3 ? 20000 : 0;

    check_list_integrity(list);
    ll_list_destroy_and_free(list);
    return test_score;
}

int descending_order_comparator(void* arg1, void* arg2) {
    int a = INT_CAST(arg1);
    int b = INT_CAST(arg2);

    if(a > b) return -1;
    if(a < b) return 1;
    return 0;
}

int is_in_descending_order(LinkedList* list) {
    int list_size = list->size;
    if(list_size <= 1) return 1;

    int curr;
    int prev = INT_CAST(ll_get(list, 0));
    for(int i = 1; i < list_size; i++) {
        if((curr = INT_CAST(ll_get(list, i))) > prev)
            return 0;
        prev = curr;
    }
    return 1;
}

int test_sort() {
    LinkedList* list = list_create_with_ascending_ints(10);
    ll_sort(list, descending_order_comparator);

    int test_score = is_in_descending_order(list)*2;

    check_list_integrity(list);
    ll_list_destroy_and_free(list);
    return test_score;
}

int test_reverse() {
    LinkedList* list1 = list_create_with_ascending_ints(10);
    LinkedList* list2 = list_create_with_ascending_ints(11);
    ll_reverse(list1);
    ll_reverse(list2);

    int test_score = 0;
    test_score += is_in_descending_order(list1) ? 1 : 0;
    test_score += is_in_descending_order(list2) ? 20 : 0;

    check_list_integrity(list1);
    check_list_integrity(list2);
    ll_list_destroy_and_free(list1);
    ll_list_destroy_and_free(list2);
    return test_score;
}

void duplicate_int(void* ptr) {
    *((int*) ptr) *= 2;
}

int test_for_each() {
    LinkedList* list = list_create_with_ascending_ints(10);
    ll_for_each(list, duplicate_int);

    for(int i = 0; i < 10; i++)
        if(INT_CAST(ll_get(list,i)) != i*2)
            return 0;

    check_list_integrity(list);
    ll_list_destroy_and_free(list);
    return 2;
}

int test_hash_code() {
    LinkedList* list1 = list_create_with_ascending_ints(10);
    LinkedList* list2 = list_create_with_ascending_ints(10);
    LinkedList* list3 = ll_list_create(sizeof(int));
    ll_add_all_by_reference(list3, list1);

    int test_score = 0;
    test_score += ll_hash_code(list1)!=ll_hash_code(list2) ? 1 : 0;
    test_score += ll_hash_code(list1)==ll_hash_code(list3) ? 20 : 0;

    check_list_integrity(list1);
    check_list_integrity(list2);
    check_list_integrity(list3);
    ll_list_destroy_and_free(list1);
    ll_list_destroy_and_free(list2);
    ll_list_destroy(list3);
    return 2;
}

int main() {
    printf("Test list_clear: %d\n", test_list_clear());
    printf("Test add: %d\n", test_add());
    printf("Test add_at: %d\n", test_add_at());
    printf("Test get: %d\n", test_get());
    printf("Test remove: %d\n", test_remove());
    printf("Test set: %d\n", test_set());
    printf("Test add_all: %d\n", test_add_all());
    printf("Test add_all_at: %d\n", test_add_all_at());
    printf("Test index_of: %d\n", test_index_of());
    printf("Test last_index_of: %d\n", test_last_index_of());
    printf("Test remove_by: %d\n", test_remove_by());
    printf("Test contains_all: %d\n", test_contains_all());
    printf("Test remove_all: %d\n", test_remove_all());
    printf("Test retain_all: %d\n", test_retain_all());
    printf("Test sublist: %d\n", test_sublist());
    printf("Test to_array: %d\n", test_to_array());
    printf("Test swap: %d\n", test_swap());
    printf("Test sort: %d\n", test_sort());
    printf("Test reverse: %d\n", test_reverse());
    printf("Test for_each: %d\n", test_for_each());
    printf("Test hash_code: %d\n", test_hash_code());

    printf("\nMalloc calls: %d\nFree calls: %d\n", malloc_calls, free_calls);

    return 0;
}