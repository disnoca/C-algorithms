#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "my_malloc.h"

extern void* heap_start;

void fill_with_trash(void* ptr, size_t size)
{
	for(size_t i = 0; i < size; i++)
		((char*) ptr)[i] = rand();
}

void test_malloc(void)
{
	srand(time(NULL));

    size_t sizes[1000];
	void* ptrs[1000];

	for(int i = 0; i < 1000; i++) {
		sizes[i] = rand() % 131072 + 1;
		ptrs[i] = slab_malloc(sizes[i]);
		fill_with_trash(ptrs[i], sizes[i]);
	}

	for(int i = 0; i < 500; i++) {
		slab_free(ptrs[i], sizes[i]);
	}

	for(int i = 0; i < 500; i++) {
		sizes[i] = rand() % 131072 + 1;
		ptrs[i] = slab_malloc(sizes[i]);
		fill_with_trash(ptrs[i], sizes[i]);
	}

	for(int i = 0; i < 1000; i++) {
		slab_free(ptrs[i], sizes[i]);
	}

	//print_heap();
	slab_print();
	fprintf(stderr, "test malloc finished\n");
}

void test_add_hole(void)
{
	slab_free(slab_malloc(1), 1);

	bb_add_hole(heap_start + (1 << 29), heap_start + (1 << 30));
	bb_add_hole(heap_start + (1 << 26), heap_start + (1 << 28));
	bb_add_hole(heap_start, heap_start + 1);

	//print_heap();
}

int main(void)
{
	slab_init();

	for(int i = 0; i < 1000; i++)
		test_malloc();
	
	//test_add_hole();
    return 0;
}
