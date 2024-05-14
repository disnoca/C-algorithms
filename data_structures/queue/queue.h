#pragma once

typedef int data_type;

typedef struct QueueNode QueueNode;

typedef struct {
	QueueNode *head, *tail;
	int length;
} Queue;

struct QueueNode {
	data_type data;
	QueueNode* next;
};

/* ---------------- Functions ---------------- */

Queue* queue_create();

void queue_enqueue(Queue* queue, data_type data);

data_type queue_dequeue(Queue* queue);

data_type queue_peek(Queue* queue);

void queue_destroy(Queue* queue);
