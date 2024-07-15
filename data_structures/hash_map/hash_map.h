#pragma once

/* ---------------- Structs ---------------- */

typedef struct HMNode HMNode;

struct HMNode {
	HMNode* next;
	void *key, *value;
};

typedef struct {
	HMNode* buckets;   	// the pointer to the first position of the array of buckets
	size_t capacity;    // the number of buckets
	float load_factor;	// the maximum ratio of size/capacity before the hash_map is resized
	size_t size;       	// the number of elements the hash_map currently has
	unsigned long(*hash)(const void*);
} HashMap;


/* ---------------- Functions ---------------- */

HashMap* hm_create(unsigned long(*hash)(const void*));
void hm_destroy(HashMap* hash_map);
void hm_clear(HashMap* hash_map);

void hm_put(HashMap* hash_map, void* key, void* value);
void* hm_remove(HashMap* hash_map, void* key);
void* hm_get(HashMap* hash_map, void* key);
void* hm_replace(HashMap* hash_map, void* key, void* value);


// TODO: put returns old value
