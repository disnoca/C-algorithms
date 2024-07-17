#pragma once

#include <stdbool.h>

/* ---------------- Structs ---------------- */

typedef struct AVLTreeNode {
    struct AVLTreeNode *parent, *left, *right;
    int balance;
    void *key, *value;
} AVLTreeNode;

typedef struct {
    AVLTreeNode *root;
    size_t size;
    int(*compare)(void*,void*);
} AVLTree;


/* ---------------- Functions ---------------- */

void avlt_init(AVLTree* tree, int(*compare)(void*,void*));
void avlt_free(AVLTree* tree, bool free_keys, bool free_values);
AVLTree* avlt_create(int(*compare)(void*, void*));
void avlt_destroy(AVLTree* tree, bool free_keys, bool free_values);

void* avlt_put(AVLTree* tree, void* key, void* value);
void* avlt_remove(AVLTree* tree, void* key);
void* avlt_get(AVLTree* tree, void* key);
void avlt_clear(AVLTree* tree, bool free_keys, bool free_values);

void* avlt_min(AVLTree* tree);
void* avlt_max(AVLTree* tree);
void* avlt_predecessor(AVLTree* tree, void* key);
void* avlt_successor(AVLTree* tree, void* key);
