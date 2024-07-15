/**
 * Quick Sort sorting algorithm implementation.
 * 
 * @author Samuel Pires
*/

#include <stdbool.h>
#include "sorting.h"
#include "helper_functions.h"

static void prepare_pivot(int* arr, int start_pos, int end_pos)
{
	int mid_pos = (start_pos + end_pos) / 2;

	if (arr[mid_pos] < arr[start_pos])
		swap(arr+start_pos, arr+mid_pos);
	if (arr[end_pos] < arr[start_pos])
		swap(arr+start_pos, arr+end_pos);
	if (arr[mid_pos] < arr[end_pos])
		swap(arr+mid_pos, arr+end_pos);
}

int partition(int* arr, int start_pos, int end_pos)
{
	prepare_pivot(arr, start_pos, end_pos);
	int pivot = arr[end_pos];

    int i = start_pos;

    for (int j = start_pos; j < end_pos; j++)
        if (arr[j] < pivot)
			swap(arr+i++, arr+j);

    swap(arr+i, arr+end_pos);

    return i;
}

void quick_sort(int* arr, int start_pos, int end_pos)
{
	if (start_pos >= end_pos) return;
	
	int partition_pos = partition(arr, start_pos, end_pos);
	
	quick_sort(arr, start_pos, partition_pos-1);
	quick_sort(arr, partition_pos+1, end_pos);
}
