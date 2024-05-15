#include "heap.h"
#include "../../wrapper_functions.h"


/* ---------------- Compare Function ---------------- */

static int compare(data_type e1, data_type e2)
{
    if (e1 > e2) return 1;
    if (e1 < e2) return -1;
    return 0;
}

/* ---------------- Helper Functions ---------------- */

#define LEFT_CHILD_POS(i)	(2 * (i) + 1)
#define RIGHT_CHILD_POS(i)	(2 * (i) + 2)
#define PARENT_POS(i)		(((i) - 1) / 2)


static void resize(Heap* h) {
	h->capacity *= 2;
	h->arr = Realloc(h->arr, sizeof(data_type) * h->capacity);
}

static void swap(data_type* pos1, data_type* pos2)
{
	data_type temp = *pos1;
	*pos1 = *pos2;
	*pos2 = temp;
}

/* ---------------- Header Implementation ---------------- */

Heap* heap_create(int initial_capacity)
{
	Heap* h =  (Heap*) Malloc(sizeof(Heap));

	h->arr = (data_type*) Malloc(sizeof(data_type) * initial_capacity);
	h->size = 0;
	h->capacity = initial_capacity;

	return h;
}

void heap_insert(Heap* h, data_type data)
{
	if (h->size >= h->capacity)
		resize(h);

	int i = h->size++;
	h->arr[i] = data;

	// Move new node upwards until root or greater parent is reached
	for (; i > 0 && compare(data, h->arr[PARENT_POS(i)]) > 0; i = PARENT_POS(i))
		swap(h->arr + i, h->arr + PARENT_POS(i));
}

data_type heap_extract(Heap* h)
{
	data_type data = h->arr[0];

	// Move last node to root
	h->arr[0] = h->arr[--h->size];

	// Move new root downwards until bottom or lesser children are reached
	for (int i = 0; LEFT_CHILD_POS(i) < h->size; )
	{
		int child_to_swap = LEFT_CHILD_POS(i) == h->size - 1 || compare(h->arr[LEFT_CHILD_POS(i)], h->arr[RIGHT_CHILD_POS(i)]) >= 0 ?
							LEFT_CHILD_POS(i) : RIGHT_CHILD_POS(i);

		if (compare(h->arr[i], h->arr[child_to_swap]) >= 0)
			break;

		swap(h->arr + i, h->arr + child_to_swap);
		i = child_to_swap;
	}

	return data;
}

void heap_destroy(Heap* h)
{
	Free(h->arr);
	Free(h);
}
