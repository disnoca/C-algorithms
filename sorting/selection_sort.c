/**
 * Selection Sort sorting algorithm implementation.
 * 
 * @author Samuel Pires
*/

#include "sorting.h"
#include "helper_functions.h"

void selection_sort(int* arr, int length) {
	for(int i = 0; i < length; i++) {
		int current_min_pos = i;

		for(int j = i; j < length; j++) 
			if(arr[j] < arr[current_min_pos]) 
				current_min_pos = j;

		if(i != current_min_pos) 
			swap(arr, i, current_min_pos);
	}
}