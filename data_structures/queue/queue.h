#pragma once


typedef struct QueueNode QueueNode;

typedef struct {
	QueueNode *head, *tail;
	size_t length;
} Queue;

struct QueueNode {
	void* data;
	QueueNode* next;
};

/* ---------------- Functions ---------------- */

Queue* queue_create();

void queue_enqueue(Queue* queue, void* data);

void* queue_dequeue(Queue* queue);

void* queue_peek(Queue* queue);

void queue_destroy(Queue* queue);
