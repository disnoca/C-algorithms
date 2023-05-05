#include <stdio.h>
#include <stdbool.h>
#include "array_list.h"
#include "../../../wrapper_functions.h"

void print_list_contents(ArrayList* list) {
    for(int i = 0; i < list->size; i++)
        printf("%d ", list->arr[i]);
    printf("\n");
}

ArrayList* list_create_with_ascending_ints(int size) {
    ArrayList* list = al_create(size);
    for(int i = 0; i < size; i++)
        al_add(list, i);

    return list;
}

int test_add() {
    ArrayList* list = al_create(5);
    for(int i = 0; i < 11; i++)
		al_add(list, i);

	int test_score = 0;
	test_score += list->arr[0]==0 ? 1 : 0;
	test_score += list->arr[5]==5 ? 10 : 0;
	test_score += list->arr[10]==10 ? 100 : 0;
	test_score += list->size==11 ? 1000 : 0;
	test_score += list->capacity==20 ? 20000 : 0;
    
    al_destroy(list);
    return test_score;
}

int test_add_at() {
    ArrayList* list = list_create_with_ascending_ints(10);
	al_add_at(list, 10, 10);
	al_add_at(list, 5, 5);
	al_add_at(list, 0, 0);

	int test_score = 0;
	test_score += list->arr[0]==0 ? 1 : 0;
	test_score += list->arr[1]==0 ? 10 : 0;
	test_score += list->arr[6]==5 ? 100 : 0;
	test_score += list->arr[7]==5 ? 1000 : 0;
	test_score += list->arr[11]==9 ? 10000 : 0;
	test_score += list->arr[12]==10 ? 100000 : 0;
	test_score += list->size==13 ? 1000000 : 0;
	test_score += list->capacity==20 ? 20000000 : 0;
    
    al_destroy(list);
    return test_score;
}

int test_get() {
    ArrayList* list = list_create_with_ascending_ints(10);

	int test_score = 0;
	test_score += al_get(list, 0)==0 ? 1 : 0;
	test_score += al_get(list, 5)==5 ? 10 : 0;
	test_score += al_get(list, 9)==9 ? 200 : 0;
    
    al_destroy(list);
    return test_score;
}

int test_remove_at() {
    ArrayList* list = list_create_with_ascending_ints(10);

	int test_score = 0;
	test_score += al_remove_last(list)==9 ? 1 : 0;
	test_score += al_remove_at(list, 8)==8 ? 10 : 0;
	test_score += al_remove_at(list, 4)==4 ? 100 : 0;
	test_score += al_remove_at(list, 0)==0 ? 1000 : 0;
	test_score += al_get(list, 0)==1 ? 10000 : 0;
	test_score += al_get(list, 3)==5 ? 100000 : 0;
	test_score += al_get(list, 5)==7 ? 1000000 : 0;
	test_score += list->size==6 ? 20000000 : 0;

	while(list->size > 0)
		al_remove_last(list);
    
    al_destroy(list);
    return test_score;
}

int test_remove() {
    ArrayList* list = list_create_with_ascending_ints(10);

	int test_score = 0;
	test_score += al_remove(list, 9) ? 1 : 0;
	test_score += al_remove(list, 5) ? 10 : 0;
	test_score += al_remove(list, 0) ? 100 : 0;
	test_score += al_get(list, 0)==1 ? 1000 : 0;
	test_score += al_get(list, 4)==6 ? 10000 : 0;
	test_score += al_get(list, 8)==8 ? 100000 : 0;
	test_score += list->size==7 ? 2000000 : 0;
    
    al_destroy(list);
    return test_score;
}

int test_set() {
    ArrayList* list = list_create_with_ascending_ints(10);

	int test_score = 0;
	test_score += al_set(list, 0, 0)==0 ? 1 : 0;
	test_score += al_set(list, 50, 5)==5 ? 10 : 0;
	test_score += al_set(list, 90, 9)==9 ? 100 : 0;
	test_score += al_get(list, 0)==0 ? 1000 : 0;
	test_score += al_get(list, 5)==50 ? 10000 : 0;
	test_score += al_get(list, 9)==90 ? 200000 : 0;
    
    al_destroy(list);
    return test_score;
}

int test_index_of() {
    ArrayList* list = list_create_with_ascending_ints(10);

	int test_score = 0;
	test_score += al_index_of(list, 0)==0 ? 1 : 0;
	test_score += al_index_of(list, 5)==5 ? 10 : 0;
	test_score += al_index_of(list, 9)==9 ? 100 : 0;
	test_score += al_index_of(list, 10)==-1 ? 2000 : 0;
    
    al_destroy(list);
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