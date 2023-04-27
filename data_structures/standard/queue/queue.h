#ifndef QUEUE
#define QUEUE

typedef int data_type;

/* ---------------- Structs ---------------- */

typedef struct QNode QNode;

struct QNode {
    QNode* next;
    data_type data;
};

typedef struct {
    QNode *head, *tail;
    int size;
} Queue;

/* ---------------- Functions ---------------- */

Queue* queue_create();

void queue_clear();

void queue_add(Queue* queue, data_type data);

data_type queue_poll(Queue* queue);

data_type queue_peek(Queue* queue);

#endif