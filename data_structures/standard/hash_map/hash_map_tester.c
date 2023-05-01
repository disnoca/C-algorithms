#include <stdio.h>
#include <string.h>
#include "hash_map.h"
#include "../../../wrapper_functions.h"

const HMNode EMPTY_NODE;

int print_hash_map_contents(HashMap* hash_map) {
    int measured_size = 0;
    for(int i = 0; i < hash_map->capacity; i++) {
        HMNode* curr_node = hash_map->buckets + i;
        if(!memcmp(curr_node, &EMPTY_NODE, sizeof(HMNode))) continue;

        do {
            printf("bucket %d, index %d: %s %d\n", i, measured_size++, curr_node->key, curr_node->value);
            curr_node = curr_node->next;
        } while(curr_node != NULL);
    }

    if(measured_size == hash_map->size)
        return 0;
    if(measured_size < hash_map->size)
        printf("Too few entries according to hash map's size (%d)\n", hash_map->size);
    else
        printf("Too many entries according to hash map's size (%d)\n", hash_map->size);
    return 1;
}

HashMap* create_hash_map_with_ascending_values() {
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

int test_put_get() {
    HashMap* hash_map = create_hash_map_with_ascending_values();

    int test_score = 0;
    test_score += hm_get(hash_map, "zero") == 0 ? 1 : 0;
    test_score += hm_get(hash_map, "one") == 1 ? 10 : 0;
    test_score += hm_get(hash_map, "two") == 2 ? 100 : 0;
    test_score += hm_get(hash_map, "three") == 3 ? 1000 : 0;
    test_score += hm_get(hash_map, "four") == 4 ? 10000 : 0;
    test_score += hm_get(hash_map, "five") == 5 ? 100000 : 0;
    test_score += hm_get(hash_map, "seven") == 7 ? 1000000 : 0;
    test_score += hm_get(hash_map, "eight") == 8 ? 10000000 : 0;
    test_score += hm_get(hash_map, "nine") == 9 ? 100000000 : 0;
    test_score += hash_map->size == 10 ? 2000000000 : 0;

    hm_clear(hash_map);
    hm_destroy(hash_map);
    return test_score;
}

int test_rehash() {
    HashMap* hash_map = create_hash_map_with_ascending_values();
    hm_put(hash_map, "ten", 10);
    hm_put(hash_map, "eleven", 11);
    hm_put(hash_map, "twelve", 12);
    hm_put(hash_map, "thirteen", 13);
    hm_put(hash_map, "fourteen", 14);
    hm_put(hash_map, "fifteen", 15);
    hm_put(hash_map, "sixteen", 16);

    int test_score = 0;
    test_score += hm_get(hash_map, "zero") == 0 ? 1 : 0;
    test_score += hm_get(hash_map, "ten") == 10 ? 10 : 0;
    test_score += hm_get(hash_map, "fifteen") == 15 ? 100 : 0;
    test_score += hm_get(hash_map, "sixteen") == 16 ? 1000 : 0;
    test_score += hash_map->size == 17 ? 10000 : 0;
    test_score += hash_map->capacity == 32 ? 200000 : 0;
    
    hm_clear(hash_map);
    hm_destroy(hash_map);
    return test_score;
}

int test_remove() {
    HashMap* hash_map = create_hash_map_with_ascending_values();

    int test_score = 0;
    test_score += hm_remove(hash_map, "zero") == 0 ? 1 : 0;
    test_score += hm_remove(hash_map, "five") == 5 ? 10 : 0;
    test_score += hm_remove(hash_map, "nine") == 9 ? 100 : 0;
    test_score += hash_map->size == 7 ? 2000 : 0;

    hm_clear(hash_map);
    hm_destroy(hash_map);
    return test_score;
}

int test_replace() {
    HashMap* hash_map = create_hash_map_with_ascending_values();

    int test_score = 0;
    test_score += hm_replace(hash_map, "zero", 10) == 0 ? 1 : 0;
    test_score += hm_replace(hash_map, "five", 15) == 5 ? 10 : 0;
    test_score += hm_replace(hash_map, "nine", 19) == 9 ? 100 : 0;
    test_score += hm_get(hash_map, "zero") == 10 ? 1000 : 0;
    test_score += hm_get(hash_map, "five") == 15 ? 10000 : 0;
    test_score += hm_get(hash_map, "nine") == 19 ? 100000 : 0;
    test_score += hash_map->size == 10 ? 2000000 : 0;

    hm_clear(hash_map);
    hm_destroy(hash_map);
    return test_score;
}

int test_clear() {
    HashMap* hash_map = create_hash_map_with_ascending_values();
    hm_clear(hash_map);

    int test_score = 0;
    test_score += hash_map->size == 0 ? 1 : 0;
    for(int i = 0; i < 10; i++)
        if(!memcmp(hash_map->buckets+i, &EMPTY_NODE, sizeof(HMNode)))
            test_score++;
    test_score += 10;

    hm_destroy(hash_map);
    return test_score;
}

int main() {
    printf("test_put_get: %d\n", test_put_get());
    printf("test_rehash: %d\n", test_rehash());
    printf("test_remove: %d\n", test_remove());
    printf("test_replace: %d\n", test_replace());
    printf("test_clear %d\n", test_clear());

    printf("\nMalloc calls: %d\nFree calls: %d\n", malloc_calls, free_calls);

    return 0;
}