#include "helper_functions.h"

void swap(int* pos1, int* pos2) {
	int temp = *pos1;
	*pos1 = *pos2;
	*pos2 = temp;
}