/**
 * Stack implementation.
 * 
 * @author Samuel Pires
*/

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "../../wrapper_functions.h"

/* ---------------- Helper Functions ---------------- */

static StackNode* create_stack_node(data_type data) {
	StackNode* node = Malloc(sizeof(StackNode));
	node->data = data;
    node->prev = NULL;
	return node;
}


/* ---------------- Header Implementation ---------------- */

Stack* stack_create() {
    return (Stack*) Calloc(1, sizeof(Stack));
}

void stack_push(Stack* stack, data_type data) {
    StackNode* node = create_stack_node(data);
    node->prev = stack->tail;
    stack->tail = node;
    stack->size++;
}

data_type stack_pop(Stack* stack) {
    StackNode* node = stack->tail;
    data_type data = node->data;

    stack->tail = node->prev;
    stack->size--;

    Free(node);
    return data;
}

data_type stack_peek(Stack* stack)
{
    return stack->tail->data;
}

void stack_destroy(Stack* stack) {
    StackNode* curr_node = stack->tail;
    StackNode* next_node = NULL;

    while (curr_node != NULL) {
        next_node = curr_node->prev;
        Free(curr_node);
        curr_node = next_node;
    }

    Free(stack);
}
