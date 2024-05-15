#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <string.h>
#include "hash_map.h"
#include "../../wrapper_functions.h"

const HMNode EMPTY_NODE;


char keys[8192];
int values[1024];
int curr_key, curr_val;

#define REF_OF_KEY(s)   ({ char* key_addr = keys + curr_key; strcpy(key_addr, s); curr_key += strlen(s) + 1; key_addr; })
#define REF_OF_VAL(x)   ({ values[curr_val] = (x); values + curr_val++; })
#define DEREF_VAL(p)    (*((int*)(p)))


unsigned long hash(const void* key) {
	const char* s = key;
	unsigned long hash = 0;
	while (*s)
		hash = hash * 101  +  *s++;

	return hash;
}


__attribute__((unused)) static void print_hash_map_contents(HashMap* hm) {
	size_t measured_size = 0;
	for(size_t i = 0; i < hm->capacity; i++) {
		HMNode* curr_node = hm->buckets + i;
		if (!memcmp(curr_node, &EMPTY_NODE, sizeof(HMNode))) continue;

		do {
			printf("bucket %ld, index %ld: %s %d\n", i, measured_size++, (char*) curr_node->key, DEREF_VAL(curr_node->value));
			curr_node = curr_node->next;
		} while (curr_node != NULL);
	}

	if (measured_size < hm->size)
		printf("Too few entries according to hash map's size (%ld)\n", hm->size);
	else if (measured_size > hm->size)
		printf("Too many entries according to hash map's size (%ld)\n", hm->size);
}

static HashMap* create_hash_map_with_ascending_values() {
	HashMap* hm = hm_create(hash);
	hm_put(hm, REF_OF_KEY("zero"), REF_OF_VAL(0));
	hm_put(hm, REF_OF_KEY("one"), REF_OF_VAL(1));
	hm_put(hm, REF_OF_KEY("two"), REF_OF_VAL(2));
	hm_put(hm, REF_OF_KEY("three"), REF_OF_VAL(3));
	hm_put(hm, REF_OF_KEY("four"), REF_OF_VAL(4));
	hm_put(hm, REF_OF_KEY("five"), REF_OF_VAL(5));
	hm_put(hm, REF_OF_KEY("six"), REF_OF_VAL(6));
	hm_put(hm, REF_OF_KEY("seven"), REF_OF_VAL(7));
	hm_put(hm, REF_OF_KEY("eight"), REF_OF_VAL(8));
	hm_put(hm, REF_OF_KEY("nine"), REF_OF_VAL(9));
	return hm;
}

static void test_put_get() {
	HashMap* hm = create_hash_map_with_ascending_values();

	assert(DEREF_VAL(hm_get(hm, REF_OF_KEY("zero"))) == 0);
	assert(DEREF_VAL(hm_get(hm, REF_OF_KEY("one"))) == 1);
	assert(DEREF_VAL(hm_get(hm, REF_OF_KEY("two"))) == 2);
	assert(DEREF_VAL(hm_get(hm, REF_OF_KEY("three"))) == 3);
	assert(DEREF_VAL(hm_get(hm, REF_OF_KEY("four"))) == 4);
	assert(DEREF_VAL(hm_get(hm, REF_OF_KEY("five"))) == 5);
	assert(DEREF_VAL(hm_get(hm, REF_OF_KEY("seven"))) == 7);
	assert(DEREF_VAL(hm_get(hm, REF_OF_KEY("eight"))) == 8);
	assert(DEREF_VAL(hm_get(hm, REF_OF_KEY("nine"))) == 9);
	assert(hm->size == 10);

	hm_clear(hm);
	hm_destroy(hm);
}

static void test_rehash() {
	HashMap* hm = create_hash_map_with_ascending_values();
	hm_put(hm, REF_OF_KEY("ten"), REF_OF_VAL(10));
	hm_put(hm, REF_OF_KEY("eleven"), REF_OF_VAL(11));
	hm_put(hm, REF_OF_KEY("twelve"), REF_OF_VAL(12));
	hm_put(hm, REF_OF_KEY("thirteen"), REF_OF_VAL(13));
	hm_put(hm, REF_OF_KEY("fourteen"), REF_OF_VAL(14));
	hm_put(hm, REF_OF_KEY("fifteen"), REF_OF_VAL(15));
	hm_put(hm, REF_OF_KEY("sixteen"), REF_OF_VAL(16));

	assert(DEREF_VAL(hm_get(hm, REF_OF_KEY("zero"))) == 0);
	assert(DEREF_VAL(hm_get(hm, REF_OF_KEY("ten"))) == 10);
	assert(DEREF_VAL(hm_get(hm, REF_OF_KEY("fifteen"))) == 15);
	assert(DEREF_VAL(hm_get(hm, REF_OF_KEY("sixteen"))) == 16);
	assert(hm->size == 17);
	assert(hm->capacity == 32);
	
	hm_clear(hm);
	hm_destroy(hm);
}

static void test_remove() {
	HashMap* hm = create_hash_map_with_ascending_values();

	assert(DEREF_VAL(hm_remove(hm, REF_OF_KEY("zero"))) == 0);
	assert(DEREF_VAL(hm_remove(hm, REF_OF_KEY("five"))) == 5);
	assert(DEREF_VAL(hm_remove(hm, REF_OF_KEY("nine"))) == 9);
	assert(hm->size == 7);

	hm_clear(hm);
	hm_destroy(hm);
}

static void test_replace() {
	HashMap* hm = create_hash_map_with_ascending_values();

	assert(DEREF_VAL(hm_replace(hm, REF_OF_KEY("zero"), REF_OF_VAL(10))) == 0);
	assert(DEREF_VAL(hm_replace(hm, REF_OF_KEY("five"), REF_OF_VAL(15))) == 5);
	assert(DEREF_VAL(hm_replace(hm, REF_OF_KEY("nine"), REF_OF_VAL(19))) == 9);
	assert(DEREF_VAL(hm_get(hm, REF_OF_KEY("zero"))) == 10);
	assert(DEREF_VAL(hm_get(hm, REF_OF_KEY("five"))) == 15);
	assert(DEREF_VAL(hm_get(hm, REF_OF_KEY("nine"))) == 19);
	assert(hm->size == 10);

	hm_clear(hm);
	hm_destroy(hm);
}

static void test_clear() {
	HashMap* hm = create_hash_map_with_ascending_values();
	hm_clear(hm);

	assert(hm->size == 0);
	for(int i = 0; i < 10; i++)
		assert(!memcmp(hm->buckets+i, &EMPTY_NODE, sizeof(HMNode)));

	hm_destroy(hm);
}

int main() {
	test_put_get();
	test_rehash();
	test_remove();
	test_replace();
	test_clear();

	assert(malloc_calls == free_calls);

	printf("All tests passed.\n");
	return 0;
}
