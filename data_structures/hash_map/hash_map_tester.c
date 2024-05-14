#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hash_map.h"
#include "../../wrapper_functions.h"

const HMNode EMPTY_NODE;

__attribute__((unused)) static void print_hash_map_contents(HashMap* hash_map) {
    int measured_size = 0;
    for(int i = 0; i < hash_map->capacity; i++) {
        HMNode* curr_node = hash_map->buckets + i;
        if (!memcmp(curr_node, &EMPTY_NODE, sizeof(HMNode))) continue;

        do {
            printf("bucket %d, index %d: %s %d\n", i, measured_size++, curr_node->key, curr_node->value);
            curr_node = curr_node->next;
        } while (curr_node != NULL);
    }

    if (measured_size < hash_map->size)
        printf("Too few entries according to hash map's size (%d)\n", hash_map->size);
    else if (measured_size > hash_map->size)
        printf("Too many entries according to hash map's size (%d)\n", hash_map->size);
}

static HashMap* create_hash_map_with_ascending_values() {
    HashMap* hash_map = hm_create();
    hm_put(hash_map, "zero", 0);
    hm_put(hash_map, "one", 1);
    hm_put(hash_map, "two", 2);
    hm_put(hash_map, "three", 3);
    hm_put(hash_map, "four", 4);
    hm_put(hash_map, "five", 5);
    hm_put(hash_map, "six", 6);
    hm_put(hash_map, "seven", 7);
    hm_put(hash_map, "eight", 8);
    hm_put(hash_map, "nine", 9);
    return hash_map;
}

static void test_put_get() {
    HashMap* hash_map = create_hash_map_with_ascending_values();

    assert(hm_get(hash_map, "zero") == 0);
    assert(hm_get(hash_map, "one") == 1);
    assert(hm_get(hash_map, "two") == 2);
    assert(hm_get(hash_map, "three") == 3);
    assert(hm_get(hash_map, "four") == 4);
    assert(hm_get(hash_map, "five") == 5);
    assert(hm_get(hash_map, "seven") == 7);
    assert(hm_get(hash_map, "eight") == 8);
    assert(hm_get(hash_map, "nine") == 9);
    assert(hash_map->size == 10);

    hm_clear(hash_map);
    hm_destroy(hash_map);
}

static void test_rehash() {
    HashMap* hash_map = create_hash_map_with_ascending_values();
    hm_put(hash_map, "ten", 10);
    hm_put(hash_map, "eleven", 11);
    hm_put(hash_map, "twelve", 12);
    hm_put(hash_map, "thirteen", 13);
    hm_put(hash_map, "fourteen", 14);
    hm_put(hash_map, "fifteen", 15);
    hm_put(hash_map, "sixteen", 16);

    assert(hm_get(hash_map, "zero") == 0);
    assert(hm_get(hash_map, "ten") == 10);
    assert(hm_get(hash_map, "fifteen") == 15);
    assert(hm_get(hash_map, "sixteen") == 16);
    assert(hash_map->size == 17);
    assert(hash_map->capacity == 32);
    
    hm_clear(hash_map);
    hm_destroy(hash_map);
}

static void test_remove() {
    HashMap* hash_map = create_hash_map_with_ascending_values();

    assert(hm_remove(hash_map, "zero") == 0);
    assert(hm_remove(hash_map, "five") == 5);
    assert(hm_remove(hash_map, "nine") == 9);
    assert(hash_map->size == 7);

    hm_clear(hash_map);
    hm_destroy(hash_map);
}

static void test_replace() {
    HashMap* hash_map = create_hash_map_with_ascending_values();

    assert(hm_replace(hash_map, "zero", 10) == 0);
    assert(hm_replace(hash_map, "five", 15) == 5);
    assert(hm_replace(hash_map, "nine", 19) == 9);
    assert(hm_get(hash_map, "zero") == 10);
    assert(hm_get(hash_map, "five") == 15);
    assert(hm_get(hash_map, "nine") == 19);
    assert(hash_map->size == 10);

    hm_clear(hash_map);
    hm_destroy(hash_map);
}

static void test_clear() {
    HashMap* hash_map = create_hash_map_with_ascending_values();
    hm_clear(hash_map);

    assert(hash_map->size == 0);
    for(int i = 0; i < 10; i++)
        assert(!memcmp(hash_map->buckets+i, &EMPTY_NODE, sizeof(HMNode)));

    hm_destroy(hash_map);
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
