#include "../sorting.h"
#include "../../wrapper_functions.h"

static void merge(int* arr, int* left_arr, int left_length, int* right_arr, int right_length) {		
	int i = 0, j = 0, k = 0;
	
	while(i < left_length && j < right_length) {
		if(left_arr[i] < right_arr[j]) 
			arr[k++] = left_arr[i++];
		else
			arr[k++] = right_arr[j++];
	}
	
	while(i < left_length)
		arr[k++] = left_arr[i++];
	
	while(j < right_length)
		arr[k++] = right_arr[j++];
}

void merge_sort(int* arr, int length) {
	if(length < 2) return;
	
	int mid_pos = length/2;

	int left_length = mid_pos;
	int* left_arr = Calloc(sizeof(int), left_length);
	int right_length = length - mid_pos;
	int* right_arr = Calloc(sizeof(int), right_length);
	
	for(int i = 0; i < mid_pos; i++)
		left_arr[i] = arr[i];
	
	for(int i = mid_pos; i < length; i++)
		right_arr[i-mid_pos] = arr[i];
	
	merge_sort(left_arr, left_length);
	merge_sort(right_arr, right_length);
	
	merge(arr, left_arr, left_length, right_arr, right_length);

	Free(left_arr);
	Free(right_arr);
}