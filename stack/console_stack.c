#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_char(const void* data) {
    char* ch = (char*) data;
    printf("%c ", *ch);
}

long long comp_char(const void* a, const void* b) {
    return *(char*)a - *(char*)b;
}

void execute_command(stack* my_stack, const char* command) {
    char ch;
    int num;
    switch (command[0]) {
        case '+':
            push_stack(my_stack, &command[1]);
        break;
        case '-':
            if (!is_stack_empty(my_stack)) {
                peek_stack(my_stack, &ch);
                printf("Popped: %c\n", ch);
                pop_stack(my_stack);
            } else {
                printf("Stack is empty.\n");
            }
        break;
        case 'P':
            if (!is_stack_empty(my_stack)) {
                peek_stack(my_stack, &ch);
                printf("Peek: %c\n", ch);
            } else {
                printf("Stack is empty.\n");
            }
        break;
        case 'T':
            if (!is_stack_empty(my_stack)) {
                peek_stack(my_stack, &ch);
                printf("Top: (%c, %lld)\n", ch, count_stack(my_stack));
            } else {
                printf("Stack is empty.\n");
            }
        break;
        case 'L':
            print_stack(my_stack, print_char);
        break;
        case '#':
            printf("Count: %lld\n", count_stack(my_stack));
        break;
        case '?':
            printf("%s\n", is_member_in_stack(my_stack, &command[1], comp_char) ? "True" : "False");
        break;
        case '=':
            replace_top_stack(my_stack, &command[1]);
        break;
        case 'C':
            destroy_stack(&my_stack);
        printf("Stack destroyed.\n");
        exit(0);
        break;
        default:
            printf("Invalid command.\n");
    }
}

int main() {
    stack* my_stack = new_stack(sizeof(char));
    char command[100];

    while (true) {
        while (scanf("%99s", command) == 1) { // Read until a non-successful input (like newline)
            char ch;
            int num;
            switch (command[0]) {
                case '+':
                    push_stack(my_stack, &command[1]);
                break;
                case '-':
                    if (!is_stack_empty(my_stack)) {
                        peek_stack(my_stack, &ch);
                        printf("Popped: %c\n", ch);
                        pop_stack(my_stack);
                    } else {
                        printf("Stack is empty.\n");
                    }
                break;
                case 'P':
                    if (!is_stack_empty(my_stack)) {
                        peek_stack(my_stack, &ch);
                        printf("Peek: %c\n", ch);
                    } else {
                        printf("Stack is empty.\n");
                    }
                break;
                case 'T':
                    if (!is_stack_empty(my_stack)) {
                        peek_stack(my_stack, &ch);
                        printf("Top: (%c, %lld)\n", ch, count_stack(my_stack));
                    } else {
                        printf("Stack is empty.\n");
                    }
                break;
                case 'L':
                    print_stack(my_stack, print_char);
                break;
                case '#':
                    printf("Count: %lld\n", count_stack(my_stack));
                break;
                case '?':
                    printf("%s\n", is_member_in_stack(my_stack, &command[1], comp_char) ? "True" : "False");
                break;
                case '=':
                    replace_top_stack(my_stack, &command[1]);
                break;
                case 'C':
                    destroy_stack(&my_stack);
                    printf("Stack destroyed.\n");
                    return 0;
                default:
                    printf("Invalid command.\n");
            }
        }
        // Clearing the input buffer in case of any residual characters
        int c;
        do {
            c = getchar();
        } while (c != '\n' && c != EOF);
    }

    return 0;
}