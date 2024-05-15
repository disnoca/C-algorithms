#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "stack.h"
#include "../../wrapper_functions.h"


int values[1024];
int curr_val;

#define REF_OF(x)   ({ values[curr_val] = (x); values + curr_val++; })
#define DEREF(p)    (*((int*)(p)))


bool check_stack_integrity(Stack* stack) {
    if (stack->size == 0) 
		return true;
    
    StackNode* curr_node = stack->tail;
    for(size_t i = 0; i < stack->size; i++) { // check all prev links
        if (curr_node == NULL) {
            printf("Assuming stack size (%ld) is correct, prev links incomplete.\n", stack->size);
            return false;
        }
        curr_node = curr_node->prev;
    }

    if (curr_node != NULL) {
        printf("Assuming stack size (%ld) is correct, extra elements before head.\n", stack->size);
        return false;
    }

    return true;
}

__attribute__((unused)) static void print_stack_contents(Stack* stack) {
    StackNode* curr_node = stack->tail;
    for(size_t i = 0; i < stack->size; i++) {
        printf("%d ", DEREF(curr_node->data));
        curr_node = curr_node->prev;
    }
    printf("\n");
}

static Stack* create_stack_with_ascending_values(int size) {
    Stack* stack = stack_create();
    for(int i = 0; i < size; i++)
        stack_push(stack, REF_OF(i));
    return stack;
}

static void test_stack_push() {
    Stack* stack = stack_create();

    stack_push(stack, REF_OF(10));

    assert(stack->size == 1);
    assert(DEREF(stack->tail->data) == 10);

    stack_push(stack, REF_OF(11));
    stack_push(stack, REF_OF(12));

    assert(DEREF(stack->tail->data) == 12);
    assert(DEREF(stack->tail->prev->data) == 11);
    assert(stack->size == 3);

    assert(check_stack_integrity(stack));
    stack_destroy(stack);
}

static void test_stack_pop() {
    Stack* stack = create_stack_with_ascending_values(10);

    for(int i = 0; i < 10; i++)
        assert(DEREF(stack_pop(stack)) == 9 - i);
    assert(stack->size == 0);

    assert(check_stack_integrity(stack));
    stack_destroy(stack);
}

int main() {
    test_stack_push();
    test_stack_pop();

    assert(malloc_calls == free_calls);

    printf("All tests passed.\n");
    return 0;
}
