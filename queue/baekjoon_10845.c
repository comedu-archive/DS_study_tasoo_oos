
#include <stdio.h>
#include <string.h>
#include "queue.h"

int main() {
    queue *num = new_queue(sizeof(int));
    int n;
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        char line[100];
        scanf("%s", line);

        if (strncmp(line, "push", 4) == 0) {
            int x;
            scanf("%d", &x);
            enqueue(num, &x);
        } else if (strncmp(line, "pop", 3) == 0) {
            if (!is_queue_empty(num)) {
                int result;
                queue_front(num, &result);
                printf("%d\n", result);
                dequeue(num);
            } else {
                printf("-1\n");
            }
        } else if (strncmp(line, "size", 4) == 0) {
            printf("%zu\n", queue_count(num));
        } else if (strncmp(line, "empty", 5) == 0) {
            printf("%d\n", is_queue_empty(num));
        } else if (strncmp(line, "front", 5) == 0) {
            int result;
            if (queue_front(num, &result)) {
                printf("%d\n", result);
            } else {
                printf("-1\n");
            }
        } else if (strncmp(line, "back", 4) == 0) {
            int result;
            if (queue_back(num, &result)) {
                printf("%d\n", result);
            } else {
                printf("-1\n");
            }
        }
    }

    destroy_queue(&num);
    return 0;
}
