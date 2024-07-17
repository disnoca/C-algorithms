#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "avl_tree.h"
#include "../../wrapper_functions.h"


char keys[8192];
int values[1024];
int curr_key, curr_val;

#define REF_OF_KEY(s)   ({ __typeof__ (s) _s = (s); char* key_addr = keys + curr_key; strcpy(key_addr, _s); curr_key += strlen(_s) + 1; key_addr; })
#define REF_OF_VAL(x)   ({ values[curr_val] = (x); values + curr_val++; })
#define DEREF_VAL(p)    ({ __typeof__ (p) _p = (p); assert((_p) != NULL); *((int*)(_p)); })


int compare(void* key1, void* key2)
{
	int val1 = atoi(key1);
	int val2 = atoi(key2);

	if (val1 > val2) return 1;
	if (val1 < val2) return -1;

	return 0;
}


static AVLTree* create_avltree_with_content()
{
	AVLTree* tree = avlt_create(compare);
	avlt_put(tree, REF_OF_KEY("5"), REF_OF_VAL(5));
	avlt_put(tree, REF_OF_KEY("3"), REF_OF_VAL(3));
	avlt_put(tree, REF_OF_KEY("7"), REF_OF_VAL(7));
	avlt_put(tree, REF_OF_KEY("2"), REF_OF_VAL(2));
	avlt_put(tree, REF_OF_KEY("4"), REF_OF_VAL(4));
	avlt_put(tree, REF_OF_KEY("6"), REF_OF_VAL(6));
	avlt_put(tree, REF_OF_KEY("8"), REF_OF_VAL(8));
	return tree;
}

static void test_add_and_tree_structure()
{
	AVLTree* tree = create_avltree_with_content();

	assert(DEREF_VAL(tree->root->value) == 5);
	assert(DEREF_VAL(tree->root->left->value) == 3);
	assert(DEREF_VAL(tree->root->right->value) == 7);
	assert(DEREF_VAL(tree->root->left->left->value) == 2);
	assert(DEREF_VAL(tree->root->left->right->value) == 4);
	assert(DEREF_VAL(tree->root->right->left->value) == 6);
	assert(DEREF_VAL(tree->root->right->right->value) == 8);

	assert(avlt_put(tree, REF_OF_KEY("0"), REF_OF_VAL(0)) == NULL);
	assert(DEREF_VAL(avlt_put(tree, REF_OF_KEY("0"), REF_OF_VAL(10))) == 0);
	assert(DEREF_VAL(avlt_put(tree, REF_OF_KEY("4"), REF_OF_VAL(40))) == 4);
	assert(DEREF_VAL(avlt_put(tree, REF_OF_KEY("4"), REF_OF_VAL(4))) == 40);

	assert(tree->size == 8);

	avlt_destroy(tree);
}

static void test_add_and_balance_values()
{
	AVLTree* tree = avlt_create(compare);

	avlt_put(tree, REF_OF_KEY("5"), REF_OF_VAL(5));
	avlt_put(tree, REF_OF_KEY("3"), REF_OF_VAL(3));

	assert(tree->root->balance == -1);

	avlt_put(tree, REF_OF_KEY("7"), REF_OF_VAL(7));

	assert(tree->root->balance == 0);

	avlt_put(tree, REF_OF_KEY("6"), REF_OF_VAL(6));

	assert(tree->root->balance == 1);
	assert(tree->root->right->balance == -1);

	avlt_put(tree, REF_OF_KEY("8"), REF_OF_VAL(8));
	avlt_put(tree, REF_OF_KEY("2"), REF_OF_VAL(2));

	assert(tree->root->balance == 0);
	assert(tree->root->right->balance == 0);
	assert(tree->root->left->balance == -1);

	avlt_destroy(tree);
}

static void test_get()
{
	AVLTree* tree = create_avltree_with_content();

	assert(DEREF_VAL(avlt_get(tree, REF_OF_KEY("2"))) == 2);
	assert(DEREF_VAL(avlt_get(tree, REF_OF_KEY("3"))) == 3);
	assert(DEREF_VAL(avlt_get(tree, REF_OF_KEY("4"))) == 4);
	assert(DEREF_VAL(avlt_get(tree, REF_OF_KEY("5"))) == 5);
	assert(DEREF_VAL(avlt_get(tree, REF_OF_KEY("6"))) == 6);
	assert(DEREF_VAL(avlt_get(tree, REF_OF_KEY("7"))) == 7);
	assert(DEREF_VAL(avlt_get(tree, REF_OF_KEY("8"))) == 8);
	assert(avlt_get(tree, REF_OF_KEY("0")) == 0);

	avlt_destroy(tree);
}

static void test_remove_with_no_children()
{
	AVLTree* tree = create_avltree_with_content();

	assert(DEREF_VAL(avlt_remove(tree, REF_OF_KEY("2"))) == 2);
	assert(tree->root->left->left == NULL);
	assert(tree->root->left->balance == 1);
	assert(DEREF_VAL(avlt_remove(tree, REF_OF_KEY("4"))) == 4);
	assert(tree->root->left->right == NULL);
	assert(tree->root->left->balance == 0);
	assert(tree->root->balance == 1);
	assert(avlt_remove(tree, REF_OF_KEY("0")) == NULL);
	assert(tree->size == 5);

	avlt_destroy(tree);
}

static void test_remove_with_single_children()
{
	AVLTree* tree = create_avltree_with_content();
	avlt_put(tree, REF_OF_KEY("1"), REF_OF_VAL(1));
	avlt_put(tree, REF_OF_KEY("9"), REF_OF_VAL(9));

	assert(DEREF_VAL(avlt_remove(tree, REF_OF_KEY("2"))) == 2);
	assert(DEREF_VAL(tree->root->left->left->value) == 1);
	assert(DEREF_VAL(tree->root->left->left->parent->value) == 3);
	assert(tree->root->left->balance == 0);
	assert(tree->root->balance == 1);

	assert(DEREF_VAL(avlt_remove(tree, REF_OF_KEY("8"))) == 8);

	assert(DEREF_VAL(tree->root->right->right->value) == 9);
	assert(DEREF_VAL(tree->root->right->right->parent->value) == 7);
	assert(tree->root->right->balance == 0);
	assert(tree->root->balance == 0);
	assert(tree->size == 7);

	avlt_destroy(tree);
}

static void test_remove_with_double_children()
{
	AVLTree* tree = create_avltree_with_content();

	assert(DEREF_VAL(avlt_remove(tree, REF_OF_KEY("5"))) == 5);
	assert(DEREF_VAL(tree->root->value) == 4);
	assert(tree->root->balance == 0);
	assert(tree->root->left->balance == -1);

	assert(DEREF_VAL(avlt_remove(tree, REF_OF_KEY("7"))) == 7);

	assert(DEREF_VAL(tree->root->right->value) == 6);
	assert(tree->root->balance == 0);
	assert(tree->root->right->balance == 1);

	avlt_destroy(tree);
}

static void test_left_rotation_on_add()
{
	AVLTree* tree = avlt_create(compare);

	avlt_put(tree, REF_OF_KEY("5"), REF_OF_VAL(5));
	avlt_put(tree, REF_OF_KEY("4"), REF_OF_VAL(4));
	avlt_put(tree, REF_OF_KEY("6"), REF_OF_VAL(6));
	avlt_put(tree, REF_OF_KEY("7"), REF_OF_VAL(7));
	avlt_put(tree, REF_OF_KEY("8"), REF_OF_VAL(8));

	assert(DEREF_VAL(tree->root->right->left->parent->right->parent->value) == 7);
	assert(DEREF_VAL(tree->root->right->left->value) == 6);
	assert(DEREF_VAL(tree->root->right->right->value) == 8);
	assert(tree->root->balance == 1);
	assert(tree->root->right->balance == 0);
	assert(tree->root->right->left->balance == 0);
	assert(tree->root->right->right->balance == 0);

	avlt_destroy(tree);
}

static void test_left_rotation_on_remove()
{
	AVLTree* tree = create_avltree_with_content();

	assert(DEREF_VAL(avlt_remove(tree, REF_OF_KEY("2"))) == 2);
	assert(DEREF_VAL(avlt_remove(tree, REF_OF_KEY("3"))) == 3);
	assert(DEREF_VAL(avlt_remove(tree, REF_OF_KEY("4"))) == 4);

	assert(DEREF_VAL(tree->root->left->right->parent->parent->value) == 7);
	assert(DEREF_VAL(tree->root->left->value) == 5);
	assert(DEREF_VAL(tree->root->left->right->value) == 6);
	assert(DEREF_VAL(tree->root->right->value) == 8);
	assert(tree->root->balance == -1);
	assert(tree->root->left->balance == 1);
	assert(tree->root->left->right->balance == 0);
	assert(tree->root->right->balance == 0);

	avlt_destroy(tree);
}

static void test_right_rotation_on_add()
{
	AVLTree* tree = avlt_create(compare);

	avlt_put(tree, REF_OF_KEY("5"), REF_OF_VAL(5));
	avlt_put(tree, REF_OF_KEY("6"), REF_OF_VAL(6));
	avlt_put(tree, REF_OF_KEY("4"), REF_OF_VAL(4));
	avlt_put(tree, REF_OF_KEY("3"), REF_OF_VAL(3));
	avlt_put(tree, REF_OF_KEY("2"), REF_OF_VAL(2));

	assert(DEREF_VAL(tree->root->left->left->parent->right->parent->value) == 3);
	assert(DEREF_VAL(tree->root->left->left->value) == 2);
	assert(DEREF_VAL(tree->root->left->right->value) == 4);
	assert(tree->root->balance == -1);
	assert(tree->root->left->balance == 0);
	assert(tree->root->left->left->balance == 0);
	assert(tree->root->left->right->balance == 0);

	avlt_destroy(tree);
}

static void test_right_rotation_on_remove()
{
	AVLTree* tree = create_avltree_with_content();

	assert(DEREF_VAL(avlt_remove(tree, REF_OF_KEY("6"))) == 6);
	assert(DEREF_VAL(avlt_remove(tree, REF_OF_KEY("7"))) == 7);
	assert(DEREF_VAL(avlt_remove(tree, REF_OF_KEY("8"))) == 8);

	assert(DEREF_VAL(tree->root->right->left->parent->parent->value) == 3);
	assert(DEREF_VAL(tree->root->right->value) == 5);
	assert(DEREF_VAL(tree->root->right->left->value) == 4);
	assert(DEREF_VAL(tree->root->left->value) == 2);
	assert(tree->root->balance == 1);
	assert(tree->root->right->balance == -1);
	assert(tree->root->right->left->balance == 0);
	assert(tree->root->left->balance == 0);

	avlt_destroy(tree);
}

static void test_lr_rotation_on_add()
{
	AVLTree* tree = avlt_create(compare);

	avlt_put(tree, REF_OF_KEY("5"), REF_OF_VAL(5));
	avlt_put(tree, REF_OF_KEY("3"), REF_OF_VAL(3));
	avlt_put(tree, REF_OF_KEY("4"), REF_OF_VAL(4));

	assert(DEREF_VAL(tree->root->left->parent->right->parent->value) == 4);
	assert(DEREF_VAL(tree->root->left->value) == 3);
	assert(DEREF_VAL(tree->root->right->value) == 5);
	assert(tree->root->balance == 0);
	assert(tree->root->left->balance == 0);
	assert(tree->root->right->balance == 0);

	avlt_destroy(tree);
}

static void test_lr_rotation_on_remove()
{
	AVLTree* tree = avlt_create(compare);

	avlt_put(tree, REF_OF_KEY("5"), REF_OF_VAL(5));
	avlt_put(tree, REF_OF_KEY("2"), REF_OF_VAL(2));
	avlt_put(tree, REF_OF_KEY("6"), REF_OF_VAL(6));
	avlt_put(tree, REF_OF_KEY("1"), REF_OF_VAL(1));
	avlt_put(tree, REF_OF_KEY("7"), REF_OF_VAL(7));
	avlt_put(tree, REF_OF_KEY("4"), REF_OF_VAL(4));
	avlt_put(tree, REF_OF_KEY("3"), REF_OF_VAL(3));
	assert(DEREF_VAL(avlt_remove(tree, REF_OF_KEY("7"))) == 7);

	assert(DEREF_VAL(tree->root->left->left->parent->right->parent->parent->value) == 4);
	assert(DEREF_VAL(tree->root->left->value) == 2);
	assert(DEREF_VAL(tree->root->left->right->value) == 3);
	assert(DEREF_VAL(tree->root->left->left->value) == 1);
	assert(DEREF_VAL(tree->root->right->value) == 5);
	assert(DEREF_VAL(tree->root->right->right->value) == 6);
	assert(tree->root->balance == 0);
	assert(tree->root->left->balance == 0);
	assert(tree->root->right->balance == 1);

	avlt_destroy(tree);
}

static void test_rl_rotation_on_add()
{
	AVLTree* tree = avlt_create(compare);

	avlt_put(tree, REF_OF_KEY("5"), REF_OF_VAL(5));
	avlt_put(tree, REF_OF_KEY("7"), REF_OF_VAL(7));
	avlt_put(tree, REF_OF_KEY("6"), REF_OF_VAL(6));

	assert(DEREF_VAL(tree->root->left->parent->right->parent->value) == 6);
	assert(DEREF_VAL(tree->root->left->value) == 5);
	assert(DEREF_VAL(tree->root->right->value) == 7);
	assert(tree->root->balance == 0);
	assert(tree->root->left->balance == 0);
	assert(tree->root->right->balance == 0);

	avlt_destroy(tree);
}

static void test_rl_rotation_on_remove()
{
	AVLTree* tree = avlt_create(compare);

	avlt_put(tree, REF_OF_KEY("5"), REF_OF_VAL(5));
	avlt_put(tree, REF_OF_KEY("4"), REF_OF_VAL(4));
	avlt_put(tree, REF_OF_KEY("8"), REF_OF_VAL(8));
	avlt_put(tree, REF_OF_KEY("3"), REF_OF_VAL(3));
	avlt_put(tree, REF_OF_KEY("9"), REF_OF_VAL(9));
	avlt_put(tree, REF_OF_KEY("6"), REF_OF_VAL(6));
	avlt_put(tree, REF_OF_KEY("7"), REF_OF_VAL(7));
	assert(DEREF_VAL(avlt_remove(tree, REF_OF_KEY("3"))) == 3);

	assert(DEREF_VAL(tree->root->right->right->parent->left->parent->parent->value) == 6);
	assert(DEREF_VAL(tree->root->right->value) == 8);
	assert(DEREF_VAL(tree->root->right->left->value) == 7);
	assert(DEREF_VAL(tree->root->right->right->value) == 9);
	assert(DEREF_VAL(tree->root->left->value) == 5);
	assert(DEREF_VAL(tree->root->left->left->value) == 4);
	assert(tree->root->balance == 0);
	assert(tree->root->right->balance == 0);
	assert(tree->root->left->balance == -1);

	avlt_destroy(tree);
}

static void test_min_max()
{
	AVLTree* tree = avlt_create(compare);

	assert(avlt_min(tree) == NULL);
	assert(avlt_max(tree) == NULL);

	avlt_put(tree, REF_OF_KEY("5"), REF_OF_VAL(5));
	avlt_put(tree, REF_OF_KEY("4"), REF_OF_VAL(4));
	avlt_put(tree, REF_OF_KEY("8"), REF_OF_VAL(8));
	assert(DEREF_VAL(avlt_min(tree)) == 4);
	assert(DEREF_VAL(avlt_max(tree)) == 8);

	avlt_put(tree, REF_OF_KEY("3"), REF_OF_VAL(3));
	avlt_put(tree, REF_OF_KEY("9"), REF_OF_VAL(9));
	avlt_put(tree, REF_OF_KEY("6"), REF_OF_VAL(6));
	avlt_put(tree, REF_OF_KEY("7"), REF_OF_VAL(7));
	assert(DEREF_VAL(avlt_min(tree)) == 3);
	assert(DEREF_VAL(avlt_max(tree)) == 9);

	avlt_destroy(tree);
}

static void test_predecessor_successor()
{
	AVLTree* tree = avlt_create(compare);

	assert(avlt_predecessor(tree, "5") == NULL);
	assert(avlt_successor(tree, "5") == NULL);

	avlt_put(tree, REF_OF_KEY("5"), REF_OF_VAL(5));
	avlt_put(tree, REF_OF_KEY("4"), REF_OF_VAL(4));
	avlt_put(tree, REF_OF_KEY("8"), REF_OF_VAL(8));
	assert(DEREF_VAL(avlt_predecessor(tree, "5")) == 4);
	assert(DEREF_VAL(avlt_successor(tree, "5")) == 8);
	assert(DEREF_VAL(avlt_predecessor(tree, "9")) == 8);
	assert(DEREF_VAL(avlt_successor(tree, "0")) == 4);
	assert(avlt_predecessor(tree, "4") == NULL);
	assert(avlt_successor(tree, "8") == NULL);
	assert(avlt_predecessor(tree, "0") == NULL);
	assert(avlt_successor(tree, "9") == NULL);

	avlt_put(tree, REF_OF_KEY("3"), REF_OF_VAL(3));
	avlt_put(tree, REF_OF_KEY("9"), REF_OF_VAL(9));
	avlt_put(tree, REF_OF_KEY("6"), REF_OF_VAL(6));
	avlt_put(tree, REF_OF_KEY("7"), REF_OF_VAL(7));
	assert(DEREF_VAL(avlt_predecessor(tree, "5")) == 4);
	assert(DEREF_VAL(avlt_successor(tree, "5")) == 6);
	assert(DEREF_VAL(avlt_predecessor(tree, "8")) == 7);
	assert(DEREF_VAL(avlt_successor(tree, "8")) == 9);
	assert(DEREF_VAL(avlt_predecessor(tree, "10")) == 9);
	assert(DEREF_VAL(avlt_successor(tree, "0")) == 3);
	assert(avlt_successor(tree, "9") == NULL);
	assert(avlt_predecessor(tree, "3") == NULL);

	avlt_destroy(tree);
}

int main()
{
	test_add_and_tree_structure();
	test_add_and_balance_values();
	test_get();
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
	test_min_max();
	test_predecessor_successor();

	assert(malloc_calls == free_calls);

	printf("All tests passed.\n");
	return 0;
}
