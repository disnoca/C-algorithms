#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "../../../wrapper_functions.h"

bool check_stack_integrity(Stack* stack) {
    if(stack->size == 0) 
		return true;

    LLNode* curr_node = stack->head;
    for(int i = 0; i < stack->size; i++) {    // check all next links
        if(curr_node == NULL) {
            printf("Assuming stack size (%d) is correct, next links incomplete.\n", stack->size);
            return false;
        }
        curr_node = curr_node->next;
    }

    if(curr_node != NULL) {
        printf("Assuming stack size (%d) is correct, extra elements after tail.\n", stack->size);
        return false;
    } 
    
    curr_node = stack->tail;
    for(int i = stack->size-1; i >= 0; i--) { // check all prev links
        if(curr_node == NULL) {
            printf("Assuming stack size (%d) is correct, prev links incomplete.\n", stack->size);
            return false;
        }
        curr_node = curr_node->prev;
    }

    if(curr_node != NULL) {
        printf("Assuming stack size (%d) is correct, extra elements before head.\n", stack->size);
        return false;
    }

    return true;
}

void print_stack_contents(Stack* stack) {
    LLNode* curr_node = stack->head;
    for(int i = 0; i < stack->size; i++) {
        printf("%d ", curr_node->data);
        curr_node = curr_node->next;
    }
    printf("\n");
}

Stack* create_stack_with_ascending_values(int size) {
    Stack* stack = stack_create();
    for(int i = 0; i < size; i++)
        stack_push(stack, i);
    return stack;
}

int test_stack_push() {
    Stack* stack = stack_create();

    stack_push(stack, 10);

    int test_score = 0;
    test_score += stack->size == 1 ? 1 : 0;
    test_score += stack->tail->data == 10 ? 10 : 0;

    stack_push(stack, 11);
    stack_push(stack, 12);

    test_score += stack->tail->data == 12 ? 100 : 0;
    test_score += stack->tail->prev->data == 11 ? 1000 : 0;
    test_score += stack->size == 3 ? 20000 : 0;

    check_stack_integrity(stack);
    stack_destroy(stack);
    return test_score;
}

int test_stack_pop() {
    Stack* stack = create_stack_with_ascending_values(10);

    int test_score = 10;
    for(int i = 0; i < 10; i++)
        test_score += stack_pop(stack) == 9-i ? 1 : 0;
    test_score += stack->size == 0 ? 1 : 0;

    check_stack_integrity(stack);
    stack_destroy(stack);
    return test_score;
}

int test_stack_peek() {
    Stack* stack = create_stack_with_ascending_values(10);

    int test_score = 11;
    for(int i = 0; i < 10; i++) {
        test_score += stack_peek(stack) == 9-i ? 1 : 0;
        stack_pop(stack);
    }

    check_stack_integrity(stack);
    stack_destroy(stack);
    return test_score;
}

int main() {
    printf("stack_push: %d\n", test_stack_push());
    printf("stack_pop: %d\n", test_stack_pop());
    printf("stack_peek: %d\n", test_stack_peek());

    printf("\nMalloc calls: %d\nFree calls: %d\n", malloc_calls, free_calls);

    return 0;
}