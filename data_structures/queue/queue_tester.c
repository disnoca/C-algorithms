#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "queue.h"
#include "../../wrapper_functions.h"


int values[1024];
int curr_val;

#define REF_OF(x)   ({ values[curr_val] = (x); values + curr_val++; })
#define DEREF(p)    (*((int*)(p)))


static bool check_queue_integrity(Queue* queue) {
	if (queue->length == 0) 
		return true;

	QueueNode* curr_node = queue->head;
	for(size_t i = 0; i < queue->length - 1; i++) {    // check all next links
		if (curr_node == NULL) {
			printf("Assuming queue length (%ld) is correct, next links incomplete.\n", queue->length);
			return false;
		}
		curr_node = curr_node->next;
	}

	if (curr_node != queue->tail) {
		printf("Assuming queue length (%ld) is correct, last node does not correspond to tail.\n", queue->length);
		return false;
	}

	if (curr_node->next != NULL) {
		printf("Assuming queue length (%ld) is correct, extra elements after tail.\n", queue->length);
		return false;
	} 

	return true;
}

__attribute__((unused)) void print_queue_contents(Queue* queue) {
	QueueNode* curr_node = queue->head;
	for(size_t i = 0; i < queue->length; i++) {
		printf("%d ", DEREF(curr_node->data));
		curr_node = curr_node->next;
	}
	printf("\n");
}

static Queue* create_queue_with_ascending_values(int length) {
	Queue* queue = queue_create();
	for(int i = 0; i < length; i++)
		queue_enqueue(queue, REF_OF(i));
	return queue;
}

static void test_queue_enqueue() {
	Queue* queue = queue_create();

	queue_enqueue(queue, REF_OF(10));

	assert(queue->length == 1);
	assert(DEREF(queue->head->data) == 10);
	assert(DEREF(queue->tail->data) == 10);

	queue_enqueue(queue, REF_OF(11));
	queue_enqueue(queue, REF_OF(12));

	assert(queue->length == 3);
	assert(DEREF(queue->head->data) == 10);
	assert(DEREF(queue->head->next->data) == 11);
	assert(DEREF(queue->tail->data) == 12);

	assert(check_queue_integrity(queue));
	queue_destroy(queue);
}

static void test_queue_dequeue() {
	Queue* queue = create_queue_with_ascending_values(10);

	for(int i = 0; i < 10; i++)
		assert(DEREF(queue_dequeue(queue)) == i);
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
