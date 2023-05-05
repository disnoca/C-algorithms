/**
 * AVL Tree implementation.
 * 
 * @author Samuel Pires
*/

#include <stdio.h>
#include <stdlib.h>
#include "avl_tree.h"
#include "../../../wrapper_functions.h"

/* ---------------- Compare Function ---------------- */

static data_type compare(data_type e1, data_type e2) {
    if(e1 > e2) return 1;
    if (e1 < e2) return -1;
    return 0;
}

/* ---------------- Tree Balance Functions ---------------- */

static void replace_parents_child(TreeNode* old_child, TreeNode* new_child) {
    TreeNode* parent = old_child->parent;
    if(parent->left == old_child)
        parent->left = new_child;
    else
        parent->right = new_child;
}

static void perform_left_rotation(AVLTree* tree, TreeNode* node) {
    TreeNode* parent_node = node->parent;
    TreeNode* sub_node = node->right;

    if(parent_node == NULL)
        tree->root = sub_node;
    else
        replace_parents_child(node, sub_node);

    sub_node->parent = parent_node;
    node->parent = sub_node;

    node->right = sub_node->left;
    if(node->right != NULL)
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

static void perform_right_rotation(AVLTree* tree, TreeNode* node) {
    TreeNode* parent_node = node->parent;
    TreeNode* sub_node = node->left;

    if(parent_node == NULL)
        tree->root = sub_node;
    else
        replace_parents_child(node, sub_node);

    sub_node->parent = parent_node;
    node->parent = sub_node;

    node->left = sub_node->right;
    if(node->left != NULL)
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

static void perform_LR_rotation(AVLTree* tree, TreeNode* node) {
    TreeNode* parent_node = node->parent;
    TreeNode* sub_node = node->left;
    TreeNode* sub_sub_node = sub_node->right;

    if(parent_node == NULL)
        tree->root = sub_sub_node;
    else
        replace_parents_child(node, sub_sub_node);

    sub_sub_node->parent = parent_node;
    sub_node->parent = sub_sub_node;
    node->parent = sub_sub_node;

    sub_node->right = sub_sub_node->left;
    if(sub_node->right != NULL)
        sub_node->right->parent = sub_node;
    node->left = sub_sub_node->right;
    if(node->left != NULL)
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

static void perform_RL_rotation(AVLTree* tree, TreeNode* node) {
    TreeNode* parent_node = node->parent;
    TreeNode* sub_node = node->right;
    TreeNode* sub_sub_node = sub_node->left;

    if(parent_node == NULL)
        tree->root = sub_sub_node;
    else
        replace_parents_child(node, sub_sub_node);


    sub_sub_node->parent = parent_node;
    sub_node->parent = sub_sub_node;
    node->parent = sub_sub_node;

    sub_node->left = sub_sub_node->right;
    if(sub_node->left != NULL)
        sub_node->left->parent = sub_node;
    node->right = sub_sub_node->left;
    if(node->right != NULL)
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
static void balance_node(AVLTree* tree, TreeNode* node) {
    if(node->balance < -1)
        if(node->left->balance <= 0)
            perform_right_rotation(tree, node);
        else
            perform_LR_rotation(tree, node);
    else
        if(node->right->balance >= 0)
            perform_left_rotation(tree, node);
        else
            perform_RL_rotation(tree, node);
}

static int node_is_balanced(TreeNode* node) {
    return node->balance >= -1 && node->balance <= 1;
}

static void update_balance_on_add(AVLTree* tree, TreeNode* node) {
    TreeNode* parent_node = node->parent;

    while(parent_node != NULL) {
        parent_node->balance += compare(node->data, parent_node->data);     // this only works if compare returns -1 or 1
        if(parent_node->balance == 0) 
            return;
        if(!node_is_balanced(parent_node)) {
            balance_node(tree, parent_node);
            return;
        }
        node = parent_node;
        parent_node = node->parent;
    }
}

static void update_balance_on_remove(AVLTree* tree, TreeNode* node) {
    TreeNode* parent_node = node->parent;

    while(parent_node != NULL) {
        parent_node->balance -= compare(node->data, parent_node->data);     // this only works if compare returns -1 or 1
        if(!node_is_balanced(parent_node)) {
            balance_node(tree, parent_node);
            return;
        }
        if(parent_node->balance != 0) 
            return;
        node = parent_node;
        parent_node = node->parent;
    }
}

/* ---------------- Helper Functions ---------------- */

static TreeNode* create_node(data_type data) {
    TreeNode* node = (TreeNode*) Calloc(1, sizeof(TreeNode));
    node->data = data;
    return node;
}

static TreeNode* get_node(AVLTree* tree, data_type data) {
    TreeNode* curr_node = tree->root;
    int comparison;

    while(curr_node != NULL && (comparison = compare(data, curr_node->data)) != 0) {
        if(comparison < 1)
            curr_node = curr_node->left;
        else
            curr_node = curr_node->right;
    }

    return curr_node;
}

static TreeNode* get_parent_to_be_node(AVLTree* tree, data_type data) {
    TreeNode* curr_node = tree->root;
    while(1) {
        if(compare(data, curr_node->data) < 1) {
            if(curr_node->left == NULL)
                return curr_node;
            curr_node = curr_node->left;
        }
        else {
            if(curr_node->right == NULL)
                return curr_node;
            curr_node = curr_node->right;
        }
    }
}

static TreeNode* get_predecessor_node(TreeNode* node) {
    node = node->left;
    while(node->right != NULL)
        node = node->right;
    return node;
}

static TreeNode* get_successor_node(TreeNode* node) {
    node = node->right;
    while(node->left != NULL)
        node = node->left;
    return node;
}

/* ---------------- Header Implementation ---------------- */

AVLTree* avlt_create() {
    return (AVLTree*) Calloc(1, sizeof(AVLTree));
}

void avlt_add(AVLTree* tree, data_type data) {
    TreeNode* new_node = create_node(data);
    if(tree->size++ == 0) {
        tree->root = new_node;
        return;
    }

    TreeNode* parent_node = get_parent_to_be_node(tree, data);
    if(compare(data, parent_node->data) < 1)
        parent_node->left = new_node;
    else
        parent_node->right = new_node;
    new_node->parent = parent_node;

    update_balance_on_add(tree, new_node);
}

data_type avlt_remove(AVLTree* tree, data_type data) {
    TreeNode* node = get_node(tree, data);
    data_type node_data = node->data;

    if(node->left != NULL && node->right != NULL) {
        TreeNode* replacement_node = node->balance < 0 ? get_successor_node(node) : get_predecessor_node(node);
        update_balance_on_remove(tree, replacement_node);     // balance update must be done before the node's value replacement in this case
        node->data = replacement_node->data;
        node = replacement_node;
    }
    else
        update_balance_on_remove(tree, node);

    
    if(node->left != NULL) {
        node->left->parent = node->parent;
        replace_parents_child(node, node->left);
    }
    else if(node->right != NULL) {
        node->right->parent = node->parent;
        replace_parents_child(node, node->right);
    }
    else
        replace_parents_child(node, NULL);
    
    tree->size--;
    Free(node);
    return node_data;
}

data_type avlt_get(AVLTree* tree, data_type data) {
    return get_node(tree, data)->data;
}

int avlt_contains(AVLTree* tree, data_type data) {
    return get_node(tree, data) == NULL ? 0 : 1;
}

void avlt_clear(AVLTree* tree) {
    if(tree->size == 0) return;

    TreeNode* nodes[tree->size];
    nodes[0] = tree->root;
    int i = 1, j = 0;

    TreeNode* curr_node;
    do {
        curr_node = nodes[j++];

        if(curr_node->left != NULL)
            nodes[i++] = curr_node->left;
        if(curr_node->right != NULL)
            nodes[i++] = curr_node->right;

        Free(curr_node);
    }  while(j < i);

    tree->size = 0;
}

void avlt_destroy(AVLTree* tree) {
    avlt_clear(tree);
    Free(tree);
}