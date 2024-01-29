#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "queue.h"
#include "../../../wrapper_functions.h"

bool check_queue_integrity(Queue* queue) {
    if(queue->length == 0) 
		return true;

    QueueNode* curr_node = queue->head;
    for(int i = 0; i < queue->length - 1; i++) {    // check all next links
        if(curr_node == NULL) {
            printf("Assuming queue length (%d) is correct, next links incomplete.\n", queue->length);
            return false;
        }
        curr_node = curr_node->next;
    }

    if(curr_node != queue->tail) {
        printf("Assuming queue length (%d) is correct, last node does not correspond to tail.\n", queue->length);
        return false;
    }

    if(curr_node->next != NULL) {
        printf("Assuming queue length (%d) is correct, extra elements after tail.\n", queue->length);
        return false;
    } 

    return true;
}

void print_queue_contents(Queue* queue) {
    QueueNode* curr_node = queue->head;
    for(int i = 0; i < queue->length; i++) {
        printf("%d ", curr_node->data);
        curr_node = curr_node->next;
    }
    printf("\n");
}

Queue* create_queue_with_ascending_values(int length) {
    Queue* queue = queue_create();
    for(int i = 0; i < length; i++)
        queue_enqueue(queue, i);
    return queue;
}

int test_queue_enqueue() {
    Queue* queue = queue_create();

    queue_enqueue(queue, 10);

    int test_score = 0;
    test_score += queue->length == 1 ? 1 : 0;
    test_score += queue->head->data == 10 ? 10 : 0;
    test_score += queue->tail->data == 10 ? 100 : 0;

    queue_enqueue(queue, 11);
    queue_enqueue(queue, 12);

    test_score += queue->length == 3 ? 1000 : 0;
    test_score += queue->head->data == 10 ? 10000 : 0;
    test_score += queue->head->next->data == 11 ? 100000 : 0;
    test_score += queue->tail->data == 12 ? 2000000 : 0;

    check_queue_integrity(queue);
    queue_destroy(queue);
    return test_score;
}

int test_queue_dequeue() {
    Queue* queue = create_queue_with_ascending_values(10);

    int test_score = 10;
    for(int i = 0; i < 10; i++)
        test_score += queue_poll(queue) == i ? 1 : 0;
    test_score += queue->length == 0 ? 1 : 0;

    check_queue_integrity(queue);
    queue_destroy(queue);
    return test_score;
}

int main() {
    printf("queue_enqueue: %d\n", test_queue_enqueue());
    printf("queue_dequeue: %d\n", test_queue_dequeue());

    printf("\nMalloc calls: %d\nFree calls: %d\n", malloc_calls, free_calls);

    return 0;
}
