#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "../../../wrapper_functions.h"

static const Queue EMPTY_QUEUE;

/* ---------------- Helper Functions ---------------- */

static QNode* create_node(data_type data) {
    QNode* node = (QNode*) Calloc(1, sizeof(QNode));
    node->data = data;
    return node;
}

/* ---------------- Header Implementation ---------------- */

Queue* queue_create() {
    return (Queue*) Calloc(1, sizeof(Queue));
}

void queue_clear(Queue* queue) {
    QNode *prev_node, *curr_node = queue->head;

    int queue_size = queue->size;
    for(int i = 0; i < queue_size; i++) {
        prev_node = curr_node;
        curr_node = curr_node->next;
        Free(prev_node);
    }

    *queue = EMPTY_QUEUE;
}

void queue_add(Queue* queue, data_type data) {
    QNode* node = create_node(data);

    if(queue->size++ == 0)
        queue->head = node;
    else
        queue->tail->next = node;

    queue->tail = node;
}

// crashes the program when used on an empty queue
data_type queue_poll(Queue* queue) {
    QNode* head = queue->head;
    queue->head = head->next;
    data_type data = head->data;

    queue->size--;
    // it's fine to ignore tail because if the queue's last element was polled the Free wrapper function would set its pointer to NULL
    Free(head);
    return data;
}

// crashes the program when used on an empty queue
data_type queue_peek(Queue* queue) {
    return queue->head->data;
}
