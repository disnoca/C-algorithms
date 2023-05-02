#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "../wrapper_functions.h"
#include "search.h"

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

bool test_linear_search(int length) {
	int* arr = get_randomized_int_array(length);

	int target_pos = rand() % ARRAY_LENGTH;
	int result = linear_search(arr, length, arr[target_pos]);

	Free(arr);
	return result == target_pos;
}

bool test_binary_search(int length) {
	int* arr = get_sorted_int_array(length);

	int target_pos = rand() % ARRAY_LENGTH;
	int result = linear_search(arr, length, arr[target_pos]);

	Free(arr);
	return result == target_pos;
}

int main(void) {
	srand(time(NULL));
	int length = ARRAY_LENGTH;

	printf("Linear Search: %d\n", test_linear_search(length));
	printf("Binary Search: %d\n", test_binary_search(length));

	return 0;
}