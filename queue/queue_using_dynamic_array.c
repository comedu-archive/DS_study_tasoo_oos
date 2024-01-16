#ifndef tasoo_queue
#define tasoo_queue

#include <stdlib.h>
#include <string.h>
#include "queue.h"

typedef struct QUEUE_ {
    void *mem;
    size_t n;
    size_t front;
    size_t back;
    size_t elem_size;
} queue;

queue *new_queue(const size_t elem_size) {
    queue *q = malloc(sizeof(queue));
    if (!q) {
        perror("Not enough memory");
        exit(EXIT_FAILURE);
    }
    q->n = 8;
    q->elem_size = elem_size;
    q->mem = malloc(q->n * elem_size);
    if (!q->mem) {
        perror("Not enough memory");
        exit(EXIT_FAILURE);
    }
    q->front = 0;
    q->back = 0;
    return q;
}

inline size_t queue_count(const queue *q) {
    return (q->n + q->back - q->front) % q->n;
}

inline bool is_queue_empty(const queue *q) {
    return !q || q->front == q->back;
}

// Function for resizing the queue memory
static void resize_queue(queue *q, const bool increase) {
    const size_t new_size = increase ? q->n * 2 : q->n / 2;
    void *new_mem = malloc(new_size * q->elem_size);
    if (!new_mem) {
        perror("Not enough memory");
        exit(EXIT_FAILURE);
    }

    const size_t count = queue_count(q);

    if(q->front < q->back) {
        memcpy(new_mem, q->mem + q->front * q->elem_size, count * q->elem_size);
    } else {
        size_t size_1 = q->n - q->front;
        size_t size_2 = count - size_1;
        memcpy(new_mem, q->mem + q->front * q->elem_size, size_1 * q->elem_size);
        memcpy(new_mem + size_1 * q->elem_size, q->mem, size_2 * q->elem_size);
    }

    q->front = 0;
    q->back = count;
    q->n = new_size;
    free(q->mem);
    q->mem = new_mem;
}

bool enqueue(queue *q, const void *x) {
    if(!q || !q->mem) return false;

    if ((q->back + 1) % q->n == q->front)
        resize_queue(q, true);

    memcpy(q->mem + q->back * q->elem_size, x, q->elem_size);
    q->back = (q->back + 1) % q->n;
    return true;
}

bool dequeue(queue *q) {
    if (is_queue_empty(q)) return false;

    if (queue_count(q) == q->n / 4 && q->n > 8)
        resize_queue(q, false);

    q->front = (q->front + 1) % q->n;
    return true;
}

bool queue_front(const queue *q, void *out) {
    if (is_queue_empty(q)) return false;

    memcpy(out, q->mem + q->front * q->elem_size, q->elem_size);
    return true;
}

bool queue_back(const queue *q, void *out) {
    if (is_queue_empty(q)) return false;

    memcpy(out, q->mem + ((q->back + q->n - 1) % q->n) * q->elem_size, q->elem_size);
    return true;
}

void destroy_queue(queue** q) {
    free((*q)->mem);
    free(*q);
    *q = NULL;
}

#endif