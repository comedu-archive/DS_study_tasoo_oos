#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define element char

typedef struct node {
    element data;
    struct node *next;
    struct node *prev;
} node;

typedef struct list {
    node* head;
    node* now;
} list;

list* new_list() {
    list* new = malloc(sizeof(list));
    if (new == NULL) {
        exit(EXIT_FAILURE);
    }

    new->now = NULL;
    new->head = NULL;
    return new;
}

node* new_node(const element data) {
    node* new = malloc(sizeof(node));
    if (new == NULL) {
        exit(EXIT_FAILURE);
    }

    new->data = data;
    new->next = NULL;
    new->prev = NULL;
    return new;
}

void add_first_node(list* my_list, const element data) {
    node* first = new_node(data);
    first->next = first;
    first->prev = first;
    my_list->now = first;
    my_list->head = first;
}

void check_list_initialized(const list* my_list) {
    if(!my_list || !my_list->head) {
        exit(EXIT_FAILURE);
    }
}

void display(const list* my_list) {
    check_list_initialized(my_list);

    printf("  %c  ", my_list->head->data);
    for(const node* now = my_list->head->next; now != my_list->head; now = now->next) {
        printf("  %c  ", now->data);
    }
    printf("\n");

    printf(" H ");
    if(my_list->head == my_list->now)
        printf("N");
    else
        printf(" ");
    printf(" ");
    for(const node* now = my_list->head->next; now != my_list->head; now = now->next) {
        printf("   ");
        if(now == my_list->now)
            printf("N");
        else
            printf(" ");
        printf(" ");
    }
    printf("\n");
    printf("\n");
}

element get_data(const list* my_list) {
    check_list_initialized(my_list);
    return my_list->now->data;
}

void traverse_front(list* my_list, const int n) {
    check_list_initialized(my_list);
    for(int i=0;i<n;i++) {
        my_list->now = my_list->now->prev;
    }
}

void traverse_rear(list* my_list, const int n) {
    check_list_initialized(my_list);
    for(int i=0;i<n;i++) {
        my_list->now = my_list->now->next;
    }
}

void delete_now(list* my_list) {
    check_list_initialized(my_list);

    node* prev = my_list->now->prev;
    node* next = my_list->now->next;
    node* del  = my_list->now;

    if(prev == del || next == del) {
        free(del);
        my_list->head = NULL;
        my_list->now = NULL;
        return;
    }

    my_list->now = next;

    if(my_list->head == del)
        my_list->head = next;

    prev->next = next;
    next->prev = prev;

    free(del);
}

void add_now(list* my_list, const element data) {
    if(!my_list) {
        exit(EXIT_FAILURE);
    }
    if(my_list->now == NULL) {
        add_first_node(my_list, data);
        return;
    }
    node* ins = new_node(data);
    node* next = my_list->now->next;
    node* prev = my_list->now;
    ins->next = next;
    ins->prev = prev;
    prev->next = ins;
    next->prev = ins;
    my_list->now = ins;
}

void add_tail(list* my_list, const element data) {
    if(!my_list) {
        exit(EXIT_FAILURE);
    }
    if(my_list->now == NULL) {
        add_first_node(my_list, data);
        return;
    }
    node* head = my_list->head;
    node* new = new_node(data);
    node* tail = my_list->head->prev;

    tail->next = head->prev = new;
    new->next = head;
    new->prev = tail;

    if(tail == my_list->now) {
        my_list->now = my_list->now->next;
    }
}

void add_pos(list* my_list, const int pos, const element data) {
    if(!my_list) {
        exit(EXIT_FAILURE);
    }
    if(my_list->now == NULL) {
        add_first_node(my_list, data);
        return;
    }

    node* now = my_list->head;

    if(pos >= 0)
        for(int i=0;now && i<pos;i++)
            now = now->next;
    else
        for(int i=0;now && i<-pos;i++)
            now = now->prev;

    if(now == NULL) {
        exit(EXIT_FAILURE);
    }

    node* ins = new_node(data);
    node* next = now;
    node* prev = now->prev;

    prev->next = next->prev = ins;
    ins->next = next;
    ins->prev = prev;

    my_list->now = ins;
}

void replace(list* my_list, const element data) {
    check_list_initialized(my_list);
    my_list->now->data = data;
}

long long data_count(const list* my_list) {
    if(!my_list || !my_list->now) return 0L;

    long long cnt = 1;
    for(const node* now = my_list->head->next; now != my_list->head; now = now->next)
        cnt++;

    return cnt;
}

bool is_member(const list* my_list, const element target) {
    if(!my_list || !my_list->now) return false;

    if(my_list->head->data == target) return true;
    for(const node* now = my_list->head->next; now != my_list->head; now = now->next)
        if(now->data == target) return true;

    return false;
}

void clear(list* my_list) {
    if(!my_list) {
        exit(EXIT_FAILURE);
    }
    if(my_list->now == NULL) {
        return;
    }

    node* now = my_list->head->next;
    while(now != my_list->head) {
        //printf("%c ", now->data);
        node* next = now->next;
        free(now);
        now = next;
    }

    //printf("%c ", now->data);
    free(now);

    my_list->head = NULL;
    my_list->now = NULL;
}

bool is_empty(const list* my_list) {
    if(!my_list || !my_list->now) {
        return true;
    }
    return false;
}

void linked_list_test_case_1() {
    list *my_list = new_list();

    add_tail(my_list, 'a');
    add_tail(my_list, 'b');
    add_tail(my_list, 'c');
    add_tail(my_list, 'd');
    display(my_list);

    printf("Recieved data: %c\n", get_data(my_list));

    traverse_front(my_list, 1);
    display(my_list);

    delete_now(my_list);
    display(my_list);

    add_now(my_list, 'b');
    add_now(my_list, 'c');
    add_now(my_list, 'f');
    display(my_list);

    traverse_front(my_list, 4);
    add_now(my_list, 'e');
    add_now(my_list, 'f');
    add_now(my_list, 'g');
    display(my_list);

    traverse_rear(my_list, 4);
    delete_now(my_list);
    display(my_list);

    traverse_front(my_list, 1);
    add_pos(my_list, 0, 'x');
    add_pos(my_list, 0, 'y');
    add_pos(my_list, 0, 'z');
    display(my_list);
    // 자료의 요구사항과 최대한 비슷하게 코드를 짰지만 자료의 요구사항에 모순이 있어 정답인지 불명확함

    replace(my_list, 'm');
    display(my_list);

    traverse_front(my_list, 4);
    printf("Recieved data: %c\n", get_data(my_list));
    display(my_list);

    printf("Number of values: %lld\n", data_count(my_list));
    const element target = 'e';
    printf("Result of is_member of %c: %d\n", target, is_member(my_list, target));

    // traverse_front(my_list, 2-1)
    // replace(my_list, data)
    // Inlab 자료에 해당 부분의 설명이 다른 부분들과 모순이 있다고 생각하여 구현하지 않음

    printf("Is my_list empty? %d\n", is_empty(my_list));
    clear(my_list);
    printf("Is my_list empty? %d\n", is_empty(my_list));

    free(my_list);
}

void linked_list_test_case_2() {
    list* empty_list = new_list();

    printf("Is my_list empty? %d\n", is_empty(empty_list));
    const element target = 'e';
    printf("Result of is_member of %c: %d\n", target, is_member(empty_list, target));

    free(empty_list);
}
