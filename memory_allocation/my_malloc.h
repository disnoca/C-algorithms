#pragma once

#include <stddef.h>

void* bb_malloc(size_t size);
void bb_free(void* ptr, size_t size);
void bb_add_hole(void* start_addr, void* end_addr);
void bb_print_heap(void);

void kmem_cache_init(void);
void* kmem_alloc(size_t size);
void kmem_free(void* ptr, size_t size);
#define slab_init() kmem_cache_init()
#define slab_malloc(size) kmem_alloc(size)
#define slab_free(ptr, size) kmem_free(ptr, size)
void slab_print_heap(void);
