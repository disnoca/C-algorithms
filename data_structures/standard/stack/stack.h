#ifndef STACK
#define STACK

#include "../linked_list/linked_list.h"

typedef LinkedList Stack;

/* ---------------- Functions ---------------- */

Stack* stack_create();

void stack_push(Stack* stack, data_type data);

data_type stack_pop(Stack* stack);

data_type stack_peek(Stack* stack);

void stack_destroy(Stack* stack);

#endif