#pragma once

#include <stdbool.h>


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

void queue_init(Queue* queue);
void queue_free(Queue* queue, bool free_data);
Queue* queue_create(void);
void queue_destroy(Queue* queue, bool free_data);

void queue_enqueue(Queue* queue, void* data);
void* queue_dequeue(Queue* queue);
void* queue_peek(Queue* queue);
void queue_clear(Queue* queue, bool free_data);
