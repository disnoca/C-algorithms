#pragma once

// This tree does not support duplicate elements

typedef int data_type;

/* ---------------- Structs ---------------- */

typedef struct AVLTreeNode AVLTreeNode;

struct AVLTreeNode {
    AVLTreeNode *parent, *left, *right;
    int balance;
    data_type data;
};

typedef struct {
    AVLTreeNode *root;
    int size;
} AVLTree;

/* ---------------- Functions ---------------- */

AVLTree* avlt_create();

void avlt_add(AVLTree* tree, data_type data);

data_type avlt_remove(AVLTree* tree, data_type data);

data_type avlt_get(AVLTree* tree, data_type data);

int avlt_contains(AVLTree* tree, data_type data);

void avlt_clear();

void avlt_destroy();
