#include "../sorting.h"

void bubble_sort(int* arr, int length) {
	for(int i = 0; i < length; i++)
		for(int j = length-1; j > i; j--)
			if(arr[j] < arr[j-1]) {
				int temp = arr[j-1];
				arr[j-1] = arr[j];
				arr[j] = temp;
			}
}