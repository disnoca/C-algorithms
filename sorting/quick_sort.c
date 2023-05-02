#include "sorting.h"
#include "helper_functions.h"

static int partition(int* arr, int start_pos, int end_pos) {
	int pivot = arr[end_pos];
    int i = start_pos;

    for (int j = start_pos; j < end_pos; j++)
        if (arr[j] < pivot)
			swap(arr, i++, j);

    swap(arr, i, end_pos);

    return i;
}

void quick_sort(int* arr, int start_pos, int end_pos) {
	if(start_pos >= end_pos) return;
	
	int partition_pos = partition(arr, start_pos, end_pos);
	
	quick_sort(arr, start_pos, partition_pos-1);
	quick_sort(arr, partition_pos+1, end_pos);
}