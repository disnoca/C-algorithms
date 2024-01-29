#ifndef STACK
#define STACK

typedef int data_type;

typedef struct StackNode StackNode;

typedef struct {
	StackNode *tail;
	int size;
} Stack;

struct StackNode {
	data_type data;
	StackNode* prev;
};

/* ---------------- Functions ---------------- */

Stack* stack_create();

void stack_push(Stack* stack, data_type data);

data_type stack_pop(Stack* stack);

void stack_destroy(Stack* stack);

#endif
