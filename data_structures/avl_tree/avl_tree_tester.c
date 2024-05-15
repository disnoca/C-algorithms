#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "avl_tree.h"
#include "../../wrapper_functions.h"


int values[1024];
int curr_val;

#define REF_OF(x)   ({ values[curr_val] = (x); values + curr_val++; })
#define DEREF(p)    (*((int*)(p)))


int compare(void* arg1, void* arg2) {
	if (DEREF(arg1) > DEREF(arg2)) return 1;
	if (DEREF(arg1) < DEREF(arg2)) return -1;
	return 0;
}


static AVLTree* create_avltree_with_content() {
	AVLTree* tree = avlt_create(compare);
	avlt_add(tree, REF_OF(5));
	avlt_add(tree, REF_OF(3));
	avlt_add(tree, REF_OF(7));
	avlt_add(tree, REF_OF(2));
	avlt_add(tree, REF_OF(4));
	avlt_add(tree, REF_OF(6));
	avlt_add(tree, REF_OF(8));
	return tree;
}

static void test_add_and_tree_structure() {
	AVLTree* tree = create_avltree_with_content();

	assert(DEREF(tree->root->data) == 5);
	assert(DEREF(tree->root->left->data) == 3);
	assert(DEREF(tree->root->right->data) == 7);
	assert(DEREF(tree->root->left->left->data) == 2);
	assert(DEREF(tree->root->left->right->data) == 4);
	assert(DEREF(tree->root->right->left->data) == 6);
	assert(DEREF(tree->root->right->right->data) == 8);
	assert(tree->size == 7);

	avlt_clear(tree);
	Free(tree);
}

static void test_add_and_balance_values() {
	AVLTree* tree = avlt_create(compare);

	avlt_add(tree, REF_OF(5));
	avlt_add(tree, REF_OF(3));

	assert(tree->root->balance == -1);

	avlt_add(tree, REF_OF(7));

	assert(tree->root->balance == 0);

	avlt_add(tree, REF_OF(6));

	assert(tree->root->balance == 1);
	assert(tree->root->right->balance == -1);

	avlt_add(tree, REF_OF(8));
	avlt_add(tree, REF_OF(2));

	assert(tree->root->balance == 0);
	assert(tree->root->right->balance == 0);
	assert(tree->root->left->balance == -1);

	avlt_clear(tree);
	Free(tree);
}

static void test_get() {
	AVLTree* tree = create_avltree_with_content();

	assert(DEREF(avlt_get(tree, values + curr_val - 4)) == 2);
	assert(DEREF(avlt_get(tree, values + curr_val - 6)) == 3);
	assert(DEREF(avlt_get(tree, values + curr_val - 3)) == 4);
	assert(DEREF(avlt_get(tree, values + curr_val - 7)) == 5);
	assert(DEREF(avlt_get(tree, values + curr_val - 2)) == 6);
	assert(DEREF(avlt_get(tree, values + curr_val - 5)) == 7);
	assert(DEREF(avlt_get(tree, values + curr_val - 1)) == 8);

	avlt_clear(tree);
	Free(tree);
}

static void test_contains() {
	AVLTree* tree = create_avltree_with_content();

	assert(!avlt_contains(tree, REF_OF(1)));
	for (size_t i = 0; i < tree->size; i++)
		assert(avlt_contains(tree, values + curr_val - (int) i - 2));
	assert(!avlt_contains(tree, REF_OF(9)));

	avlt_clear(tree);
	Free(tree);
}

static void test_remove_with_no_children() {
	AVLTree* tree = create_avltree_with_content();

	assert(DEREF(avlt_remove(tree, values + curr_val - 4)) == 2);
	assert(tree->root->left->left == NULL);
	assert(tree->root->left->balance == 1);
	assert(DEREF(avlt_remove(tree, values + curr_val - 3)) == 4);
	assert(tree->root->left->right == NULL);
	assert(tree->root->left->balance == 0);
	assert(tree->root->balance == 1);
	assert(tree->size == 5);

	avlt_clear(tree);
	Free(tree);
}

static void test_remove_with_single_children() {
	AVLTree* tree = create_avltree_with_content();
	avlt_add(tree, REF_OF(1));
	avlt_add(tree, REF_OF(9));

	assert(DEREF(avlt_remove(tree, values + curr_val - 6)) == 2);
	assert(DEREF(tree->root->left->left->data) == 1);
	assert(DEREF(tree->root->left->left->parent->data) == 3);
	assert(tree->root->left->balance == 0);
	assert(tree->root->balance == 1);

	assert(DEREF(avlt_remove(tree, values + curr_val - 3)) == 8);

	assert(DEREF(tree->root->right->right->data) == 9);
	assert(DEREF(tree->root->right->right->parent->data) == 7);
	assert(tree->root->right->balance == 0);
	assert(tree->root->balance == 0);
	assert(tree->size == 7);

	avlt_clear(tree);
	Free(tree);
}

static void test_remove_with_double_children() {
	AVLTree* tree = create_avltree_with_content();

	assert(DEREF(avlt_remove(tree, values + curr_val - 7)) == 5);
	assert(DEREF(tree->root->data) == 4);
	assert(tree->root->balance == 0);
	assert(tree->root->left->balance == -1);

	assert(DEREF(avlt_remove(tree, values + curr_val - 5)) == 7);

	assert(DEREF(tree->root->right->data) == 6);
	assert(tree->root->balance == 0);
	assert(tree->root->right->balance == 1);

	avlt_clear(tree);
	Free(tree);
}

static void test_left_rotation_on_add() {
	AVLTree* tree = avlt_create(compare);

	avlt_add(tree, REF_OF(5));
	avlt_add(tree, REF_OF(4));
	avlt_add(tree, REF_OF(6));
	avlt_add(tree, REF_OF(7));
	avlt_add(tree, REF_OF(8));

	assert(DEREF(tree->root->right->left->parent->right->parent->data) == 7);
	assert(DEREF(tree->root->right->left->data) == 6);
	assert(DEREF(tree->root->right->right->data) == 8);
	assert(tree->root->balance == 1);
	assert(tree->root->right->balance == 0);
	assert(tree->root->right->left->balance == 0);
	assert(tree->root->right->right->balance == 0);

	avlt_clear(tree);
	Free(tree);
}

static void test_left_rotation_on_remove() {
	AVLTree* tree = create_avltree_with_content();

	assert(DEREF(avlt_remove(tree, values + curr_val - 4)) == 2);
	assert(DEREF(avlt_remove(tree, values + curr_val - 6)) == 3);
	assert(DEREF(avlt_remove(tree, values + curr_val - 3)) == 4);

	assert(DEREF(tree->root->left->right->parent->parent->data) == 7);
	assert(DEREF(tree->root->left->data) == 5);
	assert(DEREF(tree->root->left->right->data) == 6);
	assert(DEREF(tree->root->right->data) == 8);
	assert(tree->root->balance == -1);
	assert(tree->root->left->balance == 1);
	assert(tree->root->left->right->balance == 0);
	assert(tree->root->right->balance == 0);

	avlt_clear(tree);
	Free(tree);
}

static void test_right_rotation_on_add() {
	AVLTree* tree = avlt_create(compare);

	avlt_add(tree, REF_OF(5));
	avlt_add(tree, REF_OF(6));
	avlt_add(tree, REF_OF(4));
	avlt_add(tree, REF_OF(3));
	avlt_add(tree, REF_OF(2));

	assert(DEREF(tree->root->left->left->parent->right->parent->data) == 3);
	assert(DEREF(tree->root->left->left->data) == 2);
	assert(DEREF(tree->root->left->right->data) == 4);
	assert(tree->root->balance == -1);
	assert(tree->root->left->balance == 0);
	assert(tree->root->left->left->balance == 0);
	assert(tree->root->left->right->balance == 0);

	avlt_clear(tree);
	Free(tree);
}

static void test_right_rotation_on_remove() {
	AVLTree* tree = create_avltree_with_content();

	assert(DEREF(avlt_remove(tree, values + curr_val - 2)) == 6);
	assert(DEREF(avlt_remove(tree, values + curr_val - 5)) == 7);
	assert(DEREF(avlt_remove(tree, values + curr_val - 1)) == 8);

	assert(DEREF(tree->root->right->left->parent->parent->data) == 3);
	assert(DEREF(tree->root->right->data) == 5);
	assert(DEREF(tree->root->right->left->data) == 4);
	assert(DEREF(tree->root->left->data) == 2);
	assert(tree->root->balance == 1);
	assert(tree->root->right->balance == -1);
	assert(tree->root->right->left->balance == 0);
	assert(tree->root->left->balance == 0);

	avlt_clear(tree);
	Free(tree);
}

static void test_lr_rotation_on_add() {
	AVLTree* tree = avlt_create(compare);

	avlt_add(tree, REF_OF(5));
	avlt_add(tree, REF_OF(3));
	avlt_add(tree, REF_OF(4));

	assert(DEREF(tree->root->left->parent->right->parent->data) == 4);
	assert(DEREF(tree->root->left->data) == 3);
	assert(DEREF(tree->root->right->data) == 5);
	assert(tree->root->balance == 0);
	assert(tree->root->left->balance == 0);
	assert(tree->root->right->balance == 0);

	avlt_clear(tree);
	Free(tree);
}

static void test_lr_rotation_on_remove() {
	AVLTree* tree = avlt_create(compare);

	avlt_add(tree, REF_OF(5));
	avlt_add(tree, REF_OF(2));
	avlt_add(tree, REF_OF(6));
	avlt_add(tree, REF_OF(1));
	avlt_add(tree, REF_OF(7));
	avlt_add(tree, REF_OF(4));
	avlt_add(tree, REF_OF(3));
	assert(DEREF(avlt_remove(tree, values + curr_val - 3)) == 7);

	assert(DEREF(tree->root->left->left->parent->right->parent->parent->data) == 4);
	assert(DEREF(tree->root->left->data) == 2);
	assert(DEREF(tree->root->left->right->data) == 3);
	assert(DEREF(tree->root->left->left->data) == 1);
	assert(DEREF(tree->root->right->data) == 5);
	assert(DEREF(tree->root->right->right->data) == 6);
	assert(tree->root->balance == 0);
	assert(tree->root->left->balance == 0);
	assert(tree->root->right->balance == 1);

	avlt_clear(tree);
	Free(tree);
}

static void test_rl_rotation_on_add() {
	AVLTree* tree = avlt_create(compare);

	avlt_add(tree, REF_OF(5));
	avlt_add(tree, REF_OF(7));
	avlt_add(tree, REF_OF(6));

	assert(DEREF(tree->root->left->parent->right->parent->data) == 6);
	assert(DEREF(tree->root->left->data) == 5);
	assert(DEREF(tree->root->right->data) == 7);
	assert(tree->root->balance == 0);
	assert(tree->root->left->balance == 0);
	assert(tree->root->right->balance == 0);

	avlt_clear(tree);
	Free(tree);
}

static void test_rl_rotation_on_remove() {
	AVLTree* tree = avlt_create(compare);

	avlt_add(tree, REF_OF(5));
	avlt_add(tree, REF_OF(4));
	avlt_add(tree, REF_OF(8));
	avlt_add(tree, REF_OF(3));
	avlt_add(tree, REF_OF(9));
	avlt_add(tree, REF_OF(6));
	avlt_add(tree, REF_OF(7));
	assert(DEREF(avlt_remove(tree, values + curr_val - 4)) == 3);

	assert(DEREF(tree->root->right->right->parent->left->parent->parent->data) == 6);
	assert(DEREF(tree->root->right->data) == 8);
	assert(DEREF(tree->root->right->left->data) == 7);
	assert(DEREF(tree->root->right->right->data) == 9);
	assert(DEREF(tree->root->left->data) == 5);
	assert(DEREF(tree->root->left->left->data) == 4);
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
