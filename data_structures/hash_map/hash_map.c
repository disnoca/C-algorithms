/**
 * Hash Map implementation.
 * 
 * @author Samuel Pires
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_map.h"
#include "../../wrapper_functions.h"

static const HMNode EMPTY_NODE;

#define BUCKET_IS_EMPTY(b)	(!memcmp(b, &EMPTY_NODE, sizeof(HMNode)))


// The initial capacity - MUST be a power of two.
static const size_t DEFUALT_INITIAL_CAPACITY = 1 << 4;    // aka 16
static const float DEFAULT_LOAD_FACTOR = 0.75;

static void* put_entry(HashMap* hm, void* key, void* value);

/* ---------------- Helper Functions ---------------- */

static void rehash(HashMap* hm)
{
	hm->capacity *= 2;
	size_t new_capacity = hm->capacity;

	HMNode* old_buckets = hm->buckets;
	hm->buckets = (HMNode*) Calloc(new_capacity, sizeof(HMNode));

	HMNode *curr_node, *next_node;
	size_t old_capacity = new_capacity/2;
	for(size_t i = 0; i < old_capacity; i++) {
		curr_node = old_buckets + i;
		if (BUCKET_IS_EMPTY(curr_node))
			continue;

		put_entry(hm, curr_node->key, curr_node->value);
		next_node = curr_node->next;
		while (next_node != NULL) {
			curr_node = next_node;
			next_node = curr_node->next;
			put_entry(hm, curr_node->key, curr_node->value);
			Free(curr_node);       // only free the consequent nodes because the buckets themselves will be freed all at once
		}
	}

	Free(old_buckets);
}

static HMNode* create_node(void* key, void* value)
{
	HMNode* node = (HMNode*) Malloc(sizeof(HMNode));
	node->key = key;
	node->value = value;
	node->next = NULL;
	return node;
}

static HMNode* get_bucket(HashMap* hm, void* key)
{
	return hm->buckets + (hm->hash(key) % hm->capacity);
}

static HMNode* get_node(HashMap* hm, void* key)
{
	HMNode* node = get_bucket(hm, key);
	if (BUCKET_IS_EMPTY(node))
		return NULL;

	while (node != NULL && !hm->equals(node->key, key))
		node = node->next;

	return node;
}

static void* put_entry(HashMap* hm, void* key, void* value)
{
	HMNode* curr_node = get_bucket(hm, key);
	if (BUCKET_IS_EMPTY(curr_node)) {
		curr_node->key = key;
		curr_node->value = value;
		return NULL;
	}

	HMNode* prev_node = NULL;
	while (curr_node != NULL) {
		if (hm->equals(curr_node->key, key)) {
			void* old_val = curr_node->value;
			curr_node->value = value;
			return old_val;
		}

		prev_node = curr_node;
		curr_node = curr_node->next;
	}
		
	prev_node->next = create_node(key, value);
	return NULL;
}

/* ---------------- Header Implementation ---------------- */

void hm_init(HashMap* hm, unsigned long(*hash)(const void*), bool(*equals)(void*,void*))
{
	hm->buckets = (HMNode*) Calloc(DEFUALT_INITIAL_CAPACITY, sizeof(HMNode));
	hm->capacity = DEFUALT_INITIAL_CAPACITY;
	hm->load_factor = DEFAULT_LOAD_FACTOR;
	hm->size = 0;
	hm->hash = hash;
	hm->equals = equals;
}

HashMap* hm_create(unsigned long(*hash)(const void*), bool(*equals)(void*,void*))
{
	HashMap* hm = (HashMap*) Malloc(sizeof(HashMap));
	hm_init(hm, hash, equals);
	return hm;
}

void* hm_put(HashMap* hm, void* key, void* value)
{
	void* old_val = put_entry(hm, key, value);

	if (old_val != NULL)
		return old_val;

	if (++hm->size > (hm->capacity * hm->load_factor))
		rehash(hm);

	return NULL;
}

void* hm_remove(HashMap* hm, void* key)
{
	HMNode* curr_node = get_bucket(hm, key);
	if (BUCKET_IS_EMPTY(curr_node))
		return NULL;

	HMNode* prev_node = NULL;
	while (curr_node != NULL && !hm->equals(curr_node->key, key)) {
		prev_node = curr_node;
		curr_node = curr_node->next;
	}

	if (curr_node == NULL)
		return NULL;

	void* value = curr_node->value;
	if (prev_node == NULL) {
		*curr_node = EMPTY_NODE;
	} else {
		prev_node->next = curr_node->next;
		Free(curr_node);
	}

	hm->size--;
	return value;
}

void* hm_get(HashMap* hm, void* key)
{
	HMNode* node = get_node(hm, key);
	return node ? node->value : NULL;
}

void* hm_replace(HashMap* hm, void* key, void* value)
{
	HMNode* node = get_node(hm, key);
	if (node == NULL)
		return NULL;

	void* old_value = node->value;
	node->value = value;
	return old_value;

}

void hm_clear(HashMap* hm, bool free_keys, bool free_values)
{
	HMNode* buckets = hm->buckets;
	size_t capacity = hm->capacity;

	HMNode *curr_node, *next_node;
	for(size_t i = 0; i < capacity; i++) {
		curr_node = buckets + i;
		if (BUCKET_IS_EMPTY(curr_node))
			continue;

		next_node = curr_node->next;
		*curr_node = EMPTY_NODE;
		while (next_node != NULL) {
			curr_node = next_node;
			next_node = curr_node->next;
			if (free_keys) Free(curr_node->key);
			if (free_values) Free(curr_node->value);
			Free(curr_node);       // only free the subsequent nodes because the buckets themselves must stay
		}
	}

	hm->size = 0;
}

void hm_free(HashMap* hash_map, bool free_keys, bool free_values)
{
	hm_clear(hash_map, free_keys, free_values);
	Free(hash_map->buckets);
}

void hm_destroy(HashMap* hm, bool free_keys, bool free_values)
{
	hm_free(hm, free_keys, free_values);
	Free(hm);
}
