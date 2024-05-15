#include <assert.h>
#include <stdio.h>
#include "heap.h"
#include "../../wrapper_functions.h"


#define LEFT_CHILD_POS(i)	(2 * (i) + 1)
#define RIGHT_CHILD_POS(i)	(2 * (i) + 2)
#define PARENT_POS(i)		(((i) - 1) / 2)


__attribute__((unused)) static void print_heap_contents(Heap* h) {
    for(int i = 0; i < h->size; i++)
        printf("%d ", h->arr[i]);
    printf("\n");
}


static void heap_sanity_test(Heap* h)
{
	for (int i = 0; LEFT_CHILD_POS(i) < h->size; i++)
	{
		assert(h->arr[i] >= h->arr[LEFT_CHILD_POS(i)]);
		if (RIGHT_CHILD_POS(i) < h->size)
			assert(h->arr[i] >= h->arr[RIGHT_CHILD_POS(i)]);
	}
}


static void test_insert()
{
	Heap* h = heap_create(5);
    for (int i = 0; i < 11; i++)
		heap_insert(h, i);

	heap_insert(h, -1);
	heap_insert(h, 20);
	heap_insert(h, 5);

	assert(h->arr[0] == 20);
	assert(h->size == 14);

	heap_sanity_test(h);
    heap_destroy(h);
}

static void test_extract()
{
	Heap* h = heap_create(5);
    for (int i = 0; i < 11; i++)
		heap_insert(h, i);

	heap_insert(h, 20);
	assert(heap_extract(h) == 20);

	heap_sanity_test(h);

	for (int i = 10; i >= 0; i--)
		assert(heap_extract(h) == i);

    heap_destroy(h);
}


int main() {
    test_insert();
	test_extract();

	assert(malloc_calls == free_calls);

    printf("All tests passed.\n");
    return 0;
}
