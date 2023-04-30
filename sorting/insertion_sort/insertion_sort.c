#include "../sorting.h"
#include "../helper_functions.h"

void insertion_sort(int* arr, int length) {
	for(int i = 1; i < length; i++)
		for(int j = i; j > 0 && arr[j] < arr[j-1]; j--)
			swap(arr, j, j-1);
}