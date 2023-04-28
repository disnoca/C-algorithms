#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "sorting.h"
#include "../wrapper_functions.h"

#define ARRAY_LENGTH 100

static int* get_randomized_int_array(int length) {
	int* arr = Calloc(sizeof(int), length);

	for(int i = 0; i < length; i++)
		arr[i] = rand();

	return arr;
}

static bool is_sorted(int* arr, int length) {
	for(int i = 0; i < length-1; i++)
		if(arr[i] > arr[i+1])
			return false;

	return true;
}

int test_insertion_sort(int length) {
	int* arr = get_randomized_int_array(length);

	insertion_sort(arr, length);

	return is_sorted(arr, length);
}

int test_bubble_sort(int length) {
	int* arr = get_randomized_int_array(length);

	bubble_sort(arr, length);
	
	return is_sorted(arr, length);
}

int main(void) {
	srand(time(NULL));
	int length = ARRAY_LENGTH;

	printf("Insertion Sort: %d\n", test_insertion_sort(length));
	printf("Bubble Sort: %d\n", test_bubble_sort(length));

	return 0;
}