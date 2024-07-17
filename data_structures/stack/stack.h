#pragma once

#include <stdbool.h>


typedef struct StackNode StackNode;

typedef struct {
	StackNode *tail;
	size_t size;
} Stack;

struct StackNode {
	void* data;
	StackNode* prev;
};


/* ---------------- Functions ---------------- */

void stack_init(Stack* stack);
void stack_free(Stack* stack, bool free_data);
Stack* stack_create(void);
void stack_destroy(Stack* stack, bool free_data);

void stack_push(Stack* stack, void* data);
void* stack_pop(Stack* stack);
void* stack_peek(Stack* stack);
void stack_clear(Stack* stack, bool free_data);

