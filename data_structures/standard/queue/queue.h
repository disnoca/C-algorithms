#ifndef QUEUE
#define QUEUE

#include "../linked_list/linked_list.h"

typedef LinkedList Queue;

/* ---------------- Functions ---------------- */

Queue* queue_create();

void queue_add(Queue* queue, data_type data);

data_type queue_poll(Queue* queue);

data_type queue_peek(Queue* queue);

void queue_destroy(Queue* queue);

#endif