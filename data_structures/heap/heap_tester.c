#include <assert.h>
#include <stdio.h>
#include "heap.h"
#include "../../wrapper_functions.h"


int values[1024];
int curr_val;

#define REF_OF(x)   ({ values[curr_val] = (x); values + curr_val++; })
#define DEREF(p)    (*((int*)(p)))


#define LEFT_CHILD_POS(i)	(2 * (i) + 1)
#define RIGHT_CHILD_POS(i)	(2 * (i) + 2)
#define PARENT_POS(i)		(((i) - 1) / 2)


static int compare(void* arg1, void* arg2)
{
    if (DEREF(arg1) > DEREF(arg2)) return 1;
    if (DEREF(arg1) < DEREF(arg2)) return -1;
    return 0;
}


__attribute__((unused)) static void print_heap_contents(Heap* h)
{
    for(size_t i = 0; i < h->size; i++)
        printf("%d ", DEREF(h->arr[i]));
    printf("\n");
}


static void heap_sanity_test(Heap* h)
{
	for (size_t i = 0; LEFT_CHILD_POS(i) < h->size; i++)
	{
		assert(DEREF(h->arr[i]) >= DEREF(h->arr[LEFT_CHILD_POS(i)]));
		if (RIGHT_CHILD_POS(i) < h->size)
			assert(DEREF(h->arr[i]) >= DEREF(h->arr[RIGHT_CHILD_POS(i)]));
	}
}


static void test_insert()
{
	Heap* h = heap_create(5, compare);
    for (int i = 0; i < 11; i++)
		heap_insert(h, REF_OF(i));

	heap_insert(h, REF_OF(-1));
	heap_insert(h, REF_OF(20));
	heap_insert(h, REF_OF(5));

	assert(DEREF(h->arr[0]) == 20);
	assert(h->size == 14);

	heap_sanity_test(h);
    heap_destroy(h);
}

static void test_extract()
{
	Heap* h = heap_create(5, compare);
    for (int i = 0; i < 11; i++)
		heap_insert(h, REF_OF(i));

	heap_insert(h, REF_OF(20));
	assert(DEREF(heap_extract(h)) == 20);

	heap_sanity_test(h);

	for (int i = 10; i >= 0; i--)
		assert(DEREF(heap_extract(h)) == i);

    heap_destroy(h);
}


int main() {
    test_insert();
	test_extract();

	assert(malloc_calls == free_calls);

    printf("All tests passed.\n");
    return 0;
}
