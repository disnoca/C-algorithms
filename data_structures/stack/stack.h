#pragma once


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

Stack* stack_create();
void stack_destroy(Stack* stack);

void stack_push(Stack* stack, void* data);
void* stack_pop(Stack* stack);
void* stack_peek(Stack* stack);

