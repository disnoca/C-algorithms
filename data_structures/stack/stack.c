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

static StackNode* create_stack_node(void* data) {
	StackNode* node = Malloc(sizeof(StackNode));
	node->data = data;
    node->prev = NULL;
	return node;
}


/* ---------------- Header Implementation ---------------- */

void stack_init(Stack* stack)
{
    stack->tail = NULL;
    stack->size = 0;
}

Stack* stack_create()
{
    Stack* stack = Malloc(sizeof(Stack));
    stack_init(stack);
    return stack;
}

void stack_push(Stack* stack, void* data)
{
    StackNode* node = create_stack_node(data);
    node->prev = stack->tail;
    stack->tail = node;
    stack->size++;
}

void* stack_pop(Stack* stack)
{
    StackNode* node = stack->tail;
    void* data = node->data;

    stack->tail = node->prev;
    stack->size--;

    Free(node);
    return data;
}

void* stack_peek(Stack* stack)
{
    return stack->tail->data;
}

void stack_clear(Stack* stack, bool free_data)
{
    StackNode* curr_node = stack->tail;
    StackNode* next_node = NULL;

    while (curr_node != NULL) {
        next_node = curr_node->prev;
        if (free_data) Free(curr_node->data);
        Free(curr_node);
        curr_node = next_node;
    }

    stack->tail = NULL;
    stack->size = 0;
}

void stack_free(Stack* stack, bool free_data)
{
    stack_clear(stack, free_data);
}

void stack_destroy(Stack* stack, bool free_data)
{
    stack_free(stack, free_data);
    Free(stack);
}
