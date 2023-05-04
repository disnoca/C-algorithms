#include "sorting.h"

static void merge(int* arr, int start_pos, int mid_pos, int end_pos) {
	int half_start_pos = mid_pos + 1;

    if (arr[mid_pos] <= arr[half_start_pos])
        return;

    while (start_pos <= mid_pos && half_start_pos <= end_pos) {
        if (arr[start_pos] <= arr[half_start_pos]) {
            start_pos++;
			continue;
		}

        for(int i = start_pos + 1; i < half_start_pos; i++)
            arr[i+1] = arr[i];
		
        arr[start_pos] = arr[half_start_pos];

        start_pos++;
        mid_pos++;
        half_start_pos++;
    }
}

void in_place_merge_sort(int* arr, int start_pos, int end_pos) {
	if (start_pos >= end_pos)
		return;

    int mid_pos = (start_pos + end_pos) / 2;

    in_place_merge_sort(arr, start_pos, mid_pos);
    in_place_merge_sort(arr, mid_pos + 1, end_pos);

    merge(arr, start_pos, mid_pos, end_pos);
}