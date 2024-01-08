//
// Created by tasoo on 2024-01-08.
//

#ifndef STACK_H
#define STACK_H
#include <stdbool.h>

typedef struct Stack_ stack;

stack* new_stack(const size_t);
bool is_stack_empty(const stack*);
long long count_stack(const stack*);
void static _resize_stack(stack*, const bool);
bool push_stack(stack*, const void*);
bool pop_stack(stack*);
void print_stack(const stack*, void (*)(const void*));
bool peek_stack(const stack*, void* out);
bool is_member_in_stack(const stack*, const void*, long long (*)(const void*, const void*));
bool replace_top_stack(stack*, const void*);
void destroy_stack(stack**);

#endif //STACK_H
