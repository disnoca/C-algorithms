#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "..\..\wrapper_functions.h"

static const Stack EMPTY_STACK;

/* ---------------- Helper Functions ---------------- */

static SNode* create_node(data_type data) {
    SNode* node = (SNode*) Calloc(1, sizeof(SNode));
    node->data = data;
    return node;
}

/* ---------------- Header Implementation ---------------- */

Stack* stack_create() {
    return (Stack*) Calloc(1, sizeof(Stack));
}

void stack_clear(Stack* stack) {
    SNode *prev_node, *curr_node = stack->tail;

    int stack_size = stack->size;
    for(int i = 0; i < stack_size; i++) {
        prev_node = curr_node;
        curr_node = curr_node->prev;
        Free(prev_node);
    }

    *stack = EMPTY_STACK;
}

void stack_push(Stack* stack, data_type data) {
    SNode* node = create_node(data);

    node->prev = stack->tail;
    stack->tail = node;
    stack->size++;
}

// crashes the program when used on an empty stack
data_type stack_pop(Stack* stack) {
    SNode* tail = stack->tail;
    stack->tail = tail->prev;
    data_type data = tail->data;

    stack->size--;
    Free(tail);
    return data;
}

// crashes the program when used on an empty stack
data_type stack_peek(Stack* stack) {
    return stack->tail->data;
}
