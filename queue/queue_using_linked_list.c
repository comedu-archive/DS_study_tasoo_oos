#include <stdlib.h>
#include <string.h>

#include "queue.h"


typedef struct Node_ {
    void* data;
    struct Node_* next;
}node;

typedef struct QUEUE_ {
    size_t count;
    size_t elem_size;
    node* front;
    node* back;
}queue;

// Function prototypes for the queue operations
queue *new_queue(const size_t elem_size) {
    queue* q = malloc(sizeof(queue));
    if(!q) {
        perror("Not enough memory");
        exit(EXIT_FAILURE);
    }
    q->front = NULL;
    q->back = NULL;
    q->count = 0;
    q->elem_size = elem_size;
    return q;
}

static node* new_node(const void* data, const size_t elem_size) {
    node* n = malloc(sizeof(node));
    if(!n) {
        perror("Not enough memory");
        exit(EXIT_FAILURE);
    }

    n->data = malloc(elem_size);
    if(!n->data) {
        perror("Not enough memory");
        exit(EXIT_FAILURE);
    }

    memcpy(n->data, data, elem_size);
    n->next = NULL;

    return n;
}

static void destroy_node(node* del) {
    free(del->data);
    free(del);
}

size_t queue_count(const queue* q) {
    return q->count;
}

bool is_queue_empty(const queue* q) {
    return !q || !q->front;
}

bool enqueue(queue* q, const void* data) {
    if(!q) return false;

    q->count++;

    if(!q->front) {
        q->front = q->back = new_node(data, q->elem_size);
        return true;
    }

    q->back->next = new_node(data, q->elem_size);
    q->back = q->back->next;
    return true;
}

bool dequeue(queue* q) {
    if(is_queue_empty(q)) return false;

    node* del = q->front;

    if(!q->front->next) {
        q->front = q->back = NULL;
    } else {
        q->front = q->front->next;
    }

    destroy_node(del);
    q->count--;
    return true;
}

bool queue_front(const queue* q, void* out) {
    if(is_queue_empty(q)) return false;

    memcpy(out, q->front->data, q->elem_size);
    return true;
}

bool queue_back(const queue* q, void* out) {
    if(is_queue_empty(q)) return false;

    memcpy(out, q->back->data, q->elem_size);
    return true;
}

void destroy_queue(queue** q) {
    node* now = (*q)->front;
    while(now) {
        node* del = now;
        now = now->next;
        destroy_node(del);
    }
    free(*q);
    *q = NULL;
}