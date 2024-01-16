#include "stack.c"

// https://www.acmicpc.net/problem/1935
int main() {
    int n;
    char inp[101];
    double nums[26];

    scanf("%d", &n);
    scanf("%s", inp);
    for(int i = 0; i < n; i++)
        scanf("%lf", &nums[i]);

    stack* cal = new_stack(sizeof(double));

    for(int i = 0; inp[i] != '\0'; i++) {
        if('A' <= inp[i] && inp[i] <= 'Z') {
            push_stack(cal, &nums[inp[i] - 'A']);
        } else {
            double r, l, res;
            peek_stack(cal, &r);
            pop_stack(cal);
            peek_stack(cal, &l);
            pop_stack(cal);

            switch (inp[i]) {
                case '+': res = l + r; break;
                case '-': res = l - r; break;
                case '*': res = l * r; break;
                case '/': res = l / r; break;
                default: fprintf(stderr, "Not valid operator\n"); break;
            }

            push_stack(cal, &res);
        }
    }

    double result;
    peek_stack(cal, &result);
    printf("%.2lf", result);
    destroy_stack(&cal);
}

