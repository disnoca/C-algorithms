#include <stdio.h>
#include <string.h>
#include "avl_tree.h"
#include "..\..\wrapper_functions.h"

AVLTree* create_avltree_with_content() {
    AVLTree* tree = avlt_create();
    avlt_add(tree, 5);
    avlt_add(tree, 3);
    avlt_add(tree, 7);
    avlt_add(tree, 2);
    avlt_add(tree, 4);
    avlt_add(tree, 6);
    avlt_add(tree, 8);
    return tree;
}

int test_add_and_tree_structure() {
    AVLTree* tree = create_avltree_with_content();

    int test_score = 0;
    test_score += tree->root->data == 5 ? 1 : 0;
    test_score += tree->root->left->data == 3 ? 10 : 0;
    test_score += tree->root->right->data == 7 ? 100 : 0;
    test_score += tree->root->left->left->data == 2 ? 1000 : 0;
    test_score += tree->root->left->right->data == 4 ? 10000 : 0;
    test_score += tree->root->right->left->data == 6 ? 100000 : 0;
    test_score += tree->root->right->right->data == 8 ? 1000000 : 0;
    test_score += tree->size == 7 ? 20000000 : 0;

    avlt_clear(tree);
    Free(tree);
    return test_score;
}

int test_add_and_balance_values() {
    AVLTree* tree = avlt_create();

    avlt_add(tree, 5);
    avlt_add(tree, 3);

    int test_score = 0;
    test_score += tree->root->balance == -1 ? 1 : 0;

    avlt_add(tree, 7);

    test_score += tree->root->balance == 0 ? 10 : 0;

    avlt_add(tree, 6);

    test_score += tree->root->balance == 1 ? 100 : 0;
    test_score += tree->root->right->balance == -1 ? 1000 : 0;

    avlt_add(tree, 8);
    avlt_add(tree, 2);

    test_score += tree->root->balance == 0 ? 10000 : 0;
    test_score += tree->root->right->balance == 0 ? 100000 : 0;
    test_score += tree->root->left->balance == -1 ? 2000000 : 0;

    avlt_clear(tree);
    Free(tree);
    return test_score;
}

int test_get() {
    AVLTree* tree = create_avltree_with_content();

    int test_score = 0;
    test_score += avlt_get(tree, 2) == 2 ? 1 : 0;
    test_score += avlt_get(tree, 3) == 3 ? 10 : 0;
    test_score += avlt_get(tree, 4) == 4 ? 100 : 0;
    test_score += avlt_get(tree, 5) == 5 ? 1000 : 0;
    test_score += avlt_get(tree, 6) == 6 ? 10000 : 0;
    test_score += avlt_get(tree, 7) == 7 ? 100000 : 0;
    test_score += avlt_get(tree, 8) == 8 ? 2000000 : 0;

    avlt_clear(tree);
    Free(tree);
    return test_score;
}

int test_contains() {
    AVLTree* tree = create_avltree_with_content();

    int test_score = 0;
    test_score += !avlt_contains(tree, 1) ? 1 : 0;
    test_score += avlt_contains(tree, 2) ? 10 : 0;
    test_score += avlt_contains(tree, 3) ? 100 : 0;
    test_score += avlt_contains(tree, 4) ? 1000 : 0;
    test_score += avlt_contains(tree, 5) ? 10000 : 0;
    test_score += avlt_contains(tree, 6) ? 100000 : 0;
    test_score += avlt_contains(tree, 7) ? 1000000 : 0;
    test_score += avlt_contains(tree, 8) ? 10000000 : 0;
    test_score += !avlt_contains(tree, 9) ? 200000000 : 0;

    avlt_clear(tree);
    Free(tree);
    return test_score;
}

int test_remove_with_no_children() {
    AVLTree* tree = create_avltree_with_content();

    int test_score = 0;
    test_score += avlt_remove(tree, 2) == 2 ? 1 : 0;
    test_score += tree->root->left->left == NULL ? 10 : 0;
    test_score += tree->root->left->balance == 1 ? 100 : 0;
    test_score += avlt_remove(tree, 4) == 4 ? 1000 : 0;
    test_score += tree->root->left->right == NULL ? 10000 : 0;
    test_score += tree->root->left->balance == 0 ? 100000 : 0;
    test_score += tree->root->balance == 1 ? 1000000 : 0;
    test_score += tree->size == 5 ? 20000000 : 0;

    avlt_clear(tree);
    Free(tree);
    return test_score;
}

int test_remove_with_single_children() {
    AVLTree* tree = create_avltree_with_content();
    avlt_add(tree, 1);
    avlt_add(tree, 9);

    int test_score = 0;
    test_score += avlt_remove(tree, 2) == 2 ? 1 : 0;
    test_score += tree->root->left->left->data == 1 ? 10 : 0;
    test_score += tree->root->left->left->parent->data == 3 ? 100 : 0;
    test_score += tree->root->left->balance == 0 ? 1000 : 0;
    test_score += tree->root->balance == 1 ? 10000 : 0;

    avlt_remove(tree, 8);

    test_score += tree->root->right->right->data == 9 ? 100000 : 0;
    test_score += tree->root->right->right->parent->data == 7 ? 1000000 : 0;
    test_score += tree->root->right->balance == 0 ? 10000000 : 0;
    test_score += tree->root->balance == 0 ? 100000000 : 0;
    test_score += tree->size == 7 ? 2000000000 : 0;

    avlt_clear(tree);
    Free(tree);
    return test_score;
}

int test_remove_with_double_children() {
    AVLTree* tree = create_avltree_with_content();

    int test_score = 0;
    test_score += avlt_remove(tree, 5) == 5 ? 1 : 0;
    test_score += tree->root->data == 4 ? 10 : 0;
    test_score += tree->root->balance == 0 ? 100 : 0;
    test_score += tree->root->left->balance == -1 ? 1000 : 0;

    avlt_remove(tree, 7);

    test_score += tree->root->right->data == 6 ? 10000 : 0;
    test_score += tree->root->balance == 0 ? 100000 : 0;
    test_score += tree->root->right->balance == 1 ? 2000000 : 0;

    avlt_clear(tree);
    Free(tree);
    return test_score;
}

int test_left_rotation_on_add() {
    AVLTree* tree = avlt_create();

    avlt_add(tree, 5);
    avlt_add(tree, 4);
    avlt_add(tree, 6);
    avlt_add(tree, 7);
    avlt_add(tree, 8);

    int test_score = 0;
    test_score += tree->root->right->left->parent->right->parent->data == 7 ? 1 : 0;
    test_score += tree->root->right->left->data == 6 ? 10 : 0;
    test_score += tree->root->right->right->data == 8 ? 100 : 0;
    test_score += tree->root->balance == 1 ? 1000 : 0;
    test_score += tree->root->right->balance == 0 ? 10000 : 0;
    test_score += tree->root->right->left->balance == 0 ? 100000 : 0;
    test_score += tree->root->right->right->balance == 0 ? 2000000 : 0;

    avlt_clear(tree);
    Free(tree);
    return test_score;
}

int test_left_rotation_on_remove() {
    AVLTree* tree = create_avltree_with_content();

    avlt_remove(tree, 2);
    avlt_remove(tree, 3);
    avlt_remove(tree, 4);

    int test_score = 0;
    test_score += tree->root->left->right->parent->parent->data == 7 ? 1 : 0;
    test_score += tree->root->left->data == 5 ? 10 : 0;
    test_score += tree->root->left->right->data == 6 ? 100 : 0;
    test_score += tree->root->right->data == 8 ? 1000 : 0;
    test_score += tree->root->balance == -1 ? 10000 : 0;
    test_score += tree->root->left->balance == 1 ? 100000 : 0;
    test_score += tree->root->left->right->balance == 0 ? 1000000 : 0;
    test_score += tree->root->right->balance == 0 ? 20000000 : 0;

    avlt_clear(tree);
    Free(tree);
    return test_score;
}

int test_right_rotation_on_add() {
    AVLTree* tree = avlt_create();

    avlt_add(tree, 5);
    avlt_add(tree, 6);
    avlt_add(tree, 4);
    avlt_add(tree, 3);
    avlt_add(tree, 2);

    int test_score = 0;
    test_score += tree->root->left->left->parent->right->parent->data == 3 ? 1 : 0;
    test_score += tree->root->left->left->data == 2 ? 10 : 0;
    test_score += tree->root->left->right->data == 4 ? 100 : 0;
    test_score += tree->root->balance == -1 ? 1000 : 0;
    test_score += tree->root->left->balance == 0 ? 10000 : 0;
    test_score += tree->root->left->left->balance == 0 ? 100000 : 0;
    test_score += tree->root->left->right->balance == 0 ? 2000000 : 0;

    avlt_clear(tree);
    Free(tree);
    return test_score;
}

int test_right_rotation_on_remove() {
    AVLTree* tree = create_avltree_with_content();

    avlt_remove(tree, 6);
    avlt_remove(tree, 7);
    avlt_remove(tree, 8);

    int test_score = 0;
    test_score += tree->root->right->left->parent->parent->data == 3 ? 1 : 0;
    test_score += tree->root->right->data == 5 ? 10 : 0;
    test_score += tree->root->right->left->data == 4 ? 100 : 0;
    test_score += tree->root->left->data == 2 ? 1000 : 0;
    test_score += tree->root->balance == 1 ? 10000 : 0;
    test_score += tree->root->right->balance == -1 ? 100000 : 0;
    test_score += tree->root->right->left->balance == 0 ? 1000000 : 0;
    test_score += tree->root->left->balance == 0 ? 20000000 : 0;

    avlt_clear(tree);
    Free(tree);
    return test_score;
}

int test_lr_rotation_on_add() {
    AVLTree* tree = avlt_create();

    avlt_add(tree, 5);
    avlt_add(tree, 3);
    avlt_add(tree, 4);

    int test_score = 0;
    test_score += tree->root->left->parent->right->parent->data == 4 ? 1 : 0;
    test_score += tree->root->left->data == 3 ? 10 : 0;
    test_score += tree->root->right->data == 5 ? 100 : 0;
    test_score += tree->root->balance == 0 ? 1000 : 0;
    test_score += tree->root->left->balance == 0 ? 10000 : 0;
    test_score += tree->root->right->balance == 0 ? 200000 : 0;

    avlt_clear(tree);
    Free(tree);
    return test_score;
}

int test_lr_rotation_on_remove() {
    AVLTree* tree = avlt_create();

    avlt_add(tree, 5);
    avlt_add(tree, 2);
    avlt_add(tree, 6);
    avlt_add(tree, 1);
    avlt_add(tree, 7);
    avlt_add(tree, 4);
    avlt_add(tree, 3);
    avlt_remove(tree, 7);

    int test_score = 0;
    test_score += tree->root->left->left->parent->right->parent->parent->data == 4 ? 1 : 0;
    test_score += tree->root->left->data == 2 ? 10 : 0;
    test_score += tree->root->left->right->data == 3 ? 100 : 0;
    test_score += tree->root->left->left->data == 1 ? 1000 : 0;
    test_score += tree->root->right->data == 5 ? 10000 : 0;
    test_score += tree->root->right->right->data == 6 ? 100000 : 0;
    test_score += tree->root->balance == 0 ? 1000000 : 0;
    test_score += tree->root->left->balance == 0 ? 10000000 : 0;
    test_score += tree->root->right->balance == 1 ? 200000000 : 0;

    avlt_clear(tree);
    Free(tree);
    return test_score;
}

int test_rl_rotation_on_add() {
    AVLTree* tree = avlt_create();

    avlt_add(tree, 5);
    avlt_add(tree, 7);
    avlt_add(tree, 6);

    int test_score = 0;
    test_score += tree->root->left->parent->right->parent->data == 6 ? 1 : 0;
    test_score += tree->root->left->data == 5 ? 10 : 0;
    test_score += tree->root->right->data == 7 ? 100 : 0;
    test_score += tree->root->balance == 0 ? 1000 : 0;
    test_score += tree->root->left->balance == 0 ? 10000 : 0;
    test_score += tree->root->right->balance == 0 ? 200000 : 0;

    avlt_clear(tree);
    Free(tree);
    return test_score;
}

int test_rl_rotation_on_remove() {
    AVLTree* tree = avlt_create();

    avlt_add(tree, 5);
    avlt_add(tree, 4);
    avlt_add(tree, 8);
    avlt_add(tree, 3);
    avlt_add(tree, 9);
    avlt_add(tree, 6);
    avlt_add(tree, 7);
    avlt_remove(tree, 3);

    int test_score = 0;
    test_score += tree->root->right->right->parent->left->parent->parent->data == 6 ? 1 : 0;
    test_score += tree->root->right->data == 8 ? 10 : 0;
    test_score += tree->root->right->left->data == 7 ? 100 : 0;
    test_score += tree->root->right->right->data == 9 ? 1000 : 0;
    test_score += tree->root->left->data == 5 ? 10000 : 0;
    test_score += tree->root->left->left->data == 4 ? 100000 : 0;
    test_score += tree->root->balance == 0 ? 1000000 : 0;
    test_score += tree->root->right->balance == 0 ? 10000000 : 0;
    test_score += tree->root->left->balance == -1 ? 200000000 : 0;

    avlt_clear(tree);
    Free(tree);
    return test_score;
}

int main() {
    printf("test_add_and_tree_structure: %d\n", test_add_and_tree_structure());
    printf("test_add_and_balance_values: %d\n", test_add_and_balance_values());
    printf("test_get: %d\n", test_get());
    printf("test_contains: %d\n", test_contains());
    printf("test_remove_with_no_children: %d\n", test_remove_with_no_children());
    printf("test_remove_with_single_children: %d\n", test_remove_with_single_children());
    printf("test_remove_with_double_children: %d\n", test_remove_with_double_children());
    printf("test_left_rotation_on_add: %d\n", test_left_rotation_on_add());
    printf("test_left_rotation_on_remove: %d\n", test_left_rotation_on_remove());
    printf("test_right_rotation_on_add: %d\n", test_right_rotation_on_add());
    printf("test_right_rotation_on_remove: %d\n", test_right_rotation_on_remove());
    printf("test_lr_rotation_on_add: %d\n", test_lr_rotation_on_add());
    printf("test_lr_rotation_on_remove: %d\n", test_lr_rotation_on_remove());
    printf("test_rl_rotation_on_add: %d\n", test_rl_rotation_on_add());
    printf("test_rl_rotation_on_remove: %d\n", test_rl_rotation_on_remove());

    printf("\nMalloc calls: %d\nFree calls: %d\n", malloc_calls, free_calls);
}