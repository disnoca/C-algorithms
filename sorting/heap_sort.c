/**
 * Heap Sort sorting algorithm implementation.
 * 
 * @author Samuel Pires
*/

#include "sorting.h"
#include "helper_functions.h"

static void heapify(int* arr, int length, int root_pos) {
	int largest_pos = root_pos;
	int left_pos = 2*(root_pos+1)-1;	// left child index = 2*(i+1)-1 <=> i+1;
	int right_pos = 2*(root_pos+1);		// right child index = 2*(i+1)+1-1 <=> i+2;		parent index = (i+1)/2-1 <=> i/2-1
	
	if(left_pos < length && arr[left_pos] > arr[largest_pos]) 
		largest_pos = left_pos;
	if(right_pos < length && arr[right_pos] > arr[largest_pos]) 
		largest_pos = right_pos;
	
	if(largest_pos != root_pos) {
		swap(arr+root_pos, arr+largest_pos);
		heapify(arr, length, largest_pos);
	}
}

void heap_sort(int* arr, int length) {
	for (int i = length/2-1; i >= 0; i--)		// build max heap by heapifying all non-leaf nodes 
        heapify(arr, length, i);
	
	int unsorted_size = length;
	while(unsorted_size-- > 1) {				// add the root to the sorted partition
		swap(arr, arr+unsorted_size);
		heapify(arr, unsorted_size, 0);
	}
}