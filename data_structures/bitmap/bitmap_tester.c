#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "bitmap.h"


void test_bitmap_basic() {
	uint8_t* bitmap = malloc(4096);
	size_t nfound;

	assert(bitmap_alloc((bitmap_t*)bitmap, 4096, 3, &nfound) == 0);
	assert(nfound == 3);
	assert(bitmap_alloc((bitmap_t*)bitmap, 4096, 8, &nfound) == 3);
	assert(nfound == 8);
	assert(bitmap_alloc((bitmap_t*)bitmap, 4096, 15, &nfound) == 11);
	assert(nfound == 15);

	assert(bitmap[0] == 0xFF);
	assert(bitmap[1] == 0xFF);
	assert(bitmap[2] == 0xFF);
	assert(bitmap[3] == 0x3);
	assert(bitmap[4] == 0);

	bitmap_free((bitmap_t*)bitmap, 5, 4);

	assert(bitmap[0] == 0x1F);
	assert(bitmap[1] == 0xFE);

	assert(bitmap_alloc((bitmap_t*)bitmap, 4096, 3, &nfound) == 5);
	assert(nfound == 3);

	assert(bitmap[0] == 0xFF);
	assert(bitmap[1] == 0xFE);

	assert(bitmap_alloc((bitmap_t*)bitmap, 4096, 32743, &nfound) == 0);
	assert(nfound == 32742);

	assert(bitmap_alloc((bitmap_t*)bitmap, 4096, 32742, &nfound) == 26);
	assert(bitmap[1] == 0xFE);

	for (int i = 2; i < 4096; i++)
		assert(bitmap[i] == 0xFF);
}


int main() {
	test_bitmap_basic();

	printf("All tests passed!\n");
	return 0;
}
