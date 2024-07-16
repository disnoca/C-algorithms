#include "heap.h"
#include "../../wrapper_functions.h"


/* ---------------- Helper Functions ---------------- */

#define LEFT_CHILD_POS(i)	(2 * (i) + 1)
#define RIGHT_CHILD_POS(i)	(2 * (i) + 2)
#define PARENT_POS(i)		(((i) - 1) / 2)


static void resize(Heap* h)
{
	h->capacity *= 2;
	h->arr = Realloc(h->arr, h->capacity * sizeof(void*));
}

static void swap(void** pos1, void** pos2)
{
	void* temp = *pos1;
	*pos1 = *pos2;
	*pos2 = temp;
}


/* ---------------- Header Implementation ---------------- */

void heap_init(Heap* heap, size_t initial_capacity, int(*compare)(void*, void*))
{
	heap->arr = (void**) Malloc(sizeof(void*) * initial_capacity);
	heap->size = 0;
	heap->capacity = initial_capacity;
	heap->compare = compare;

}

Heap* heap_create(size_t initial_capacity, int(*compare)(void*, void*))
{
	Heap* h = (Heap*) Malloc(sizeof(Heap));
	heap_init(h, initial_capacity, compare);
	return h;
}

void heap_insert(Heap* h, void* data)
{
	if (h->size >= h->capacity)
		resize(h);

	size_t i = h->size++;
	h->arr[i] = data;

	// Move new node upwards until root or greater parent is reached
	for (; i > 0 && h->compare(data, h->arr[PARENT_POS(i)]) > 0; i = PARENT_POS(i))
		swap(h->arr + i, h->arr + PARENT_POS(i));
}

void* heap_extract(Heap* h)
{
	void* data = h->arr[0];

	// Move last node to root
	h->arr[0] = h->arr[--h->size];

	// Move new root downwards until bottom or lesser children are reached
	for (size_t i = 0; LEFT_CHILD_POS(i) < h->size; )
	{
		int child_to_swap = LEFT_CHILD_POS(i) == h->size - 1 || h->compare(h->arr[LEFT_CHILD_POS(i)], h->arr[RIGHT_CHILD_POS(i)]) >= 0 ?
							LEFT_CHILD_POS(i) : RIGHT_CHILD_POS(i);

		if (h->compare(h->arr[i], h->arr[child_to_swap]) >= 0)
			break;

		swap(h->arr + i, h->arr + child_to_swap);
		i = child_to_swap;
	}

	return data;
}

void heap_free(Heap* heap)
{
	Free(heap->arr);
}

void heap_destroy(Heap* h)
{
	heap_free(h);
	Free(h);
}
