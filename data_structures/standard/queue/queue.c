/**
 * Queue implementation.
 * 
 * @author Samuel Pires
*/

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "../../../wrapper_functions.h"

/* ---------------- Helper Functions ---------------- */

static QueueNode* create_queue_node(data_type data) {
	QueueNode* node = Malloc(sizeof(QueueNode));
	node->data = data;
    node->next = NULL;
	return node;
}


/* ---------------- Header Implementation ---------------- */

Queue* queue_create() {
    return (Queue*) Calloc(1, sizeof(Queue));
}

void queue_enqueue(Queue* queue, data_type data) {
    QueueNode* node = create_queue_node(data);

	if(queue->length == 0)
		queue->head = node;
	else
		queue->tail->next = node;

	queue->tail = node;
	queue->length++;
}

data_type queue_poll(Queue* queue) {
    QueueNode* node = queue->head;
	data_type data = node->data;

	queue->head = node->next;
	queue->length--;

	Free(node);
	return data;
}

void queue_destroy(Queue* queue) {
    QueueNode* curr_node = queue->head;
	QueueNode* next_node;

	while(curr_node != NULL) {
		next_node = curr_node->next;
		Free(curr_node);
		curr_node = next_node;
	}

	Free(queue);
}
