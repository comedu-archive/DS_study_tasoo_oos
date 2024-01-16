#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct Node_ {
    char data;
    struct Node_* next;
} node;

typedef struct QUEUE_ {
    size_t count;
    node* front;
    node* back;
} queue;

queue* new_queue() {
    queue* q = malloc(sizeof(queue));
    if (!q) {
        perror("Not enough memory");
        exit(EXIT_FAILURE);
    }
    q->front = NULL;
    q->back = NULL;
    q->count = 0;
    return q;
}

static node* new_node(const char data) {
    node* n = malloc(sizeof(node));
    if (!n) {
        perror("Not enough memory");
        exit(EXIT_FAILURE);
    }

    n->data = data;
    n->next = NULL;

    return n;
}

size_t queue_count(const queue* q) {
    return q->count;
}

bool is_queue_empty(const queue* q) {
    return !q || !q->front;
}

bool enqueue(queue* q, const char data) {
    if (!q) return false;

    q->count++;

    if (!q->front) {
        q->front = q->back = new_node(data);
        return true;
    }

    q->back->next = new_node(data);
    q->back = q->back->next;
    return true;
}

bool dequeue(queue* q) {
    if (is_queue_empty(q)) return false;

    node* del = q->front;

    if (!q->front->next) {
        q->front = q->back = NULL;
    } else {
        q->front = q->front->next;
    }

    free(del);
    q->count--;
    return true;
}

bool queue_front(const queue* q, char* out) {
    if (is_queue_empty(q)) return false;

    *out = q->front->data;
    return true;
}

bool queue_back(const queue* q, char* out) {
    if (is_queue_empty(q)) return false;

    *out = q->back->data;
    return true;
}

bool is_member(const queue* q, const char target) {
    if(is_queue_empty(q)) return false;

    node* now = q->front;
    while(now) {
        if(now->data == target)
            break;
        now = now->next;
    }

    if(now)
        return true;
    return false;
}

bool replace_queue_back(const queue* q, const char target) {
    if(is_queue_empty(q)) return false;

    q->back->data = target;
    return true;
}

void print_queue(const queue* q) {

    for(node* now = q->front;now;now = now->next)
        printf("%c ", now->data);
    printf("\n");
}

void destroy_queue(queue** q) {
    node* now = (*q)->front;
    while (now) {
        node* del = now;
        now = now->next;
        free(del);
    }
    free(*q);
    *q = NULL;
}



void console_io(queue* q) {
    char input[10], value;
    bool running = true;

    printf("Queue Console I/O\n");
    //printf("Use +<val> to enqueue, - to dequeue, H for front, T for back, L to list, # to count, E to check if empty, ?<val> to find, =<val> to replace back, C to close.\n");

    while (running) {
        //printf("Enter command: ");
        if (scanf("%s", input) != 1) {
            printf("Invalid input. Try again.\n");
            continue;
        }

        switch (input[0]) {
            case '+':
                if (strlen(input) != 2) {
                    printf("Invalid input format for enqueue. Use +<val>\n");
                    break;
                }
                enqueue(q, input[1]);
                break;
            case '-':
                dequeue(q);
                break;
            case 'H':
                if (queue_front(q, &value)) {
                    printf("Front: %c\n", value);
                } else {
                    printf("Queue is empty.\n");
                }
                break;
            case 'T':
                if (queue_back(q, &value)) {
                    printf("Back: %c\n", value);
                } else {
                    printf("Queue is empty.\n");
                }
                break;
            case 'L':
                print_queue(q);
                break;
            case '#':
                printf("Size: %zu\n", queue_count(q));
                break;
            case 'E':
                printf("Queue is %sempty.\n", is_queue_empty(q) ? "" : "not ");
                break;
            case '?':
                if (strlen(input) != 2) {
                    printf("Invalid input format for is_member. Use ?<val>\n");
                    break;
                }
                printf("%c is %sin the queue.\n", input[1], is_member(q, input[1]) ? "" : "not ");
                break;
            case '=':
                if (strlen(input) != 2) {
                    printf("Invalid input format for replace. Use =<val>\n");
                    break;
                }
                replace_queue_back(q, input[1]);
                break;
            case 'C':
                running = false;
                break;
            default:
                printf("Unknown command. Try again.\n");
        }
    }
}

int main() {
    queue* q = new_queue();
    console_io(q);
    destroy_queue(&q);
    return 0;
}

