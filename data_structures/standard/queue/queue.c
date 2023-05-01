#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "../../../wrapper_functions.h"

Queue* queue_create() {
    return ll_create();
}

void queue_add(Queue* queue, data_type data) {
    ll_add(queue, data);
}

data_type queue_poll(Queue* queue) {
    return ll_remove_first(queue);
}

data_type queue_peek(Queue* queue) {
    return ll_get(queue, 0);
}

void queue_destroy(Queue* queue) {
    ll_destroy(queue);
}
