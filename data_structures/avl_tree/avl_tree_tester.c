#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "avl_tree.h"
#include "../../wrapper_functions.h"

static AVLTree* create_avltree_with_content() {
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

static static void test_add_and_tree_structure() {
    AVLTree* tree = create_avltree_with_content();

    assert(tree->root->data == 5);
    assert(tree->root->left->data == 3);
    assert(tree->root->right->data == 7);
    assert(tree->root->left->left->data == 2);
    assert(tree->root->left->right->data == 4);
    assert(tree->root->right->left->data == 6);
    assert(tree->root->right->right->data == 8);
    assert(tree->size == 7);

    avlt_clear(tree);
    Free(tree);
}

static void test_add_and_balance_values() {
    AVLTree* tree = avlt_create();

    avlt_add(tree, 5);
    avlt_add(tree, 3);

    assert(tree->root->balance == -1);

    avlt_add(tree, 7);

    assert(tree->root->balance == 0);

    avlt_add(tree, 6);

    assert(tree->root->balance == 1);
    assert(tree->root->right->balance == -1);

    avlt_add(tree, 8);
    avlt_add(tree, 2);

    assert(tree->root->balance == 0);
    assert(tree->root->right->balance == 0);
    assert(tree->root->left->balance == -1);

    avlt_clear(tree);
    Free(tree);
}

static void test_get() {
    AVLTree* tree = create_avltree_with_content();

    assert(avlt_get(tree, 2) == 2);
    assert(avlt_get(tree, 3) == 3);
    assert(avlt_get(tree, 4) == 4);
    assert(avlt_get(tree, 5) == 5);
    assert(avlt_get(tree, 6) == 6);
    assert(avlt_get(tree, 7) == 7);
    assert(avlt_get(tree, 8) == 8);

    avlt_clear(tree);
    Free(tree);
}

static void test_contains() {
    AVLTree* tree = create_avltree_with_content();

    assert(!avlt_contains(tree, 1));
    assert(avlt_contains(tree, 2));
    assert(avlt_contains(tree, 3));
    assert(avlt_contains(tree, 4));
    assert(avlt_contains(tree, 5));
    assert(avlt_contains(tree, 6));
    assert(avlt_contains(tree, 7));
    assert(avlt_contains(tree, 8));
    assert(!avlt_contains(tree, 9));

    avlt_clear(tree);
    Free(tree);
}

static void test_remove_with_no_children() {
    AVLTree* tree = create_avltree_with_content();

    assert(avlt_remove(tree, 2) == 2);
    assert(tree->root->left->left == NULL);
    assert(tree->root->left->balance == 1);
    assert(avlt_remove(tree, 4) == 4);
    assert(tree->root->left->right == NULL);
    assert(tree->root->left->balance == 0);
    assert(tree->root->balance == 1);
    assert(tree->size == 5);

    avlt_clear(tree);
    Free(tree);
}

static void test_remove_with_single_children() {
    AVLTree* tree = create_avltree_with_content();
    avlt_add(tree, 1);
    avlt_add(tree, 9);

    assert(avlt_remove(tree, 2) == 2);
    assert(tree->root->left->left->data == 1);
    assert(tree->root->left->left->parent->data == 3);
    assert(tree->root->left->balance == 0);
    assert(tree->root->balance == 1);

    avlt_remove(tree, 8);

    assert(tree->root->right->right->data == 9);
    assert(tree->root->right->right->parent->data == 7);
    assert(tree->root->right->balance == 0);
    assert(tree->root->balance == 0);
    assert(tree->size == 7);

    avlt_clear(tree);
    Free(tree);
}

static void test_remove_with_double_children() {
    AVLTree* tree = create_avltree_with_content();

    assert(avlt_remove(tree, 5) == 5);
    assert(tree->root->data == 4);
    assert(tree->root->balance == 0);
    assert(tree->root->left->balance == -1);

    avlt_remove(tree, 7);

    assert(tree->root->right->data == 6);
    assert(tree->root->balance == 0);
    assert(tree->root->right->balance == 1);

    avlt_clear(tree);
    Free(tree);
}

static void test_left_rotation_on_add() {
    AVLTree* tree = avlt_create();

    avlt_add(tree, 5);
    avlt_add(tree, 4);
    avlt_add(tree, 6);
    avlt_add(tree, 7);
    avlt_add(tree, 8);

    assert(tree->root->right->left->parent->right->parent->data == 7);
    assert(tree->root->right->left->data == 6);
    assert(tree->root->right->right->data == 8);
    assert(tree->root->balance == 1);
    assert(tree->root->right->balance == 0);
    assert(tree->root->right->left->balance == 0);
    assert(tree->root->right->right->balance == 0);

    avlt_clear(tree);
    Free(tree);
}

static void test_left_rotation_on_remove() {
    AVLTree* tree = create_avltree_with_content();

    avlt_remove(tree, 2);
    avlt_remove(tree, 3);
    avlt_remove(tree, 4);

    assert(tree->root->left->right->parent->parent->data == 7);
    assert(tree->root->left->data == 5);
    assert(tree->root->left->right->data == 6);
    assert(tree->root->right->data == 8);
    assert(tree->root->balance == -1);
    assert(tree->root->left->balance == 1);
    assert(tree->root->left->right->balance == 0);
    assert(tree->root->right->balance == 0);

    avlt_clear(tree);
    Free(tree);
}

static void test_right_rotation_on_add() {
    AVLTree* tree = avlt_create();

    avlt_add(tree, 5);
    avlt_add(tree, 6);
    avlt_add(tree, 4);
    avlt_add(tree, 3);
    avlt_add(tree, 2);

    assert(tree->root->left->left->parent->right->parent->data == 3);
    assert(tree->root->left->left->data == 2);
    assert(tree->root->left->right->data == 4);
    assert(tree->root->balance == -1);
    assert(tree->root->left->balance == 0);
    assert(tree->root->left->left->balance == 0);
    assert(tree->root->left->right->balance == 0);

    avlt_clear(tree);
    Free(tree);
}

static void test_right_rotation_on_remove() {
    AVLTree* tree = create_avltree_with_content();

    avlt_remove(tree, 6);
    avlt_remove(tree, 7);
    avlt_remove(tree, 8);

    assert(tree->root->right->left->parent->parent->data == 3);
    assert(tree->root->right->data == 5);
    assert(tree->root->right->left->data == 4);
    assert(tree->root->left->data == 2);
    assert(tree->root->balance == 1);
    assert(tree->root->right->balance == -1);
    assert(tree->root->right->left->balance == 0);
    assert(tree->root->left->balance == 0);

    avlt_clear(tree);
    Free(tree);
}

static void test_lr_rotation_on_add() {
    AVLTree* tree = avlt_create();

    avlt_add(tree, 5);
    avlt_add(tree, 3);
    avlt_add(tree, 4);

    assert(tree->root->left->parent->right->parent->data == 4);
    assert(tree->root->left->data == 3);
    assert(tree->root->right->data == 5);
    assert(tree->root->balance == 0);
    assert(tree->root->left->balance == 0);
    assert(tree->root->right->balance == 0);

    avlt_clear(tree);
    Free(tree);
}

static void test_lr_rotation_on_remove() {
    AVLTree* tree = avlt_create();

    avlt_add(tree, 5);
    avlt_add(tree, 2);
    avlt_add(tree, 6);
    avlt_add(tree, 1);
    avlt_add(tree, 7);
    avlt_add(tree, 4);
    avlt_add(tree, 3);
    avlt_remove(tree, 7);

    assert(tree->root->left->left->parent->right->parent->parent->data == 4);
    assert(tree->root->left->data == 2);
    assert(tree->root->left->right->data == 3);
    assert(tree->root->left->left->data == 1);
    assert(tree->root->right->data == 5);
    assert(tree->root->right->right->data == 6);
    assert(tree->root->balance == 0);
    assert(tree->root->left->balance == 0);
    assert(tree->root->right->balance == 1);

    avlt_clear(tree);
    Free(tree);
}

static void test_rl_rotation_on_add() {
    AVLTree* tree = avlt_create();

    avlt_add(tree, 5);
    avlt_add(tree, 7);
    avlt_add(tree, 6);

    assert(tree->root->left->parent->right->parent->data == 6);
    assert(tree->root->left->data == 5);
    assert(tree->root->right->data == 7);
    assert(tree->root->balance == 0);
    assert(tree->root->left->balance == 0);
    assert(tree->root->right->balance == 0);

    avlt_clear(tree);
    Free(tree);
}

static void test_rl_rotation_on_remove() {
    AVLTree* tree = avlt_create();

    avlt_add(tree, 5);
    avlt_add(tree, 4);
    avlt_add(tree, 8);
    avlt_add(tree, 3);
    avlt_add(tree, 9);
    avlt_add(tree, 6);
    avlt_add(tree, 7);
    avlt_remove(tree, 3);

    assert(tree->root->right->right->parent->left->parent->parent->data == 6);
    assert(tree->root->right->data == 8);
    assert(tree->root->right->left->data == 7);
    assert(tree->root->right->right->data == 9);
    assert(tree->root->left->data == 5);
    assert(tree->root->left->left->data == 4);
    assert(tree->root->balance == 0);
    assert(tree->root->right->balance == 0);
    assert(tree->root->left->balance == -1);

    avlt_clear(tree);
    Free(tree);
}

int main() {
    test_add_and_tree_structure();
    test_add_and_balance_values();
    test_get();
    test_contains();
    test_remove_with_no_children();
    test_remove_with_single_children();
    test_remove_with_double_children();
    test_left_rotation_on_add();
    test_left_rotation_on_remove();
    test_right_rotation_on_add();
    test_right_rotation_on_remove();
    test_lr_rotation_on_add();
    test_lr_rotation_on_remove();
    test_rl_rotation_on_add();
    test_rl_rotation_on_remove();

	assert(malloc_calls == free_calls);

	printf("All tests passed.\n");
    return 0;
}
