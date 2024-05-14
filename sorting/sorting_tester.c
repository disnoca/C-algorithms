#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>
#include "sorting.h"
#include "../wrapper_functions.h"

#define ARRAY_LENGTH 10000

static int* get_randomized_int_array(int length) {
	int* arr = Malloc(sizeof(int) * length);

	for(int i = 0; i < length; i++)
		arr[i] = rand();

	return arr;
}

static bool is_sorted(int* arr, int length) {
	for(int i = 0; i < length-1; i++)
		if (arr[i] > arr[i+1])
			return false;
	
	return true;
}

__attribute__((unused)) static void print_array(int* arr, int length) {
	for(int i = 0; i < length; i++)
		printf("%d ", arr[i]);
	printf("\n");
}

static void test_insertion_sort(int length) {
	int* arr = get_randomized_int_array(length);

	insertion_sort(arr, length);

	assert(is_sorted(arr, length));

	Free(arr);
}

static void test_bubble_sort(int length) {
	int* arr = get_randomized_int_array(length);

	bubble_sort(arr, length);

	assert(is_sorted(arr, length));

	Free(arr);
}

static void test_selection_sort(int length) {
	int* arr = get_randomized_int_array(length);

	selection_sort(arr, length);
	
	assert(is_sorted(arr, length));

	Free(arr);
}

static void test_merge_sort(int length) {
	int* arr = get_randomized_int_array(length);

	merge_sort(arr, length);
	
	assert(is_sorted(arr, length));

	Free(arr);
}

static void test_heap_sort(int length) {
	int* arr = get_randomized_int_array(length);

	heap_sort(arr, length);
	
	assert(is_sorted(arr, length));

	Free(arr);
}

static void test_quick_sort(int length) {
	int* arr = get_randomized_int_array(length);

	quick_sort(arr, 0, length-1);
	
	assert(is_sorted(arr, length));

	Free(arr);
}

static void test_intro_sort(int length) {
	int* arr = get_randomized_int_array(length);

	intro_sort(arr, length);
	
	assert(is_sorted(arr, length));

	Free(arr);
}

int main(void) {
	srand(time(NULL));
	int length = ARRAY_LENGTH;

	test_insertion_sort(length);
	test_bubble_sort(length);
	test_selection_sort(length);
	test_merge_sort(length);
	test_heap_sort(length);
	test_quick_sort(length);
	test_intro_sort(length);

	printf("All tests passed.\n");
	return 0;
}
