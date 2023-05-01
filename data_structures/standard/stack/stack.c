#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "../../../wrapper_functions.h"

Stack* stack_create() {
    return ll_create();
}

void stack_push(Stack* stack, data_type data) {
    ll_add(stack, data);
}

data_type stack_pop(Stack* stack) {
    return ll_remove_last(stack);
}

data_type stack_peek(Stack* stack) {
    return ll_get(stack, stack->size-1);
}

void stack_destroy(Stack* stack) {
    ll_destroy(stack);
}