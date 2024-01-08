#ifndef tasoo_stack      // 이 헤더가 여러 번 포함되지 않도록 하는 전처리 지시문.
#define tasoo_stack      // 헤더가 포함되었음을 표시하기 위한 매크로 정의.

#include <stdio.h>      // printf, fprintf 같은 표준 I/O 라이브러리 포함.
#include <stdlib.h>     // 메모리 할당 함수 및 exit()를 위한 표준 라이브러리 포함.
#include <string.h>     // memcpy와 같은 메모리 조작 함수를 위한 문자열 라이브러리 포함.
#include "stack.h"      // 스택 헤더 파일 포함. 이 파일이 존재하고 올바르게 참조되었는지 확인하세요.

// 스택 구조체 정의.
// 포인터는 메모리 주소를 저장하는 변수입니다.
// 코드에서 `void* arr`은 스택을 나타내는 메모리의 시작 부분을 가리키는 데 사용됩니다.
// void* 는 모든 데이터 유형의 메모리를 가리킬 수 있음을 의미합니다.
typedef struct Stack_ {
    void* arr;          // 배열의 기본을 가리키는 포인터. 'void*'는 어떤 데이터 타입도 저장할 수 있게 해줍니다.
    long long top;      // 스택의 최상위 요소 인덱스. 스택이 비어 있을 때는 '-1'.
    size_t cap;         // 스택의 현재 용량.
    size_t elem_size;   // 스택의 각 요소 크기.
} stack;

// 새 스택을 생성하는 함수.
// size_t는 주로 크기를 표현할 때 사용되는 부호 없는 데이터 자료형으로,
// 시스템(운영체제)에서 처리할 수 있는 가장 큰 객체의 크기를 포함할 수 있을 만큼 충분히 크도록 보장됩니다.
stack* new_stack(const size_t elem_size) {
    stack* st = malloc(sizeof(stack)); // 스택 구조체에 대한 메모리 할당.
    if (st == NULL) {                  // 메모리 할당 실패 여부 확인.
        fprintf(stderr, "Not enough memory\n"); // 표준 오류로 에러 메시지 출력.
        exit(EXIT_FAILURE);             // 실패를 나타내며 프로그램 종료.
    }
    st->top = -1;                      // top 인덱스를 -1로 초기화 (빈 스택). <= 원래 배열 스택은 이게 국룰임
    st->cap = 8;                       // 초기 용량을 8로 설정.
    st->elem_size = elem_size;         // 요소 크기 설정.
    st->arr = malloc(elem_size * st->cap); // 스택 배열에 대한 메모리 할당.
    if (st->arr == NULL) {             // 배열 메모리 할당 실패 여부 확인.
        fprintf(stderr, "Not enough memory\n"); // 표준 오류로 에러 메시지 출력.
        exit(EXIT_FAILURE);             // 실패를 나타내며 프로그램 종료.
    }
    return st;                         // 새로 생성된 스택의 포인터 반환.
}

// 스택이 비어 있는지 확인하는 함수.
// `inline`으로 정의된 함수는 일반 함수 호출을 수행하는 대신 함수가 호출되는 각 위치에 코드를 직접 삽입하도록 컴파일러에 제안됩니다.
// 이는 자주 사용되는 작은 함수를 최적화하는 데 사용됩니다.
inline bool is_stack_empty(const stack* my_stack) {
    return !my_stack || my_stack->top == -1; // 스택이 NULL이거나 top 인덱스가 -1이면 true 반환.
}

// 스택에 있는 요소의 수를 세는 함수.
inline long long count_stack(const stack* my_stack) {
    return my_stack->top + 1;          // 요소 수 반환 (top 인덱스 + 1).
}

// 스택의 크기를 조정하는 내부 함수.
// `static` 키워드를 함수와 함께 사용하면 함수가 정의된 파일로 범위를 제한하므로 다른 파일에서 호출할 수 없습니다.
// 이 함수는 어차피 다른 함수에서 못 쓰니까 아예 엄한데 쓰지 말라고 막아버린 거임
void static _resize_stack(stack* my_stack, const bool increase) {
    size_t new_cap = increase ? my_stack->cap * 2 : my_stack->cap / 2; // 용량을 두 배로 늘리거나 반으로 줄임.
    void* new_arr = realloc(my_stack->arr, my_stack->elem_size * new_cap); // 새 크기에 대한 메모리 재할당.
    if (new_arr == NULL) {             // 메모리 재할당 실패 여부 확인.
        fprintf(stderr, "Not enough memory\n"); // 표준 오류로 에러 메시지 출력.
        exit(EXIT_FAILURE);             // 실패를 나타내며 프로그램 종료.
    }
    my_stack->arr = new_arr;           // 스택의 배열 포인터 업데이트.
    my_stack->cap = new_cap;           // 스택의 용량 업데이트.
}

// 스택에 요소를 푸시하는 함수.
bool push_stack(stack* my_stack, const void* data) {
    if(!my_stack) return false;         // 스택 포인터가 NULL인지 확인.

    if (my_stack->top == my_stack->cap) // 스택이 가득 찼는지 확인.
        _resize_stack(my_stack, true);  // 더 큰 용량으로 스택 크기 조정.

    my_stack->top++;                    // top 인덱스 증가.
    memcpy((char*)my_stack->arr + my_stack->top * my_stack->elem_size, data, my_stack->elem_size); // 데이터를 스택의 상단에 복사.
    return true;                        // 성공을 나타내며 true 반환.
}

// 스택에서 요소를 팝하는 함수.
bool pop_stack(stack* my_stack) {
    if (is_stack_empty(my_stack)) return false; // 스택이 비어 있는지 확인.

    my_stack->top--;                    // top 인덱스 감소.

    if (count_stack(my_stack) > 8 && count_stack(my_stack) == my_stack->cap / 4) // 스택 크기를 줄일 수 있는지 확인.
        _resize_stack(my_stack, false); // 더 작은 용량으로 스택 크기 조정.

    return true;                        // 성공을 나타내며 true 반환.
}

// 스택의 요소를 출력하는 함수.
// print_func 함수처럼 함수 또한 함수의 매개변수로 넘길 수 있다.
void print_stack(const stack* my_stack, void (*print_func)(const void*)) {
    for (size_t i = 0; i <= my_stack->top; i++) {
        print_func(my_stack->arr + i * my_stack->elem_size); // 각 요소에 대해 print 함수 호출.
    }
    printf("\n");                       // 스택 출력 후 개행.
}

// 스택의 최상위 요소를 확인하는 함수.
bool peek_stack(const stack* my_stack, void* out) {
    if (is_stack_empty(my_stack))        // 스택이 비어 있는지 확인.
        return false;

    // 메모리를 한 위치에서 다른 위치로 복사하는 데 사용됩니다.
    // void*는 메모리 크기가 얼마나 될지 몰라서 일반적인 대입이 안됨.
    // 그래서 그 대신 사용하는 함수
    memcpy(out, my_stack->arr + my_stack->top * my_stack->elem_size, my_stack->elem_size); // 'out'에 최상위 요소 복사.
    return true;                        // 성공을 나타내며 true 반환.
}

// 스택에 특정 요소가 있는지 확인하는 함수.
bool is_member_in_stack(const stack* my_stack, const void* target, long long (*cmp_func)(const void*, const void*)) {
    if(is_stack_empty(my_stack)) return false; // 스택이 비어 있는지 확인.

    for (long long i = 0; i <= my_stack->top; i++)
        if (cmp_func(my_stack->arr + (i * my_stack->elem_size), target) == 0) // 각 요소를 대상과 비교.
            return true; // 일치하는 요소가 있으면 true 반환.

    return false; // 일치하는 요소가 없으면 false 반환.
}

// 스택의 최상위 요소를 교체하는 함수.
bool replace_top_stack(stack* my_stack, const void* target) {
    if (is_stack_empty(my_stack)) {     // 스택이 비어 있는지 확인.
        return false;                   // 비어 있으면 false 반환.
    }

    memcpy(my_stack->arr + my_stack->top * my_stack->elem_size, target, my_stack->elem_size); // 최상위 요소를 'target'으로 교체.
    return true;                        // 성공을 나타내며 true 반환.
}

// 스택을 파괴하고 메모리를 해제하는 함수.
void destroy_stack(stack** my_stack) {
    free((*my_stack)->arr);             // 스택 배열에 할당된 메모리 해제.
    free(*my_stack);                    // 스택 구조체에 할당된 메모리 해제.
    *my_stack = NULL;                   // 스택 포인터를 NULL로 설정하여 댕글링 참조 방지.
}

#endif
