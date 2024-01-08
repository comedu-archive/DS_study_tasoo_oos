#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 연결 리스트의 각 데이터(여기선 글자)를 저장하는 용도의 구조체 자료형
typedef struct node{
    char data;
    struct node* below;
}node;

// 스택을 쉽게 구현하기 위한 구조체
// 물론 top만 가지고도 충분히 구현 가능하나,
// 그때그때 스택에 들어간 값 개수를 세주기엔 너무 오래 걸리므로
// (모든 node를 하나하나 세야 함)
// push, pop 할 때마다 변경해두는 편이 효울적이다.
typedef struct Stack{
    node* top;
    int size;
}stack;

void destroy_stack(stack**); // 함수 프로토타입, 함수를 정의하기 전에 사용하고 싶다면 이렇게 먼저 선언해두어야 한다.
// 자세한 설명은 아래 정의 부분에서 하겠다.

// 새로운 스택을 만들 때 편하게 만들기 위한 함수
// 이렇게 하면 우리가 스택을 생성할 때 잡다한 점(메모리, 초기화 등)
// 을 신경쓸 필요가 없어 좋다.
stack* new_stack() {
    stack* st = malloc(sizeof(stack));
    // malloc 함수를 쓸 때 숫자를 직접 쓰기보단 sizeof함수를 써서 직접 계산시키는 편이
    // 속 편할 것이다. 이거때문에 오류나면 절대 못 찾을걸?

    // malloc 함수가 메모리 할당에 실패하면 NULL(빈 포인터)를 반환한다.
    // NULL이 저장된 포인터 변수는 당연히 사용할 수 없으므로, 이렇게 예외 처리를 해주어야 한다.
    if(st == NULL) {
        printf("Not enough memory");
        exit(EXIT_FAILURE);
        // exit 함수는 그냥 프로그램을 끝내버린다.
        // 나는 일단 그냥 썼지만, GPT가 말하길 그냥 끝내는건 좀 너무하지 않냐던데,
        // 다른 방법은 너무 어려워 보여 걍 썼음을 밝힌다.
    }

    st->size = 0;
    st->top = NULL;
    // 변수 초기화
    // 이거 안 해주면 쓰레기값 들어가는거 알지?
    return st;
}

// 새로운 데이터를 넣을 때, node 자료형 변수를 만들어주는 함수
// 구조는 new_stack과 유사하므로 패스
node* new_node(const char data) {
    node* another = malloc(sizeof(node));
    if(another == NULL) {
        printf("Not enough memory");
        exit(EXIT_FAILURE);
    }
    another->data = data;
    another->below = NULL;
    return another;
}

// 스택이 비어있는지, 아니면 값이 있는지 확인하는 함수
// bool 자료형은 true/false 두 상태 중 하나를 저장하는 자료형이다.
// st->top을 확인하려면 먼저 st가 비어있지 않아야하기에 다음과 같이 작성함.

// 참고로, NULL은 숫자로 표현하면 그냥 0이고, 0은 컴퓨터가 false로 알아먹기에,
// 다음 조건문은 아래와 같이도 쓸 수 있다.
// return !st || !st->top;
bool is_empty(const stack* st){
    return st == NULL || st->top == NULL;
}

// 스택에 새 변수를 넣는 함수
void push(stack* st, const int a){
    // 변수를 선언할 때 자료형 앞에 const를 붙이면, 해당 변수를 변경하려 할 때 컴파일 에러가 난다.
    // 바뀌면 안 되는 변수 앞에는 const를 달아보자.

    node* n = new_node(a);

    // 여기서 n은 새로 맨 위에 쌓일 놈이고,
    // st->top은 이제 n 아래에 깔릴 놈이다.

    // 단, PPT에서 언급했듯이 동적 할당(malloc)을 통해 할당받은 힙 메모리에는 명시적인 이름이 없기에,
    // 나는 해당 힙 메모리를 가리키는 포인터 변수가 주체인 것처럼 이야기할 때가 있는데,
    // 포인터 변수는 그 메모리들을 가리킬 뿐이지, 그 메모리 자체가 아님을 명심하라.

    n->below = st->top; // n 아래에 st->top이 있음을 표시하고
    st->top = n;        // 이제 맨 위에 n이 있음을 표시한다.
    st->size++;         // 스택에 들어간 값의 개수가 늘어났으므로 이를 size에 반영한다.
}

// 스택의 맨 위(제일 최근에 push된) 변수를 삭제하는 함수
void pop(stack* st){
    node* del = st->top;
    // 맨 위에 지워질 놈의 주소를 미리 저장해 둔다.
    // 이는 st->top이 node 자료형이기에, push된 데이터 뿐만 아니라
    // 자신 아래(이전에 push된)의 node 자료형의 주솟값까지 담고 있기 때문이다.
    // 바로 free해버리면 아래에 뭐가 있었는지도 사라지겠지? 그래서 그렇다.

    st->top = st->top->below; // 바로 아랫놈이 이제 맨 위의 값이므로 이를 반영
    // 중첩 화살표 연산자의 사용 이유는 peek 함수 부분에서 설명함

    free(del); // ★삭★제★

    st->size--; // 스택에서 값이 삭제되어 개수가 줄어들었으므로, 반영
}

// 스택의 맨 위(제일 최근에 push된)에 있는 값을 return하는 함수
char peek(const stack* st){
    if(is_empty(st)) return '\0'; // 스택이 비어있을 땐 당연히 top도 없을테니 예외처리
    return st->top->data;
    // st는 stack 자료형의 구조체 변수를 가리키는 포인터 변수이므로, 멤버 변수에 접근하려면 화살표 연산자를 사용해야 함
    // st->top은 node 자료형의 구조체 변수를 가리키는 포인터 변수이므로, 멤버 변수에 접근하려면 화살표 연산자를 사용해야 함
    // 이런 식으로 화살표 연산자를 여러번 사용해야 할 때가 꽤나 있음

    // 단, 화살표 연산자를 사용하다가 중간에 (이 예시에서는 st->top)이 NULL 등 유효하지 않는 메모리 주소를 가리킬 경우,
    // 그 뒤로 프로그램이 어떤 행동을 할 지 알 수 없으므로 꼭 if문 등을 통해 확인해주도록 하자.
}

// 스택 안에 값이 몇개 들어 있는지 알려주는 함수
int size(const stack* st){
    if(is_empty(st)) return 0; // 비어있으면 당연히 0개지만, 예외처리를 위함
    return st->size;           // 기록해둔 size 변수를 그대로 반환
}

// 스택 내의 값을 출력해주는 함수
void print(stack* st) {
    if(is_empty(st)) { // 예외처리
        printf("Stack is empty");
        return;
    }

    // 실습 교안을 보면 스택 2개를 사용하라는 요구사항이 있는데, 그 이유는 다음과 같다.
    // 스택은 구조상 가장 먼저 들어간 값이 제일 아래에 깔리므로 가장 나중에 나온다.
    // 그렇기에, 스택에 일련의 값을 쭉 넣은 뒤, 값을 쭉 빼내면 값들의 순서가 거꾸로 된다.
    // 그러면 이걸 한번 더 해주면 순서가 원래대로 되겠지? 그래서 스택을 두개 쓰라고 한 것이다.

    stack* rev = new_stack(); // 뒤집기용 스택

    while(!is_empty(st)) {
        // 스택이 비지 않았을 동안 계속 반복하게 함
        // 스택은 값의 개수가 유동적이기에 스택 내의 모든 값을 순회해야 할 때 자주 사용하는 방법이다.

        push(rev, peek(st)); // st의 값을 rev에 넣는다. 이 때 rev에는 아까 말했듯이 값들이 역순으로 들어간다.
        pop(st);               // st에서 값을 빼낸다.
    }

    while(!is_empty(rev)) {
        push(st, peek(rev));
        printf("%c ", peek(rev));
        // 이때 순서가 st에서 맨 아래에 깔려있던 것이 먼저 나올 것이므로, 이때 출력한다.
        pop(rev);
    }
    printf("\n"); // 엔터

    destroy_stack(&rev); // rev스택은 이제 쓸모가 없으므로 삭제한다. (free해준다.)
}

// 스택이 가득 찼는지 알려주는 함수
bool is_full(const stack* st) {
    return false; // 차겠음? ㅋ
}

// target 문자가 스택 내에 존재하는지 알려주는 함수
// 있으면 true, 없으면 false
bool is_member(const stack* st, const char target) {
    if(is_empty(st))return false; // 예외처리

    // 이 함수에서는 연결리스트의 특성을 이용하여 값들을 처리했다.

    node* now = st->top;  // 연결리스트의 시작 부분의 주소를 대입한다.
    while(now) {          // 아까 말했듯이, NULL은 거짓으로 취급되므로 이렇게만 써도 NULL인지 아닌지 검사하는 것과 다름없다.
        if(now->data == target)
            break;        // 원하는 문자를 찾았으면 반복문을 바로 빠져나간다.
        now = now->below; // 바로 다음(아래) node 변수로 넘어간다.
    }

    // 만약 포인터 변수 now에 NULL값이 있으면 target 문자를 못 찾은 것이고, 아니면 찾은 것이다.
    // 이유는 변수가 NULL을 가지고 있을 경우는 오직 연결 리스트 내에서 원하는 문자를 찾지 못해 연결 리스트 끝까지 간 경우 뿐이기 때문이다.
    if(now)
        return true;
    return false;
}

// 패스
void replace_top(const stack* st, const char target) {
    if(is_empty(st)) return;

    st->top->data = target;
}

// 다 쓴 stack 변수를 삭제하는 함수
void destroy_stack(stack** st) {
    // 보면 자료형 뒤에 별이 두 개 붙어있는 것을 알 수 있는데,
    // 이는 해당 변수가 포인터 변수의 주소를 저장하는 변수임을 나타낸다.

    // 즉, stack 구조체 변수가 힙 메모리에 할당되어 있고,
    // 어딘가의 포인터 변수가 해당 힙 메모리를 가리키고 있으며,
    // st는 해당 포인터 변수의 주소를 받아 그 포인터 변수를 가리키고 있다는 말이다.

    while(!is_empty(*st)) // 스택이 빌 때까지 pop으로 값들을 삭제한다.
        pop(*st);         // 연결리스트의 특성을 사용할 수도 있겠으나, 귀찮다.
    free(*st);            // 맨 처음에 할당했던 stack 구조체 변수도 할당 해제한다.
    *st = NULL;
    // 그리고 그 stack 구조체 변수를 가르키던 포인터 변수에 비었다는 의미의 NULL 값을 대입해준다.
    // 눈치챘을지 모르겠지만, 우리의 프로그램은 우리에게 할당된 메모리 주소와 그렇지 않은 주소를 구별하지 못한다.
    // 만약 할당되지 않은 메모리 주소에 접근했다가 운이 안 좋으면 운영체제가 예끼 이놈 하면서 우리의 소중한 프로그램을 강종시킨다.
    // 그런 슬픈 사태가 실수로 일어나는 것을 방지하기 위해 포인터가 비어있다는 정보를 저장하는 것이다.
    // 그래도 비어있는 포인터에 접근해도 튕기는 건 마찬가지이기에, 항상 포인터가 비어있는지 체크하는 습관을 들이자.

    // 이렇게 메모리를 제대로 할당 해제해주지 않으면
    // 여기에 할당된 메모리를 다른 곳에 쓰지 못하고 계속 메모리 공간을 차지하고 있게 된다.
    // 이 현상을 메모리 누수라고 하며, 이 현상이 심하면 프로그램이 튕긴다.
}

// main 함수에는 별거 없다. 어차피 저거 GPT가 짜줬다.

// 이제부터 이 코드를 참고하여 큐를 구현해야 할 것이다.
// 이를 위한 팁을 조금만 주자면, 두 자료구조간의 차이를 생각해보라.
// 스택은 자료를 넣고 빼는 구멍이 하나 뿐이었지만, 큐는 넣는 구멍과 빼는 구멍이 따로 있다.
// 즉, 단방향 연결 리스트의 구조는 유지하되, 연결리스트의 시작점 (여기에선 st->top)뿐만 아니라
// 끝점도 포인터 변수에 저장해 두는 게 좋겠지?


/// 과제: 연결리스트를 이용한 큐 직접 구현하기

// 코드는 새로 짜서 깃헙에 올릴 것 (이거 재활용하는 것보다 편할거임)
// 상세 조건은 교안에 있는 것을 따른다. 단, head, tail 함수는 배열에서의 위치가 아닌 그곳에 저장된 값을 출력하는 형식으로 만들 것.
// 깐깐하게 확인할 것이므로 예외처리 잘 해올것, 단, 교안에 존재하지 않는 입력은 들어오지 않는다고 생각해도 좋다.

// GPT보고 짜달라하는 것을 지양할 것. 당신만 손해다.
// 버그도 한번 직접 고쳐보는 것을 추천한다. 해봐야 버그도 잘 고친다.
// 단, 다 짜고 예외처리도 다 된 코드를 GPT에게 보여주면서 "내 코드 어때?" 하는 것은 한번 해 보아라.
// 나보다 더 잘 해줄거임. GPT가 해주는 조언을 들으며 코드를 개선해보면 실력이 절로 늘 것이다.
// 또한, main 함수도 GPT보고 짜달라고 해라. 안 짜오면 채점 안해줌 ㅡㅡ



/// 곁다리: 후위표기식 문제 풀어보기

// 이제 구현은 해야겠는데 너무 막막하고 숨이 가쁘고 어지려운가? 그렇다면 숨을 돌리기 위한 백준 문제는 어떠한가?
// 교안에 나와 있는 중위 표기식을 후위 표기식으로 바꾸는 문제이다.
// 이 파일에 구현된 스택을 이용하여 풀어보아라. 아니면 다른 방법을 써 보아도 좋다.
//https://www.acmicpc.net/problem/1918


/// 도전 과제 1: 연결 리스트를 이용한 덱 직접 구현하기

// 여기까지 읽어주는 사람이 있다면 진심으로 감사를 표한다. 이 과제를 할 사람이 있을진 모르겠지만 난 아직 희망을 놓지 않고 있다.
// 지난 과제도 무려 4명이나 해주지 않았는가? 이것도 한명쯤은 해주리라 믿는다. 아님 말고...

// 덱이란, 양쪽 구멍에서 넣기도 하고 빼기도 하는 자료구조이다.
// 연결리스트 기준으로 표현하자면, 양쪽으로 값이 붙었다 떨어졌다 하기에,
// 단방향 연결 리스트로는 구현할 수 없다.

/*
typedef struct node{
    int data;
    struct node* next;
    struct node* prev;
};
*/

// 다음과 같은 구조의 이중 연결 리스트를 사용하는 것을 추천한다.




// 동적 할당을 이용하여 알아서 늘어났다가 줄어들었다 하는 배열의 코드도 같이 배포한다.
// 걔는 안 읽어도 큰 상관 없지만....................................
// 그래도 읽어서 저거 다 이해하면 C는 이제 조빱일걸~? 정성을 봐서 읽어줘라.



int main() {
    stack* my_stack = new_stack();

    while (true) {
        char command[1000];
        while (scanf("%999s", command) == 1) { // Read until a non-successful input (like newline)
            char ch;
            int num;
            switch (command[0]) {
                case '+':
                    push(my_stack, command[1]);
                break;
                case '-':
                    if (!is_empty(my_stack)) {
                        printf("Popped: %c\n", peek(my_stack));
                        pop(my_stack);
                    } else {
                        printf("Stack is empty.\n");
                    }
                break;
                case 'P':
                    if (!is_empty(my_stack)) {
                        printf("Peek: %c\n", peek(my_stack));
                    } else {
                        printf("Stack is empty.\n");
                    }
                break;
                case 'T':
                    if (!is_empty(my_stack)) {
                        printf("Top: (%d, %c)\n", size(my_stack), peek(my_stack));
                    } else {
                        printf("Stack is empty.\n");
                    }
                break;
                case 'L':
                    print(my_stack);
                break;
                case '#':
                    printf("Size: %d\n", size(my_stack));
                break;
                case '?':
                    printf("%s\n", is_member(my_stack, command[1]) ? "True" : "False");
                break;
                case '=':
                    if(!is_empty(my_stack))
                        replace_top(my_stack, command[1]);
                    else
                        printf("Stack is empty\n");
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