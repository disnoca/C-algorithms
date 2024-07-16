/**
 * Queue implementation.
 * 
 * @author Samuel Pires
*/

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "../../wrapper_functions.h"

/* ---------------- Helper Functions ---------------- */

static QueueNode* create_queue_node(void* data) {
	QueueNode* node = Malloc(sizeof(QueueNode));
	node->data = data;
    node->next = NULL;
	return node;
}


/* ---------------- Header Implementation ---------------- */

void queue_init(Queue* queue)
{
	queue->head = NULL;
	queue->tail = NULL;
	queue->length = 0;
}

Queue* queue_create(void)
{
	Queue* queue = Malloc(sizeof(Queue));
	queue_init(queue);
	return queue;
}

void queue_enqueue(Queue* queue, void* data)
{
    QueueNode* node = create_queue_node(data);

	if (queue->length == 0)
		queue->head = node;
	else
		queue->tail->next = node;

	queue->tail = node;
	queue->length++;
}

void* queue_dequeue(Queue* queue)
{
    QueueNode* node = queue->head;
	void* data = node->data;

	queue->head = node->next;
	queue->length--;

	Free(node);
	return data;
}

void queue_clear(Queue* queue)
{
	QueueNode* curr_node = queue->head;
	QueueNode* next_node;

	while (curr_node != NULL) {
		next_node = curr_node->next;
		Free(curr_node);
		curr_node = next_node;
	}

	queue->head = NULL;
	queue->tail = NULL;
	queue->length = 0;
}

void queue_free(Queue* queue)
{
	queue_clear(queue);
}

void queue_destroy(Queue* queue)
{
	queue_free(queue);
	Free(queue);
}
