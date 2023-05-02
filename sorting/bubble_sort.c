#include "sorting.h"
#include "helper_functions.h"

void bubble_sort(int* arr, int length) {
	for(int i = 0; i < length; i++)
		for(int j = length-1; j > i; j--)
			if(arr[j] < arr[j-1])
				swap(arr, j, j-1);
}