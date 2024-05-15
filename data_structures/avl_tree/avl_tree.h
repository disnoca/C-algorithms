#pragma once

// This tree does not support duplicate elements


/* ---------------- Structs ---------------- */

typedef struct AVLTreeNode AVLTreeNode;

struct AVLTreeNode {
    AVLTreeNode *parent, *left, *right;
    int balance;
    void* data;
};

typedef struct {
    AVLTreeNode *root;
    size_t size;
    int(*compare)(void*, void*);
} AVLTree;

/* ---------------- Functions ---------------- */

AVLTree* avlt_create(int(*compare)(void*, void*));

void avlt_add(AVLTree* tree, void* data);

void* avlt_remove(AVLTree* tree, void* data);

void* avlt_get(AVLTree* tree, void* data);

int avlt_contains(AVLTree* tree, void* data);

void avlt_clear();

void avlt_destroy();
