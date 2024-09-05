#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "list.h"
#include "my_malloc.h"
#define PAGE_SIZE 4096

#define CACHE_NAMELEN 			20

#define ON_SLAB_MAX_SIZE    	(PAGE_SIZE / 8)

/* The number of objs per slab in slabs that have an off-slab kmem_bufctl_t */
#define OFF_SLAB_OBJS_PER_SLAB 	4

/* true if the cache's slab managers are off-slab */
#define OFF_SLAB(cache)    		(((kmem_cache_t*) (cache))->obj_size > ON_SLAB_MAX_SIZE)

#define SIZE_OF_SLAB_T(cache) 	(sizeof(slab_t) + ((kmem_cache_t*) (cache))->objs_per_slab * sizeof(kmem_bufctl_t))


typedef struct kmem_cache_s {
	list_t list;
	list_t slabs_full;
	list_t slabs_partial;
	list_t slabs_free;

	unsigned int obj_size;
	unsigned int objs_per_slab;
	unsigned int pages_per_slab;

	char name[CACHE_NAMELEN];

	void (*constructor)(void*, size_t);
	void (*destructor)(void*, size_t);
} kmem_cache_t;

typedef unsigned int kmem_bufctl_t;

typedef struct slab_s {
	list_t list;
	void* s_mem;
	unsigned int num_free;
	kmem_bufctl_t free[];
} slab_t;


static kmem_cache_t cache_cache;

static list_t cache_list;

static size_t general_cache_sizes[] = {16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072};

#define NUM_GENERAL_CACHES  (sizeof(general_cache_sizes) / sizeof(general_cache_sizes[0]))

static kmem_cache_t* general_caches[NUM_GENERAL_CACHES];


void* kmem_alloc(size_t size);
void kmem_free(void* ptr, size_t size);

void kmem_cache_init(void);
kmem_cache_t* kmem_cache_create(const char* name, size_t obj_size, void (*constructor)(void*, size_t), void (*destructor)(void*, size_t));
void kmem_cache_grow(kmem_cache_t* cache);
void kmem_cache_reap(kmem_cache_t* cache);
void* kmem_cache_alloc(kmem_cache_t* cache);
void kmem_cache_free(kmem_cache_t* cache, void* objp);
void kmem_cache_destroy(kmem_cache_t* cache);

static void init_cache(kmem_cache_t* cache, const char* name, size_t obj_size, void (*constructor)(void*, size_t), void (*destructor)(void*, size_t));
static slab_t* slab_get(kmem_cache_t* cache);
static void slab_destroy(kmem_cache_t* cache, slab_t* slab);
static kmem_cache_t* general_cache(size_t size);


/* Global Functions */

void* kmem_alloc(size_t size)
{
	if(size == 0)
		return NULL;

	kmem_cache_t* cache = general_cache(size);

	if(cache == NULL)
		return NULL;

	return kmem_cache_alloc(cache);
}

void kmem_free(void* ptr, size_t size)
{
	kmem_cache_t* cache = general_cache(size);
	kmem_cache_free(cache, ptr);
}


/* Cache Functions */

void kmem_cache_init(void)
{
	cache_list = LIST_INIT(cache_list);

	init_cache(&cache_cache, "kmem_cache", sizeof(kmem_cache_t), NULL, NULL);
	list_add_last(&cache_list, &cache_cache.list);
	
	/* Initialize general caches */
	for(unsigned int i = 0; i < NUM_GENERAL_CACHES; i++)
		general_caches[i] = kmem_cache_create("kmem_cache", general_cache_sizes[i], NULL, NULL);
}

kmem_cache_t* kmem_cache_create(const char* name, size_t obj_size, void (*constructor)(void*, size_t), void (*destructor)(void*, size_t))
{
	kmem_cache_t* cache = kmem_cache_alloc(&cache_cache);

	init_cache(cache, name, obj_size, constructor, destructor);
	list_add_last(&cache_list, &cache->list);
	
	return cache;
}

/**
 * Grows a cache by allocating a new slab for it.
 * 
 * @param cache the cache to grow
*/
void kmem_cache_grow(kmem_cache_t* cache)
{
	slab_t* slab;
	size_t slab_size = cache->pages_per_slab * PAGE_SIZE;
	unsigned char* s_mem = malloc(slab_size);

	if(OFF_SLAB(cache))
		slab = kmem_alloc(SIZE_OF_SLAB_T(cache));
	else
		slab = (slab_t*) (s_mem + slab_size - SIZE_OF_SLAB_T(cache));

	slab->s_mem = s_mem;
	slab->num_free = cache->objs_per_slab;

	/* Initialize the objects and the free array */
	for(unsigned int i = 0; i < slab->num_free; i++)
	{
		slab->free[i] = i;

		if(cache->constructor != NULL)
			cache->constructor(((unsigned char*) (slab->s_mem)) + i * cache->obj_size, cache->obj_size);
	}

	list_add_last(&(cache->slabs_free), &slab->list);
}

/**
 * Frees all free slabs from a cache.
 * 
 * @param cache the cache to free all free slabs from
*/
void kmem_cache_reap(kmem_cache_t* cache)
{
	void slab_destroy_fe(list_t* entry) {
		list_remove(&cache->slabs_free, entry);
		slab_destroy(cache, (slab_t*) entry);
	}

	list_for_each(&cache->slabs_free, slab_destroy_fe);
}

/**
 * Allocates a buffer from a cache.
 * 
 * @param cache the cache to allocate the buffer from
 * 
 * @return the allocated buffer's address or NULL if there are no available
 * buffers and not enough memory could be allocated to create a new slab
*/
void* kmem_cache_alloc(kmem_cache_t* cache)
{
	slab_t* slab = slab_get(cache);

	if(slab == NULL)
		return NULL;

	void* buf = ((unsigned char*) slab->s_mem) + slab->free[--slab->num_free] * cache->obj_size;

	/* If the slab was free, move it to partial list */
	if(slab->num_free == cache->objs_per_slab - 1) {
		list_remove(&cache->slabs_free, &slab->list);
		list_add_last(&cache->slabs_partial,  &slab->list);
	}

	/* If no free buffers remain, move the slab from partial list to the full list */
	if(slab->num_free == 0) {
		list_remove(&cache->slabs_partial, &slab->list);
		list_add_last(&cache->slabs_full,  &slab->list);
	}

	return buf;
}

/**
 * Frees a buffer of a cache.
 * 
 * @param cache the cache to free the buffer from
*/
void kmem_cache_free(kmem_cache_t* cache, void* ptr)
{
	size_t slab_size = cache->pages_per_slab * PAGE_SIZE;
	uintptr_t buf_ptr = (uintptr_t) ptr;

	bool slab_contains_buf_fe(list_t* entry) {
		uintptr_t slab_start = (uintptr_t) ((slab_t*) entry)->s_mem;
		return buf_ptr >= slab_start && buf_ptr < slab_start + slab_size;
	}

	slab_t* slab = (slab_t*) list_search(&cache->slabs_partial, slab_contains_buf_fe);
	if(slab == NULL)
		slab = (slab_t*) list_search(&cache->slabs_full, slab_contains_buf_fe);

	if(slab == NULL) {
		// assert here
		printf("panic: no slab found for ptr when freeing\n");
	}

	uintptr_t slab_start = ((uintptr_t) slab->s_mem);
	unsigned int index = (buf_ptr - slab_start) / cache->obj_size;

	slab->free[(slab->num_free)++] = index;

	/* If the freed object belonged to a full list, move it to partial */
	if(slab->num_free == 1) {
		list_remove(&cache->slabs_full, &slab->list);
		list_add_last(&cache->slabs_partial, &slab->list);
	}

	/* If all objects are free, move slab to free list and reap the cache */
	if(slab->num_free == cache->objs_per_slab) {
		list_remove(&cache->slabs_partial, &slab->list);
		list_add_last(&cache->slabs_free, &slab->list);
		kmem_cache_reap(cache);
	}
}

/**
 * Destroys a cache by freeing itself all its and its slabs.
 * 
 * @param cache the cache to be destroyed
*/
void kmem_cache_destroy(kmem_cache_t* cache)
{
	list_t* list;

	void slab_destroy_fe(list_t* entry) {
		list_remove(list, entry);
		slab_destroy(cache, (slab_t*) entry);
	}

	list = &cache->slabs_full;
	list_for_each(list, slab_destroy_fe);
	list = &cache->slabs_partial;
	list_for_each(list, slab_destroy_fe);
	list = &cache->slabs_free;
	list_for_each(list, slab_destroy_fe);
}


/* Helper Functions */


/**
 * Initializes a cache with the given parameters and adds it to the caches list.
 * 
 * @param cache the cache to be initialized
 * @param name the name of the cache
 * @param obj_size the size of the cache's objects
 * @param constructor the constructor function for the cache's objects
 * @param destructor the destructor function for the cache's objects
*/
static void init_cache(kmem_cache_t* cache, const char* name, size_t obj_size, void (*constructor)(void*, size_t), void (*destructor)(void*, size_t))
{
	/* Initialize slabs lists */
	cache->slabs_full = LIST_INIT(cache->slabs_full);
	cache->slabs_partial = LIST_INIT(cache->slabs_partial);
	cache->slabs_free = LIST_INIT(cache->slabs_free);

	/* Set given parameters */
	cache->obj_size = obj_size;
	cache->constructor = constructor;
	cache->destructor = destructor;
	strncpy(cache->name, name, CACHE_NAMELEN);

	/* Calculate remaining parameters */
	if(OFF_SLAB(cache))
		cache->pages_per_slab = obj_size * OFF_SLAB_OBJS_PER_SLAB / PAGE_SIZE;
	else
		cache->pages_per_slab = 1;

	size_t slab_size = cache->pages_per_slab * PAGE_SIZE;

	if(!OFF_SLAB(cache)) {
		cache->objs_per_slab = slab_size / obj_size;
		unsigned int remaining = slab_size % obj_size;

		while(remaining < SIZE_OF_SLAB_T(cache)) {
			cache->objs_per_slab--;
			remaining += obj_size;
		}
	} else {
		cache->objs_per_slab = slab_size / obj_size;
	}
}


/**
 * Returns the general cache most suitable for a given size.
 * 
 * @param size the size of the object
 * 
 * @return the most suitable general cache for the given size
*/
static kmem_cache_t* general_cache(size_t size)
{
	/* Return NULL on size 0 requests */
	if(size == 0)
		return NULL;

	kmem_cache_t* cache = NULL;
	
	/* Search backwards in the general caches' until one doesn't fit or smallest is reached */
	for(int i = NUM_GENERAL_CACHES - 1; i >= 0; i--) {
		if(size > general_caches[i]->obj_size)
			break;

		cache = general_caches[i];
	}

	return cache;
}


/**
 * Returns a non-full slab from a cache, prioritizing partial ones.
 * Automatically grows the cache if no available slabs are found.
 * 
 * @param cache the cache from where to get the slab
 * 
 * @return a non-full slab or NULL if no available slabs are found
 * the cache wasn't able to grow
*/
static slab_t* slab_get(kmem_cache_t* cache)
{
	slab_t* slab = (slab_t*) LIST_FIRST(&cache->slabs_partial);
	
	if(slab == NULL) {
		/* Grow cache if no partial or free slabs exist */
		if(LIST_IS_EMPTY(&cache->slabs_free))
			kmem_cache_grow(cache);

		slab = (slab_t*) LIST_FIRST(&cache->slabs_free);

		/* Return NULL if cache wasn't able to grow */
		if(slab == NULL)
			return NULL;
	}

	return slab;
}


/**
 * Destroys a slab, freeing its memory.
 * 
 * The slab is not removed from the cache's corresponding list.
 * 
 * @param cache the cache that the slab belongs to
 * @param slab the slab to be destroyed 
*/
static void slab_destroy(kmem_cache_t* cache, slab_t* slab)
{
	if(cache->destructor != NULL)
		for(unsigned int i = 0; i < cache->objs_per_slab; i++)
			cache->destructor(((unsigned char*) (slab->s_mem)) + i * cache->obj_size, cache->obj_size);

	free(slab->s_mem);

	if(OFF_SLAB(cache))
		kmem_free(slab, SIZE_OF_SLAB_T(cache));
}


void slab_print_heap(void)
{
	void print_cache(list_t* entry) {
		if(((kmem_cache_t*) entry) != &cache_cache && !LIST_IS_EMPTY(&((kmem_cache_t*) entry)->slabs_partial))
			printf("bing problem at cache %s with objsize %u\n", ((kmem_cache_t*) entry)->name, ((kmem_cache_t*) entry)->obj_size);
	}

	list_for_each(&cache_list, print_cache);
}
