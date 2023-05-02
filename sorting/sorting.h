#ifndef SORTING
#define SORTING

/* Best: 	n
 * Average: n^2
 * Worst: 	n^2
 * Memory: 	1
 * Stable: 	Yes
*/
void insertion_sort(int* arr, int length);

/* Best: 	n
 * Average: n^2
 * Worst: 	n^2
 * Memory: 	1
 * Stable: 	Yes
*/
void bubble_sort(int* arr, int length);

/* Best: 	n^2
 * Average: n^2
 * Worst: 	n^2
 * Memory: 	1
 * Stable: 	Yes
*/
void selection_sort(int* arr, int length);

/* Best: 	n*log(n)
 * Average: n*log(n)
 * Worst: 	n*log(n)
 * Memory: 	n
 * Stable: 	Yes
*/
void merge_sort(int* arr, int length);

/* Best: 	n*log(n)
 * Average: n*log(n)
 * Worst: 	n*log(n)
 * Memory: 	1
 * Stable: 	No
*/
void heap_sort(int* arr, int length);

/* Best: 	n*log(n)
 * Average: n*log(n)
 * Worst: 	n^2
 * Memory: 	log(n)
 * Stable: 	No
*/
void quick_sort(int* arr, int start_pos, int end_pos);

#endif