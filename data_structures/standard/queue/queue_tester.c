#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "../../../wrapper_functions.h"

bool check_queue_integrity(Queue* queue) {
    if(queue->size == 0) 
		return true;

    LLNode* curr_node = queue->head;
    for(int i = 0; i < queue->size; i++) {    // check all next links
        if(curr_node == NULL) {
            printf("Assuming queue size (%d) is correct, next links incomplete.\n", queue->size);
            return false;
        }
        curr_node = curr_node->next;
    }

    if(curr_node != NULL) {
        printf("Assuming queue size (%d) is correct, extra elements after tail.\n", queue->size);
        return false;
    } 
    
    curr_node = queue->tail;
    for(int i = queue->size-1; i >= 0; i--) { // check all prev links
        if(curr_node == NULL) {
            printf("Assuming queue size (%d) is correct, prev links incomplete.\n", queue->size);
            return false;
        }
        curr_node = curr_node->prev;
    }

    if(curr_node != NULL) {
        printf("Assuming queue size (%d) is correct, extra elements before head.\n", queue->size);
        return false;
    }

    return true;
}

void print_queue_contents(Queue* queue) {
    LLNode* curr_node = queue->head;
    for(int i = 0; i < queue->size; i++) {
        printf("%d ", curr_node->data);
        curr_node = curr_node->next;
    }
    printf("\n");
}

Queue* create_queue_with_ascending_values(int size) {
    Queue* queue = queue_create();
    for(int i = 0; i < size; i++)
        queue_add(queue, i);
    return queue;
}

int test_queue_add() {
    Queue* queue = queue_create();

    queue_add(queue, 10);

    int test_score = 0;
    test_score += queue->size == 1 ? 1 : 0;
    test_score += queue->head->data == 10 ? 10 : 0;
    test_score += queue->tail->data == 10 ? 100 : 0;

    queue_add(queue, 11);
    queue_add(queue, 12);

    test_score += queue->size == 3 ? 1000 : 0;
    test_score += queue->head->data == 10 ? 10000 : 0;
    test_score += queue->head->next->data == 11 ? 100000 : 0;
    test_score += queue->tail->data == 12 ? 2000000 : 0;

    check_queue_integrity(queue);
    queue_destroy(queue);
    return test_score;
}

int test_queue_poll() {
    Queue* queue = create_queue_with_ascending_values(10);

    int test_score = 10;
    for(int i = 0; i < 10; i++)
        test_score += queue_poll(queue) == i ? 1 : 0;
    test_score += queue->size == 0 ? 1 : 0;

    check_queue_integrity(queue);
    queue_destroy(queue);
    return test_score;
}

int test_queue_peek() {
    Queue* queue = create_queue_with_ascending_values(10);

    int test_score = -8;
    for(int i = 0; i < 10; i++) {
        test_score += queue_peek(queue) == i ? 1 : 0;
        queue_poll(queue);
    }

    check_queue_integrity(queue);
    queue_destroy(queue);
    return test_score;
}

int main() {
    printf("queue_add: %d\n", test_queue_add());
    printf("queue_poll: %d\n", test_queue_poll());
    printf("queue_peek: %d\n", test_queue_peek());

    printf("\nMalloc calls: %d\nFree calls: %d\n", malloc_calls, free_calls);

    return 0;
}