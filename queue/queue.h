//
// Created by tasoo on 2024-01-08.
//

#ifndef QUEUE_H
#define QUEUE_H
#include <stddef.h>
#include <stdbool.h>

typedef struct QUEUE_ queue;

// Function prototypes for the queue operations
queue *new_queue(const size_t);
size_t queue_count(const queue*);
bool is_queue_empty(const queue*);
bool enqueue(queue*, const void*);
bool dequeue(queue*);
bool queue_front(const queue*, void*);
bool queue_back(const queue*, void*);
void destroy_queue(queue** q);

#endif //QUEUE_H
