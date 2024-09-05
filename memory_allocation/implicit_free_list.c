#include <unistd.h>


typedef size_t word;

#define WSIZE       		sizeof(word)    /* Word and header/footer size (bytes) */
#define DSIZE       		(2 * WSIZE)    	/* Doubleword size (bytes) */
#define CHUNKSIZE  			(1 << 12)  		/* Extend heap by this amount (bytes) */

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  	((size) | (alloc))

/* Block used to mark start and end of heap (allocated, size 0) */
#define DELIMITER			PACK(0, 1)

/* Read and write a word at address p */
#define GET(p)       		(*(word*)(p))
#define PUT(p, val)  		(*(word*)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)  		(GET(p) & ~0x7)
#define GET_ALLOC(p) 		(GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)       		((unsigned char*)(bp) - WSIZE)
#define FTRP(bp)       		((unsigned char*)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp)  		((unsigned char*)(bp) + GET_SIZE(((unsigned char*)(bp) - WSIZE)))
#define PREV_BLKP(bp)  		((unsigned char*)(bp) - GET_SIZE(((unsigned char*)(bp) - DSIZE)))

#define MAX(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
#define MIN(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })

#define DIV_ROUND_UP(x,y) 	({__typeof__ (x) _x = (x); __typeof__ (y) _y = (y); ((_x / _y) + !!(_x % _y)); })
#define ROUND_UP(n,m)		({__typeof__ (n) _n = (n); __typeof__ (m) _m = (m); _n % _m == 0 ? _n : (_n + (_m - (_n % _m))); })	
#define ROUND_DOWN(n,m)		({__typeof__ (n) _n = (n); (_n - (_n % (m))); })


static void* extend_heap(size_t size);
static void place(void* bp, size_t asize);
static void* find_fit(size_t asize);
static void *coalesce(void* bp);


static void* first_block;


static void my_init()
{
	unsigned char* heap_start = (unsigned char*) sbrk(CHUNKSIZE);	/* Assumes that the initial sbrk is aligned */

	/* Mark heap start and end with delimiters */
	PUT(heap_start, DELIMITER);
	PUT(heap_start + CHUNKSIZE - WSIZE, DELIMITER);

	/* Point first_block to the first block and mark it as free with the available size */
	first_block = heap_start + DSIZE;
	PUT(HDRP(first_block), PACK(CHUNKSIZE - DSIZE, 0));
	PUT(FTRP(first_block), PACK(CHUNKSIZE - DSIZE, 0));
}


void* my_malloc(size_t size)
{
	if(size == 0)
		return NULL;

	if(first_block == 0)
		my_init();

	void* bp;

	size_t asize = ROUND_UP(size + DSIZE, WSIZE);
	if((bp = find_fit(asize)) != NULL) {
		place(bp, asize);
		return bp;
	}

	size_t extendsize = ROUND_UP(asize, CHUNKSIZE);
	if((bp = extend_heap(extendsize)) == NULL)
		return NULL;

	place(bp, asize);
	return bp;
}


void my_free(void* bp)
{
    if(bp == NULL)
		return;

    size_t size = GET_SIZE(HDRP(bp));

    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    coalesce(bp);
}



/* Helper Functions */


static void place(void* bp, size_t asize)
{
	size_t csize = GET_SIZE(HDRP(bp));   

    if ((csize - asize) >= WSIZE) { 
		PUT(HDRP(bp), PACK(asize, 1));
		PUT(FTRP(bp), PACK(asize, 1));
		bp = NEXT_BLKP(bp);
		PUT(HDRP(bp), PACK(csize-asize, 0));
		PUT(FTRP(bp), PACK(csize-asize, 0));
    }
    else { 
		PUT(HDRP(bp), PACK(csize, 1));
		PUT(FTRP(bp), PACK(csize, 1));
    }
}


static void *coalesce(void *bp) 
{
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc)
		return bp;

    else if (prev_alloc && !next_alloc) {
		size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
		PUT(HDRP(bp), PACK(size, 0));
		PUT(FTRP(bp), PACK(size,0));
    }

    else if (!prev_alloc && next_alloc) {
		size += GET_SIZE(HDRP(PREV_BLKP(bp)));
		PUT(FTRP(bp), PACK(size, 0));
		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
		bp = PREV_BLKP(bp);
    }

    else {
		size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));
		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
		PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
		bp = PREV_BLKP(bp);
    }

    return bp;
}


static void* find_fit(size_t asize)
{
	for(void* bp = first_block; GET_SIZE(bp) == 0; bp = NEXT_BLKP(bp))
		if(!GET_ALLOC(bp) && GET_SIZE(bp) >= asize)
			return bp;
	
	return NULL;
}


static void* extend_heap(size_t size)
{
	unsigned char* old_end;
	if((old_end = (unsigned char*) sbrk(size)) == NULL)
		return NULL;

	/* Update end delimiter position */
	PUT(old_end + size - WSIZE, PACK());

	/* Update last block's size */
	void* last_block = PREV_BLKP(old_end - WSIZE);
	place(last_block, GET_SIZE(last_block) + size - WSIZE);

	return last_block;
}
