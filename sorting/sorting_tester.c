#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
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
		if(arr[i] > arr[i+1])
			return false;
	
	return true;
}

static void print_array(int* arr, int length) {
	for(int i = 0; i < length; i++)
		printf("%d ", arr[i]);
	printf("\n");
}

int test_insertion_sort(int length) {
	int* arr = get_randomized_int_array(length);

	insertion_sort(arr, length);

	bool result = is_sorted(arr, length);
	Free(arr);
	return result;
}

int test_bubble_sort(int length) {
	int* arr = get_randomized_int_array(length);

	bubble_sort(arr, length);
	
	bool result = is_sorted(arr, length);
	Free(arr);
	return result;
}

int test_selection_sort(int length) {
	int* arr = get_randomized_int_array(length);

	selection_sort(arr, length);
	
	bool result = is_sorted(arr, length);
	Free(arr);
	return result;
}

int test_merge_sort(int length) {
	int* arr = get_randomized_int_array(length);

	merge_sort(arr, length);
	
	bool result = is_sorted(arr, length);
	Free(arr);
	return result;
}

int test_heap_sort(int length) {
	int* arr = get_randomized_int_array(length);

	heap_sort(arr, length);
	
	bool result = is_sorted(arr, length);
	Free(arr);
	return result;
}

int test_quick_sort(int length) {
	int* arr = get_randomized_int_array(length);

	quick_sort(arr, 0, length-1);
	
	bool result = is_sorted(arr, length);
	Free(arr);
	return result;
}

int main(void) {
	srand(time(NULL));
	int length = ARRAY_LENGTH;

	printf("Insertion Sort: %d\n", test_insertion_sort(length));
	printf("Bubble Sort: %d\n", test_bubble_sort(length));
	printf("Selection Sort: %d\n", test_selection_sort(length));
	printf("Merge Sort: %d\n", test_merge_sort(length));
	printf("Heap Sort: %d\n", test_heap_sort(length));
	printf("Quick Sort: %d\n", test_quick_sort(length));

	return 0;
}