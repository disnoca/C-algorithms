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

// The initial capacity - MUST be a power of two.
static const size_t DEFUALT_INITIAL_CAPACITY = 1 << 4;    // aka 16
static const float DEFAULT_LOAD_FACTOR = 0.75;

static void put_entry(HashMap* hm, void* key, void* value);

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
        if (!memcmp(curr_node, &EMPTY_NODE, sizeof(HMNode))) continue;

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
    HMNode* node = (HMNode*) Calloc(1, sizeof(HMNode));
    node->key = key;
    node->value = value;
    return node;
}

static HMNode* get_bucket(HashMap* hm, void* key)
{
    return hm->buckets + (hm->hash(key) % hm->capacity);
}

static HMNode* get_node(HashMap* hm, void* key)
{
    HMNode* bucket = get_bucket(hm, key);
    while (hm->hash(bucket->key) != hm->hash(key))
        bucket = bucket->next;
    return bucket;
}

static void put_entry(HashMap* hm, void* key, void* value)
{
    HMNode* bucket = get_bucket(hm, key);
    
    if (!memcmp(bucket, &EMPTY_NODE, sizeof(HMNode))) {
        bucket->key = key;
        bucket->value = value;
        return;
    }

    while (bucket->next != NULL)
        bucket = bucket->next;
    bucket->next = create_node(key, value);
}

/* ---------------- Header Implementation ---------------- */

void hm_init(HashMap* hash_map, unsigned long(*hash)(const void*))
{
    hash_map->buckets = (HMNode*) Calloc(DEFUALT_INITIAL_CAPACITY, sizeof(HMNode));
    hash_map->capacity = DEFUALT_INITIAL_CAPACITY;
    hash_map->load_factor = DEFAULT_LOAD_FACTOR;
    hash_map->size = 0;
    hash_map->hash = hash;
}

HashMap* hm_create(unsigned long(*hash)(const void*))
{
    HashMap* hm = (HashMap*) Malloc(sizeof(HashMap));
    hm_init(hm, hash);
    return hm;
}

void hm_put(HashMap* hm, void* key, void* value)
{
    put_entry(hm, key, value);

    if (++hm->size > (hm->capacity * hm->load_factor))
        rehash(hm);
}

void* hm_remove(HashMap* hm, void* key)
{
    HMNode* bucket = get_bucket(hm, key);

    HMNode* prev_node = NULL;
    while (hm->hash(bucket->key) != hm->hash(key)) {
        prev_node = bucket;
        bucket = bucket->next;
    }

    void* value = bucket->value;
    if (prev_node == NULL)
        *bucket = EMPTY_NODE;
    else {
        prev_node->next = bucket->next;
        Free(bucket);
    }

    hm->size--;
    return value;
}

void* hm_get(HashMap* hm, void* key)
{
    return get_node(hm, key)->value;
}

void* hm_replace(HashMap* hm, void* key, void* value)
{
    HMNode* node = get_node(hm, key);
    void* old_value = node->value;
    node->value = value;
    return old_value;

}

void hm_clear(HashMap* hm)
{
    HMNode* buckets = hm->buckets;
    size_t capacity = hm->capacity;

    HMNode *curr_node, *next_node;
    for(size_t i = 0; i < capacity; i++) {
        curr_node = buckets + i;
        if (!memcmp(curr_node, &EMPTY_NODE, sizeof(HMNode))) continue;

        next_node = curr_node->next;
        *curr_node = EMPTY_NODE;
        while (next_node != NULL) {
            curr_node = next_node;
            next_node = curr_node->next;
            Free(curr_node);       // only free the consequent nodes because the buckets themselves must stay
        }
    }

    hm->size = 0;
}

void hm_free(HashMap* hash_map)
{
    hm_clear(hash_map);
    Free(hash_map->buckets);
}

void hm_destroy(HashMap* hm)
{
    hm_free(hm);
    Free(hm);
}
