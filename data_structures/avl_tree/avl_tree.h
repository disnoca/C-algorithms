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

AVLTree* avlt_create(int(*compare)(void*, void*));
void avlt_destroy();
void avlt_clear();

void avlt_add(AVLTree* tree, void* data);
void* avlt_remove(AVLTree* tree, void* data);
void* avlt_get(AVLTree* tree, void* data);
int avlt_contains(AVLTree* tree, void* data);


/* TODO:
* - Change into key-value pairs
* - init, free 
* - put, min, max, predecessor, successor
*/
