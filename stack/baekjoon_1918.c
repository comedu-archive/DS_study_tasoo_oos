#include "stack.c"

// https://www.acmicpc.net/problem/1918
int main() {
    char inp[101] = "A+(B-C*D)/E-(F/G-H);
    stack* op = new_stack(sizeof(char));
    char out[101] = {};
    long long out_len = 0;

    for (int i = 0; inp[i] != '\0'; i++) {
        if ('A' <= inp[i] && inp[i] <= 'Z') {
            out[out_len++] = inp[i];
        } else if (inp[i] == '(') {
            push_stack(op, &inp[i]);
        } else if (inp[i] == ')') {
            char top_elem;
            if (!is_stack_empty(op)) peek_stack(op, &top_elem);
            while (!is_stack_empty(op) && top_elem != '(') {
                out[out_len++] = top_elem;
                pop_stack(op);
                if (!is_stack_empty(op)) peek_stack(op, &top_elem);
            }
            if (!is_stack_empty(op)) pop_stack(op);
        } else if (inp[i] == '+' || inp[i] == '-') {
            char top_elem;
            if (!is_stack_empty(op)) peek_stack(op, &top_elem);
            while (!is_stack_empty(op) && top_elem != '(') {
                out[out_len++] = top_elem;
                pop_stack(op);
                if (!is_stack_empty(op)) peek_stack(op, &top_elem);
            }
            push_stack(op, &inp[i]);
        } else if (inp[i] == '*' || inp[i] == '/') {
            char top_elem;
            if (!is_stack_empty(op)) peek_stack(op, &top_elem);
            while (!is_stack_empty(op) && (top_elem == '*' || top_elem == '/')) {
                out[out_len++] = top_elem;
                pop_stack(op);
                if (!is_stack_empty(op)) peek_stack(op, &top_elem);
            }
            push_stack(op, &inp[i]);
        }
    }

    while (!is_stack_empty(op)) {
        char top_elem;
        peek_stack(op, &top_elem);
        out[out_len++] = top_elem;
        pop_stack(op);
    }

    printf("%s", out);

    destroy_stack(&op);
}
