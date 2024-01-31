#include <stdio.h>
#include <stdbool.h>
#include "dynamic_array.h"
#include "../../../wrapper_functions.h"

void print_da_contents(DynamicArray* da) {
    for(int i = 0; i < da->size; i++)
        printf("%d ", da->arr[i]);
    printf("\n");
}

DynamicArray* da_create_with_ascending_ints(int size) {
    DynamicArray* da = da_create(size);
    for(int i = 0; i < size; i++)
        da_add(da, i);

    return da;
}

int test_add() {
    DynamicArray* da = da_create(5);
    for(int i = 0; i < 11; i++)
		da_add(da, i);

	int test_score = 0;
	test_score += da->arr[0]==0 ? 1 : 0;
	test_score += da->arr[5]==5 ? 10 : 0;
	test_score += da->arr[10]==10 ? 100 : 0;
	test_score += da->size==11 ? 1000 : 0;
	test_score += da->capacity==20 ? 20000 : 0;
    
    da_destroy(da);
    return test_score;
}

int test_add_at() {
    DynamicArray* da = da_create_with_ascending_ints(10);
	da_add_at(da, 10, 10);
	da_add_at(da, 5, 5);
	da_add_at(da, 0, 0);

	int test_score = 0;
	test_score += da->arr[0]==0 ? 1 : 0;
	test_score += da->arr[1]==0 ? 10 : 0;
	test_score += da->arr[6]==5 ? 100 : 0;
	test_score += da->arr[7]==5 ? 1000 : 0;
	test_score += da->arr[11]==9 ? 10000 : 0;
	test_score += da->arr[12]==10 ? 100000 : 0;
	test_score += da->size==13 ? 1000000 : 0;
	test_score += da->capacity==20 ? 20000000 : 0;
    
    da_destroy(da);
    return test_score;
}

int test_get() {
    DynamicArray* da = da_create_with_ascending_ints(10);

	int test_score = 0;
	test_score += da_get(da, 0)==0 ? 1 : 0;
	test_score += da_get(da, 5)==5 ? 10 : 0;
	test_score += da_get(da, 9)==9 ? 200 : 0;
    
    da_destroy(da);
    return test_score;
}

int test_remove_at() {
    DynamicArray* da = da_create_with_ascending_ints(10);

	int test_score = 0;
	test_score += da_remove_last(da)==9 ? 1 : 0;
	test_score += da_remove_at(da, 8)==8 ? 10 : 0;
	test_score += da_remove_at(da, 4)==4 ? 100 : 0;
	test_score += da_remove_at(da, 0)==0 ? 1000 : 0;
	test_score += da_get(da, 0)==1 ? 10000 : 0;
	test_score += da_get(da, 3)==5 ? 100000 : 0;
	test_score += da_get(da, 5)==7 ? 1000000 : 0;
	test_score += da->size==6 ? 20000000 : 0;

	while(da->size > 0)
		da_remove_last(da);
    
    da_destroy(da);
    return test_score;
}

int test_remove() {
    DynamicArray* da = da_create_with_ascending_ints(10);

	int test_score = 0;
	test_score += da_remove(da, 9) ? 1 : 0;
	test_score += da_remove(da, 5) ? 10 : 0;
	test_score += da_remove(da, 0) ? 100 : 0;
	test_score += da_get(da, 0)==1 ? 1000 : 0;
	test_score += da_get(da, 4)==6 ? 10000 : 0;
	test_score += da_get(da, 6)==8 ? 100000 : 0;
	test_score += da->size==7 ? 2000000 : 0;
    
    da_destroy(da);
    return test_score;
}

int test_set() {
    DynamicArray* da = da_create_with_ascending_ints(10);

	int test_score = 0;
	test_score += da_set(da, 0, 0)==0 ? 1 : 0;
	test_score += da_set(da, 50, 5)==5 ? 10 : 0;
	test_score += da_set(da, 90, 9)==9 ? 100 : 0;
	test_score += da_get(da, 0)==0 ? 1000 : 0;
	test_score += da_get(da, 5)==50 ? 10000 : 0;
	test_score += da_get(da, 9)==90 ? 200000 : 0;
    
    da_destroy(da);
    return test_score;
}

int test_index_of() {
    DynamicArray* da = da_create_with_ascending_ints(10);

	int test_score = 0;
	test_score += da_index_of(da, 0)==0 ? 1 : 0;
	test_score += da_index_of(da, 5)==5 ? 10 : 0;
	test_score += da_index_of(da, 9)==9 ? 100 : 0;
	test_score += da_index_of(da, 10)==-1 ? 2000 : 0;
    
    da_destroy(da);
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
