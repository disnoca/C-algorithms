/**
 * Quicksort sorting algorithm implementation.
 * 
 * @author Samuel Pires
*/

#include <stdbool.h>
#include "sorting.h"
#include "helper_functions.h"

static bool is_between(int value, int bound1, int bound2) {
	return (value > bound1 && value < bound2) || (value > bound2 && value < bound1);
}

static void prepare_pivot(int* arr, int start_pos, int end_pos) {
	int mid_pos = end_pos / 2;

	int start_candidate = arr[start_pos];
	int mid_candidate = arr[mid_pos];
	int end_candidate = arr[end_pos];

	if(is_between(start_candidate, mid_candidate, end_candidate))
		swap(arr, start_pos, end_pos);
	else if(is_between(mid_candidate, start_candidate, end_candidate))
		swap(arr, mid_pos, end_pos);
}

int partition(int* arr, int start_pos, int end_pos) {
	prepare_pivot(arr, start_pos, end_pos);
	int pivot = arr[end_pos];

    int i = start_pos;

    for (int j = start_pos; j < end_pos; j++)
        if (arr[j] < pivot)
			swap(arr, i++, j);

    swap(arr, i, end_pos);

    return i;
}

void quicksort(int* arr, int start_pos, int end_pos) {
	if(start_pos >= end_pos) return;
	
	int partition_pos = partition(arr, start_pos, end_pos);
	
	quicksort(arr, start_pos, partition_pos-1);
	quicksort(arr, partition_pos+1, end_pos);
}