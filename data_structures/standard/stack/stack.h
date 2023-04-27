#ifndef STACK
#define STACK

typedef int data_type;

/* ---------------- Structs ---------------- */

typedef struct SNode SNode;

struct SNode {
    SNode* prev;
    data_type data;
};

typedef struct {
    SNode* tail;
    int size;
} Stack;

/* ---------------- Functions ---------------- */

Stack* stack_create();

void stack_clear();

void stack_push(Stack* stack, data_type data);

data_type stack_pop(Stack* stack);

data_type stack_peek(Stack* stack);

#endif