#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


#define ROUND_UP(n,m)		({__typeof__ (n) _n = (n); __typeof__ (m) _m = (m); _n % _m == 0 ? _n : (_n + (_m - (_n % _m))); })	
#define ROUND_DOWN(n,m)		({__typeof__ (n) _n = (n); (_n - (_n % (m))); })


/* Exponent of block size when initializing heap */
#define BLOCK_INIT_EXP			30		/* 1GB */
#define BLOCK_INIT_SIZE			(1 << BLOCK_INIT_EXP)

#define BLOCK_MAX_EXP			30		/* 1GB */
#define BLOCK_MAX_SIZE			(1 << BLOCK_MAX_EXP)

#define BLOCK_MIN_EXP			4		/* 16 bytes */
#define BLOCK_MIN_SIZE			(1 << BLOCK_MIN_EXP)

#define LEVEL_MAX				(BLOCK_MAX_EXP - BLOCK_MIN_EXP)
#define NUM_LEVELS				(LEVEL_MAX + 1)

#define LEVEL_TO_EXP(l)			(BLOCK_MAX_EXP - (l))
#define EXP_TO_LEVEL(e)			(BLOCK_MAX_EXP - (e))
#define EXP_TO_SIZE(e)			(1 << (e))

#define BLOCK_HIGH_HALF(b,e)	(((unsigned char*)(b)) + EXP_TO_SIZE(e) / 2)
#define BLOCK_HIGH_BUDDY(b,e)	(((unsigned char*)(b)) + EXP_TO_SIZE(e))
#define BLOCK_LOW_BUDDY(b,e)	(((unsigned char*)(b)) - EXP_TO_SIZE(e))

#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define MIN(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })


typedef struct list_t {
	struct list_t *prev, *next;
} list_t;


static list_t lists[NUM_LEVELS];

void* heap_start;


static void heap_init(void);
static void block_split(list_t* block, int exp, int target_exp);
static list_t* block_coalesce(list_t* block, int* exp);
static list_t* block_buddy(list_t* block, int exp);

static void list_enqueue(list_t* block, int exp);
static list_t* list_dequeue(int exp);
static list_t* list_remove(list_t* block, int exp);

static int exp2_round_down(int num);
static int exp2_round_up(int num);

static int exp_of_alignment(uintptr_t block_addr);


/* Header Functions */

void* bb_malloc(size_t size)
{
	/* Return NULL on invalid requests */
	if(size == 0 || size > BLOCK_MAX_SIZE)
		return NULL;

	if(heap_start == NULL)
		heap_init();

	int required_block_exp = MAX(exp2_round_up(size), BLOCK_MIN_EXP);
	int curr_block_exp = required_block_exp;
	list_t* block = NULL;

	/*  */
	for(; curr_block_exp <= BLOCK_MAX_EXP; curr_block_exp++) {
		block = list_dequeue(curr_block_exp);
		if(block != NULL)
			break;
	}

	if(block == NULL) {
		// expand
		return NULL;
	}

	block_split(block, curr_block_exp, required_block_exp);

	return block;
}

void bb_free(void* ptr, size_t size)
{
	list_t* block = (list_t*) ptr;
	int exp = MAX(exp2_round_up(size), BLOCK_MIN_EXP);
	block = block_coalesce(block, &exp);
	list_enqueue(block, exp);
}

void bb_add_hole(void* start_addr, void* end_addr)
{
	uintptr_t aligned_start_addr = ROUND_DOWN((uintptr_t) start_addr, BLOCK_MIN_SIZE);
	uintptr_t aligned_end_addr = ROUND_UP((uintptr_t) end_addr, BLOCK_MIN_SIZE);
	uintptr_t curr_addr = aligned_start_addr;
	size_t size = aligned_end_addr - aligned_start_addr;

	while(curr_addr < aligned_end_addr)
	{
		int exp_to_remove = MIN(exp_of_alignment(curr_addr - (uintptr_t) heap_start), MAX(exp2_round_down(size), BLOCK_MIN_EXP));
		list_t* block_to_remove = (list_t*) curr_addr;
		list_t* curr_block = block_to_remove;
		int curr_exp = exp_to_remove;

		for(; curr_exp <= BLOCK_MAX_EXP; curr_exp++)
		{
			/* Check if block exists */
			if(list_remove(curr_block, curr_exp) != NULL)
				break;

			/* If not, set next block to search to one size above */
			curr_block = MIN(curr_block, block_buddy(curr_block, curr_exp));
		}

		/* Split the block until the size to remove is reached,
		   always keeping the half where block to remove is located */
		for(; curr_exp > exp_to_remove; curr_exp--)
		{
			list_t* high_half_block = (list_t*) BLOCK_HIGH_HALF(curr_block, curr_exp);

			if(block_to_remove < high_half_block) {
				list_enqueue(high_half_block, curr_exp - 1);
			} else {
				list_enqueue(curr_block, curr_exp - 1);
				curr_block = high_half_block;
			}
		}

		curr_addr += EXP_TO_SIZE(exp_to_remove);
	}
}

void print_heap(void)
{
	for(int i = 0; i <= LEVEL_MAX; i++)
	{
		list_t* curr_list = lists + i;
		list_t* curr_block = curr_list->next;

		while(curr_block != curr_list) {
			fprintf(stderr, "block: 0x%lx, size: 0x%lx\n", (uintptr_t) curr_block - (uintptr_t) heap_start, (unsigned long) EXP_TO_SIZE(LEVEL_TO_EXP(i)));
			curr_block = curr_block->next;
		}
	}
}



/* Helper Functions */

static void heap_init(void)
{
	/* Mark all lists as empty (point all list to themselves) */
	for(int i = 0; i < NUM_LEVELS; i++) {
		lists[i].next = lists + i;
		lists[i].prev = lists + i;
	}

	heap_start = sbrk(BLOCK_INIT_SIZE);

	/* Add the initial block to the initial level */
	list_enqueue((list_t*) heap_start, BLOCK_INIT_EXP);
}


/**
 * Adds a block to the end of its list.
 * 
 * @param block the block to be added
 * @param exp the exponent of the size of the block
*/
static void list_enqueue(list_t* block, int exp)
{
	list_t* list = lists + EXP_TO_LEVEL(exp);

	block->next = list;
	block->prev = list->prev;
	list->prev->next = block;
	list->prev = block;
}

/**
 * Removes and returns the block at the end of a list.
 * 
 * @param exp the exponent of the size of the list's blocks
 * 
 * @return the removed block or NULL if the list has no blocks
*/
static list_t* list_dequeue(int exp)
{
	list_t* list = lists + EXP_TO_LEVEL(exp);

	/* Return NULL if list is empty */
	if(list->prev == list)
		return NULL;

	list_t* block = list->next;
	list->next = block->next;
	block->next->prev = list;

	return block;
}

/**
 * Removes a specified block from a list.
 * 
 * @param block the block to be removed
 * @param exp the exponent of the size of the block
 * 
 * @return the removed block or NULL if it wasn't in the list
*/
static list_t* list_remove(list_t* block, int exp)
{
	list_t* list = lists + EXP_TO_LEVEL(exp);
	list_t* curr_block = list->next;

	while(curr_block != list)
	{
		if(curr_block == block) {
			curr_block->prev->next = curr_block->next;
			curr_block->next->prev = curr_block->prev;
			return curr_block;
		}
		
		curr_block = curr_block->next;
	}

	return NULL;
}


/**
 * Splits a block until it reaches target_exp 
 * and adds the splits to the appropriate lists.
 * 
 * @param block the block to be split
 * @param exp the exponent of the size of the block
 * @param target_exp the exponent of the desired block size
*/
static void block_split(list_t* block, int exp, int target_exp)
{
	for(; exp > BLOCK_MIN_EXP && exp > target_exp; exp--)
		list_enqueue((list_t*) BLOCK_HIGH_HALF(block, exp), exp - 1);
}

/**
 * Coalesces a block and returns it.
 * 
 * @param block the block to be coalesced
 * @param exp the exponent of the size of the block
 * 
 * @return the coalesced block
*/
static list_t* block_coalesce(list_t* block, int* exp)
{
	for(; *exp < BLOCK_MAX_EXP; (*exp)++)
	{
		/* Get the block's buddy */
		list_t* buddy = list_remove(block_buddy(block, *exp), *exp);

		/* Stop if the buddy is in use */
		if(buddy == NULL)
			break;

		/* Update the block to the coalesced block */
		block = MIN(block, buddy);
	}

	//printf("coalesced to exp %d\n", *exp);

	return block;
}

/**
 * Returns a block's buddy.
 * 
 * @param block the block to get the buddy of
 * @param exp the exponent of the size of the block
 * 
 * @return the block's buddy
*/
static list_t* block_buddy(list_t* block, int exp)
{
	/* Offset of the block relative to the heap's start */
	uintptr_t block_offset = (uintptr_t) heap_start - (uintptr_t) block;

	/* If block is divisible by the size of the exponent above it it means
	   it's the lower address buddy, otherwise, it's the higher address buddy */
	if(block_offset % EXP_TO_SIZE(exp + 1) == 0)
		return (list_t*) BLOCK_HIGH_BUDDY(block, exp);
	else
		return (list_t*) BLOCK_LOW_BUDDY(block, exp);
}


static int exp2_round_up(int num)
{
	/* 0 and 1 are edge cases (but ignore 0) */
	if(num == 1)
		return 1;

	int exp = 0;
	unsigned int unum = num;
	
	while(unum >>= 1)
		exp++;

	bool has_remainder = num & ~(1 << exp);

	return exp + has_remainder;
}

static int exp2_round_down(int num)
{
	int exp = 0;
	
	while(num >>= 1)
		exp++;

	return exp;
}


static int exp_of_alignment(uintptr_t block_addr)
{
	for(int i = BLOCK_MAX_EXP; i >= BLOCK_MIN_EXP; i--)
		if(block_addr % EXP_TO_SIZE(i) == 0)
			return i;

	return 0;
}
