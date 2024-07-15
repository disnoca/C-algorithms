/**
 * Binary search algorithm implementation.
 *
 * @author Samuel Pires 
*/

#include "searching.h"

int binary_search(int* arr, int length, int target)
{
	int left = 0, right = length-1;

	while (left < right) {
		int mid = (left + right)/2;
		if (arr[mid] < target)
			left = mid + 1;
		else if (arr[mid] > target)
			right = mid - 1;
		else 
			return mid;
	}

	return -1;
}
