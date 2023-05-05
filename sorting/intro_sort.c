/**
 * Intro Sort sorting algorithm implementation.
 * 
 * @author Samuel Pires
*/

#include <math.h>
#include "sorting.h"

static void sort(int* arr, int length, int max_depth) {
	if(length < 16)
		insertion_sort(arr, length);
	else if(max_depth == 0)
		heap_sort(arr, length);
	else {
		int pivot_pos = partition(arr, 0, length-1);
		sort(arr, pivot_pos, max_depth-1);
		sort(arr+pivot_pos+1, length-pivot_pos-1, max_depth-1);
	}
}

void intro_sort(int* arr, int length) {
	int max_depth = 2 * log2(length);
	sort(arr, length, max_depth);
}