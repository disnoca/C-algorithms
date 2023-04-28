#include "../sorting.h"

void insertion_sort(int* arr, int length) {
	for(int i = 1; i < length; i++)
		for(int j = i; j > 0 && arr[j] < arr[j-1]; j--) {
			int temp = arr[j-1];
			arr[j-1] = arr[j];
			arr[j] = temp;
		}
}