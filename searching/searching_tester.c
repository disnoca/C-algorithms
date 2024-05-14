#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>
#include "../wrapper_functions.h"
#include "searching.h"

#define ARRAY_LENGTH 10000

static int* get_randomized_int_array(int length) {
	int* arr = Malloc(sizeof(int) * length);

	for(int i = 0; i < length; i++)
		arr[i] = rand();

	return arr;
}

static int* get_sorted_int_array(int length) {
	int* arr = Malloc(sizeof(int) * length);

	for(int i = 0; i < length; i++)
		arr[i] = i;

	return arr;
}

static void test_linear_search(int length) {
	int* arr = get_randomized_int_array(length);

	int target_pos = rand() % ARRAY_LENGTH;
	assert(linear_search(arr, length, arr[target_pos]));

	Free(arr);
}

static void test_binary_search(int length) {
	int* arr = get_sorted_int_array(length);

	int target_pos = rand() % ARRAY_LENGTH;
	assert(linear_search(arr, length, arr[target_pos]));

	Free(arr);
}

int main(void) {
	srand(time(NULL));
	int length = ARRAY_LENGTH;

	test_linear_search(length);
	test_binary_search(length);

	printf("All tests passed.\n");
	return 0;
}
