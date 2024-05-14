#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "queue.h"
#include "../../wrapper_functions.h"

static bool check_queue_integrity(Queue* queue) {
	if (queue->length == 0) 
		return true;

	QueueNode* curr_node = queue->head;
	for(int i = 0; i < queue->length - 1; i++) {    // check all next links
		if (curr_node == NULL) {
			printf("Assuming queue length (%d) is correct, next links incomplete.\n", queue->length);
			return false;
		}
		curr_node = curr_node->next;
	}

	if (curr_node != queue->tail) {
		printf("Assuming queue length (%d) is correct, last node does not correspond to tail.\n", queue->length);
		return false;
	}

	if (curr_node->next != NULL) {
		printf("Assuming queue length (%d) is correct, extra elements after tail.\n", queue->length);
		return false;
	} 

	return true;
}

__attribute__((unused)) void print_queue_contents(Queue* queue) {
	QueueNode* curr_node = queue->head;
	for(int i = 0; i < queue->length; i++) {
		printf("%d ", curr_node->data);
		curr_node = curr_node->next;
	}
	printf("\n");
}

static Queue* create_queue_with_ascending_values(int length) {
	Queue* queue = queue_create();
	for(int i = 0; i < length; i++)
		queue_enqueue(queue, i);
	return queue;
}

static void test_queue_enqueue() {
	Queue* queue = queue_create();

	queue_enqueue(queue, 10);

	assert(queue->length == 1);
	assert(queue->head->data == 10);
	assert(queue->tail->data == 10);

	queue_enqueue(queue, 11);
	queue_enqueue(queue, 12);

	assert(queue->length == 3);
	assert(queue->head->data == 10);
	assert(queue->head->next->data == 11);
	assert(queue->tail->data == 12);

	assert(check_queue_integrity(queue));
	queue_destroy(queue);
}

static void test_queue_dequeue() {
	Queue* queue = create_queue_with_ascending_values(10);

	for(int i = 0; i < 10; i++)
		assert(queue_dequeue(queue) == i);
	assert(queue->length == 0);

	assert(check_queue_integrity(queue));
	queue_destroy(queue);
}

int main() {
	test_queue_enqueue();
	test_queue_dequeue();

	assert(malloc_calls == free_calls);

	printf("All tests passed.\n");
	return 0;
}
