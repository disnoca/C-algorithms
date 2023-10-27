#ifndef HASH_MAP
#define HASH_MAP

typedef int value_type;

/* ---------------- Structs ---------------- */

typedef struct HMNode HMNode;

struct HMNode {
    HMNode* next;
    char* key;
    value_type value;
};

typedef struct {
    HMNode* buckets;    // the pointer to the first position of the array of buckets
    int capacity;       // the number of buckets
    double load_factor; // the maximum ratio of size/capacity before the hash_map is resized
    int size;           // the number of elements the hash_map currently has
} HashMap;

/* ---------------- Functions ---------------- */

HashMap* hm_create();

void hm_put(HashMap* hash_map, char* key, value_type value);

value_type hm_remove(HashMap* hash_map, char* key);

value_type hm_get(HashMap* hash_map, char* key);

value_type hm_replace(HashMap* hash_map, char* key, value_type value);

void hm_clear(HashMap* hash_map);

void hm_destroy(HashMap* hash_map);

#endif
