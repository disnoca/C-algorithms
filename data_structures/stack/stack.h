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

void stack_init(Stack* stack);
void stack_free(Stack* stack);
Stack* stack_create(void);
void stack_destroy(Stack* stack);

void stack_push(Stack* stack, void* data);
void* stack_pop(Stack* stack);
void* stack_peek(Stack* stack);
void stack_clear(Stack* stack);

