#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "../../../wrapper_functions.h"

int check_queue_integrity(Queue* queue) {
    if(queue->size == 0) return 1;

    int queue_size = queue->size;
    QNode* node = queue->head;
    for(int i = 0; i < queue_size; i++) {    // check all links
        if(node == NULL) {
            printf("assuming queue size (%d) is correct, next links incomplete\n", queue_size);
            return 0;
        }
        node = node->next;
    }

    if(node != NULL) {
        printf("assuming queue size (%d) is correct, extra elements after tail\n", queue_size);
        return 0;
    } 

    return 1;
}

void print_queue_contents(Queue* queue) {
    int queue_size = queue->size;
    QNode* node = queue->head;
    for(int i = 0; i < queue_size; i++) {
        printf("%d ", node->data);
        node = node->next;
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
    queue_clear(queue);
    Free(queue);
    return test_score;
}

int test_queue_poll() {
    Queue* queue = create_queue_with_ascending_values(10);

    int test_score = 10;
    for(int i = 0; i < 10; i++)
        test_score += queue_poll(queue) == i ? 1 : 0;
    test_score += queue->size == 0 ? 1 : 0;

    check_queue_integrity(queue);
    queue_clear(queue);
    Free(queue);
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
    queue_clear(queue);
    Free(queue);
    return test_score;
}

int main() {
    printf("queue_add: %d\n", test_queue_add());
    printf("queue_poll: %d\n", test_queue_poll());
    printf("queue_peek: %d\n", test_queue_peek());

    printf("\nMalloc calls: %d\nFree calls: %d\n", malloc_calls, free_calls);

    return 0;
}