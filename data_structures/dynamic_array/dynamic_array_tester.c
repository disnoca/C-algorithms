#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "dynamic_array.h"
#include "../../wrapper_functions.h"

__attribute__((unused)) static void print_da_contents(DynamicArray* da) {
    for(int i = 0; i < da->size; i++)
        printf("%d ", da->arr[i]);
    printf("\n");
}

static DynamicArray* da_create_with_ascending_ints(int size) {
    DynamicArray* da = da_create(size);
    for(int i = 0; i < size; i++)
        da_add(da, i);

    return da;
}

static void test_add() {
    DynamicArray* da = da_create(5);
    for(int i = 0; i < 11; i++)
		da_add(da, i);

	assert(da->arr[0]==0);
	assert(da->arr[5]==5);
	assert(da->arr[10]==10);
	assert(da->size==11);
	assert(da->capacity==20);
    
    da_destroy(da);
}

static void test_add_at() {
    DynamicArray* da = da_create_with_ascending_ints(10);
	da_add_at(da, 10, 10);
	da_add_at(da, 5, 5);
	da_add_at(da, 0, 0);

	assert(da->arr[0]==0);
	assert(da->arr[1]==0);
	assert(da->arr[6]==5);
	assert(da->arr[7]==5);
	assert(da->arr[11]==9);
	assert(da->arr[12]==10);
	assert(da->size==13);
	assert(da->capacity==20);
    
    da_destroy(da);
}

static void test_get() {
    DynamicArray* da = da_create_with_ascending_ints(10);

	assert(da_get(da, 0)==0);
	assert(da_get(da, 5)==5);
	assert(da_get(da, 9)==9);
    
    da_destroy(da);
}

static void test_remove_at() {
    DynamicArray* da = da_create_with_ascending_ints(10);

	assert(da_remove_last(da)==9);
	assert(da_remove_at(da, 8)==8);
	assert(da_remove_at(da, 4)==4);
	assert(da_remove_at(da, 0)==0);
	assert(da_get(da, 0)==1);
	assert(da_get(da, 3)==5);
	assert(da_get(da, 5)==7);
	assert(da->size==6);

	while (da->size > 0)
		da_remove_last(da);
    
    da_destroy(da);
}

static void test_remove() {
    DynamicArray* da = da_create_with_ascending_ints(10);

	assert(da_remove(da, 9));
	assert(da_remove(da, 5));
	assert(da_remove(da, 0));
	assert(da_get(da, 0)==1);
	assert(da_get(da, 4)==6);
	assert(da_get(da, 6)==8);
	assert(da->size==7);
    
    da_destroy(da);
}

static void test_set() {
    DynamicArray* da = da_create_with_ascending_ints(10);

	assert(da_set(da, 0, 0)==0);
	assert(da_set(da, 50, 5)==5);
	assert(da_set(da, 90, 9)==9);
	assert(da_get(da, 0)==0);
	assert(da_get(da, 5)==50);
	assert(da_get(da, 9)==90);
    
    da_destroy(da);
}

static void test_index_of() {
    DynamicArray* da = da_create_with_ascending_ints(10);

	assert(da_index_of(da, 0)==0);
	assert(da_index_of(da, 5)==5);
	assert(da_index_of(da, 9)==9);
	assert(da_index_of(da, 10)==-1);
    
    da_destroy(da);
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
