/**
 * AVL Tree implementation.
 * 
 * @author Samuel Pires
*/

#include <stdio.h>
#include <stdlib.h>
#include "avl_tree.h"
#include "../../wrapper_functions.h"


#define NORMALIZE_COMPARE(x) ({ int _x = (x); _x = _x > 0 ? 1 : _x; _x = _x < 0 ? -1 : _x; _x; })


/* ---------------- Tree Balance Functions ---------------- */

static void replace_parents_child(AVLTreeNode* old_child, AVLTreeNode* new_child) {
	AVLTreeNode* parent = old_child->parent;
	if (parent->left == old_child)
		parent->left = new_child;
	else
		parent->right = new_child;
}

static void perform_left_rotation(AVLTree* tree, AVLTreeNode* node) {
	AVLTreeNode* parent_node = node->parent;
	AVLTreeNode* sub_node = node->right;

	if (parent_node == NULL)
		tree->root = sub_node;
	else
		replace_parents_child(node, sub_node);

	sub_node->parent = parent_node;
	node->parent = sub_node;

	node->right = sub_node->left;
	if (node->right != NULL)
		node->right->parent = node;
	sub_node->left = node;

	if (sub_node->balance == 0) {
		node->balance = 1;
		sub_node->balance = -1;
	} 
	else {
		node->balance = 0;
		sub_node->balance = 0;
	}
}

static void perform_right_rotation(AVLTree* tree, AVLTreeNode* node) {
	AVLTreeNode* parent_node = node->parent;
	AVLTreeNode* sub_node = node->left;

	if (parent_node == NULL)
		tree->root = sub_node;
	else
		replace_parents_child(node, sub_node);

	sub_node->parent = parent_node;
	node->parent = sub_node;

	node->left = sub_node->right;
	if (node->left != NULL)
		node->left->parent = node;
	sub_node->right = node;

	if (sub_node->balance == 0) {
		node->balance = -1;
		sub_node->balance = 1;
	} 
	else {
		node->balance = 0;
		sub_node->balance = 0;
	}
}

static void perform_LR_rotation(AVLTree* tree, AVLTreeNode* node) {
	AVLTreeNode* parent_node = node->parent;
	AVLTreeNode* sub_node = node->left;
	AVLTreeNode* sub_sub_node = sub_node->right;

	if (parent_node == NULL)
		tree->root = sub_sub_node;
	else
		replace_parents_child(node, sub_sub_node);

	sub_sub_node->parent = parent_node;
	sub_node->parent = sub_sub_node;
	node->parent = sub_sub_node;

	sub_node->right = sub_sub_node->left;
	if (sub_node->right != NULL)
		sub_node->right->parent = sub_node;
	node->left = sub_sub_node->right;
	if (node->left != NULL)
		node->left->parent = node;

	sub_sub_node->left = sub_node;
	sub_sub_node->right = node;

	if (sub_sub_node->balance == 0) {
		node->balance = 0;
		sub_node->balance = 0;
	} 
	else if (sub_sub_node->balance < 0) {
		node->balance = 1;
		sub_node->balance = 0;
	} 
	else {
		node->balance = 0;
		sub_node->balance = -1;
	}
	sub_sub_node->balance = 0;
}

static void perform_RL_rotation(AVLTree* tree, AVLTreeNode* node) {
	AVLTreeNode* parent_node = node->parent;
	AVLTreeNode* sub_node = node->right;
	AVLTreeNode* sub_sub_node = sub_node->left;

	if (parent_node == NULL)
		tree->root = sub_sub_node;
	else
		replace_parents_child(node, sub_sub_node);


	sub_sub_node->parent = parent_node;
	sub_node->parent = sub_sub_node;
	node->parent = sub_sub_node;

	sub_node->left = sub_sub_node->right;
	if (sub_node->left != NULL)
		sub_node->left->parent = sub_node;
	node->right = sub_sub_node->left;
	if (node->right != NULL)
		node->right->parent = node;

	sub_sub_node->right = sub_node;
	sub_sub_node->left = node;

	if (sub_sub_node->balance == 0) {
		node->balance = 0;
		sub_node->balance = 0;
	} 
	else if (sub_sub_node->balance > 0) {
		node->balance = -1;
		sub_node->balance = 0;
	} 
	else {
		node->balance = 0;
		sub_node->balance = 1;
	}
	sub_sub_node->balance = 0;
}

// assumes that the given node is unbalanced
// this only has implications within the data structure's code
static void balance_node(AVLTree* tree, AVLTreeNode* node) {
	if (node->balance < -1)
		if (node->left->balance <= 0)
			perform_right_rotation(tree, node);
		else
			perform_LR_rotation(tree, node);
	else
		if (node->right->balance >= 0)
			perform_left_rotation(tree, node);
		else
			perform_RL_rotation(tree, node);
}

static int node_is_balanced(AVLTreeNode* node) {
	return node->balance >= -1 && node->balance <= 1;
}

static void update_balance_on_add(AVLTree* tree, AVLTreeNode* node) {
	AVLTreeNode* parent_node = node->parent;

	while (parent_node != NULL) {
		parent_node->balance += NORMALIZE_COMPARE(tree->compare(node->data, parent_node->data));
		if (parent_node->balance == 0) 
			return;
		if (!node_is_balanced(parent_node)) {
			balance_node(tree, parent_node);
			return;
		}
		node = parent_node;
		parent_node = node->parent;
	}
}

static void update_balance_on_remove(AVLTree* tree, AVLTreeNode* node) {
	AVLTreeNode* parent_node = node->parent;

	while (parent_node != NULL) {
		parent_node->balance -= NORMALIZE_COMPARE(tree->compare(node->data, parent_node->data));
		if (!node_is_balanced(parent_node)) {
			balance_node(tree, parent_node);
			return;
		}
		if (parent_node->balance != 0) 
			return;
		node = parent_node;
		parent_node = node->parent;
	}
}

/* ---------------- Helper Functions ---------------- */

static AVLTreeNode* create_node(void* data) {
	AVLTreeNode* node = (AVLTreeNode*) Calloc(1, sizeof(AVLTreeNode));
	node->data = data;
	return node;
}

static AVLTreeNode* get_node(AVLTree* tree, void* data) {
	AVLTreeNode* curr_node = tree->root;
	int comparison;

	while (curr_node != NULL && (comparison = tree->compare(data, curr_node->data)) != 0)
		curr_node = comparison < 1 ? curr_node->left : curr_node->right;

	return curr_node;
}

static AVLTreeNode* get_parent_to_be_node(AVLTree* tree, void* data) {
	AVLTreeNode* curr_node = tree->root;
	while (1) {
		if (tree->compare(data, curr_node->data) < 1) {
			if (curr_node->left == NULL)
				return curr_node;
			curr_node = curr_node->left;
		}
		else {
			if (curr_node->right == NULL)
				return curr_node;
			curr_node = curr_node->right;
		}
	}
}

static AVLTreeNode* get_predecessor_node(AVLTreeNode* node) {
	node = node->left;
	while (node->right != NULL)
		node = node->right;
	return node;
}

static AVLTreeNode* get_successor_node(AVLTreeNode* node) {
	node = node->right;
	while (node->left != NULL)
		node = node->left;
	return node;
}

/* ---------------- Header Implementation ---------------- */

AVLTree* avlt_create(int(*compare)(void*, void*)) {
	AVLTree* tree = (AVLTree*) Calloc(1, sizeof(AVLTree));
	tree->compare = compare;
	return tree;
}

void avlt_add(AVLTree* tree, void* data) {
	AVLTreeNode* new_node = create_node(data);
	if (tree->size++ == 0) {
		tree->root = new_node;
		return;
	}

	AVLTreeNode* parent_node = get_parent_to_be_node(tree, data);
	if (tree->compare(data, parent_node->data) < 1)
		parent_node->left = new_node;
	else
		parent_node->right = new_node;
	new_node->parent = parent_node;

	update_balance_on_add(tree, new_node);
}

void* avlt_remove(AVLTree* tree, void* data) {
	AVLTreeNode* node = get_node(tree, data);
	
	if (node == NULL)
		return NULL;

	void* node_data = node->data;

	if (node->left != NULL && node->right != NULL) {
		AVLTreeNode* replacement_node = node->balance < 0 ? get_successor_node(node) : get_predecessor_node(node);
		update_balance_on_remove(tree, replacement_node);     // balance update must be done before the node's value replacement in this case
		node->data = replacement_node->data;
		node = replacement_node;
	}
	else
		update_balance_on_remove(tree, node);

	
	if (node->left != NULL) {
		node->left->parent = node->parent;
		replace_parents_child(node, node->left);
	}
	else if (node->right != NULL) {
		node->right->parent = node->parent;
		replace_parents_child(node, node->right);
	}
	else
		replace_parents_child(node, NULL);
	
	tree->size--;
	Free(node);
	return node_data;
}

void* avlt_get(AVLTree* tree, void* data) {
	return get_node(tree, data)->data;
}

int avlt_contains(AVLTree* tree, void* data) {
	return get_node(tree, data) == NULL ? 0 : 1;
}

void avlt_clear(AVLTree* tree) {
	if (tree->size == 0) return;

	AVLTreeNode* nodes[tree->size];
	nodes[0] = tree->root;
	int i = 1, j = 0;

	AVLTreeNode* curr_node;
	do {
		curr_node = nodes[j++];

		if (curr_node->left != NULL)
			nodes[i++] = curr_node->left;
		if (curr_node->right != NULL)
			nodes[i++] = curr_node->right;

		Free(curr_node);
	}  while (j < i);

	tree->size = 0;
}

void avlt_destroy(AVLTree* tree) {
	avlt_clear(tree);
	Free(tree);
}
