#pragma once

/* ---------------- Structs ---------------- */

typedef struct AVLTreeNode {
    struct AVLTreeNode *parent, *left, *right;
    int balance;
    void* data;
} AVLTreeNode;

typedef struct {
    AVLTreeNode *root;
    size_t size;
    int(*compare)(void*, void*);
} AVLTree;


/* ---------------- Functions ---------------- */

void avlt_init(AVLTree* tree, int(*compare)(void*, void*));
void avlt_free(AVLTree* tree);
AVLTree* avlt_create(int(*compare)(void*, void*));
void avlt_destroy(AVLTree* tree);

void avlt_add(AVLTree* tree, void* data);
void* avlt_remove(AVLTree* tree, void* data);
void* avlt_get(AVLTree* tree, void* data);
int avlt_contains(AVLTree* tree, void* data);
void avlt_clear(AVLTree* tree);


/* TODO:
* - Change into key-value pairs
* - put, min, max, predecessor, successor
*/
