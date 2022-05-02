#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_QUEUE_SIZE 4//최대 3명까지 예약 가능+ 1명은 대출중
#define MAX_STACK_SIZE 100

typedef struct stack {
	int top;
	int book_num[MAX_STACK_SIZE];
}stack;

typedef struct que_node {//예약자의 이름을 저장
	char name[16];
}que_node;

typedef struct queue {
	int front;
	int rear;
	que_node que_name[MAX_QUEUE_SIZE];
}queue;

typedef struct element {
	char ISBN[9];
	char book_name[51];
	int num;//서가번호
	int borrowed;//대출여부
	queue reserved_man;
	int man_num;
}element;

typedef struct Lnode {//도서관리를 원형리스트로 구현
	element key;
	struct Lnode* left, * right;
}Lnode;

int compare(element e1, element e2);//두 구조체의 ISBN비교
void init(Lnode* head);//헤드 노드 초기화
void insert(Lnode* before, element key);//원형리스트 삽입
void delet(Lnode* head, Lnode* removed);//원형리스트 삭제
Lnode* search(Lnode* head, element key);//검색
void borrow(Lnode* point, que_node node);//대출 함수
void return_book(Lnode* point);//반납
void print_node(Lnode* head);//원하는 해당 노드의 구성물 출력
void init_queue(queue* q);//큐 초기화
int is_full(queue* q);
int is_empty(queue* q);
void enqueue(queue* q, que_node qnode);
void dequeue(queue* q);
void stack_init(stack* s);//스택 초기화
void push(stack* s, int num);
int pop(stack* s);
Lnode* search_booknum(Lnode* head, int num);

int main() {
	int select;
	int select2;
	element e;
	que_node reserver;
	Lnode* head = (Lnode*)malloc(sizeof(Lnode));
	Lnode* temp= NULL;
	char buffer[80];
	char* ptr;
	int book_num = 1;
	stack s;
	char lastcheck;

	stack_init(&s);
	init(head);

	FILE* fp = fopen("도서목록.txt", "r");

	fgets(buffer, sizeof(buffer), fp);

	for (int i = 0; i < 30; i++) {
		fgets(buffer, sizeof(buffer), fp);
		int length = strlen(buffer);
		buffer[length-1] = NULL;

		ptr = strtok(buffer, "	");
		strcpy(e.ISBN, ptr);
		ptr = strtok(NULL, "	");
		ptr = strtok(NULL, "	");
		strcpy(e.book_name, ptr);
		e.num = book_num++;
		insert(head->left, e);
	}
	
	fclose(fp);
	
	printf("도서관리 프로그램\n");
	
	do {
		printf("================================\n\n");
		printf("1. 신규도서 추가\n");
		printf("2. 도서 삭제\n3. 도서 검색\n4. 도서 반납\n5. 프로그램 종료\n");
		printf("\n사용할 번호를 입력하세요:");
		scanf("%d", &select);
		printf("================================\n\n");
		switch(select){
			case 1:
				printf("신규도서의 ISBN을 입력하세요:");
				scanf("%s", e.ISBN);
				printf("\n신규도서의 제목을 입력하세요:");
				scanf("%s", e.book_name);
				if (s.top == -1) {//삭제된 도서의 서가번호는 따로 스택에 보관 없을시엔 추가될때 그냥 서가번호 마지막부터 추가
					e.num = book_num++;
					insert(head->left, e);
				}
				else {
					e.num = pop(&s);
					temp = search_booknum(head, e.num);
					insert(temp, e);
				}
				printf("\n저장되었습니다.\n");
				break;

			case 2:
				printf("삭제할 도서의 ISBN을 입력하세요:");
				scanf("%s", e.ISBN);
				temp = search(head, e);
				if (temp == NULL) {
					printf("검색과 일치하는 도서가 없습니다. 삭제할 수 없습니다.\n");
					break;
				}
				else if (temp->key.borrowed == 1) {
					printf("해당 도서는 현재 대출중입니다. 삭제할 수 없습니다.\n");
					break;
				}
				print_node(temp);
				printf("삭제하시겠습니까?(Y/N):");
				getchar();
				lastcheck = getchar();
				if (lastcheck == 'y' || lastcheck == 'Y') {
					push(&s, temp->key.num);
					printf("%s 도서가 삭제되었습니다.\n", temp->key.book_name);
					delet(head, temp);
				}
				else {
					printf("\n취소하였습니다.\n");
					break;
				}
				break;

			case 3:
				printf("검색할 도서의 ISBN을 입력하세요:");
				scanf("%s", e.ISBN);
				temp = search(head, e);
				if (temp == NULL) {
					printf("검색하고자 하는 도서가 존재하지 않습니다.\n");
					break;
				}
				print_node(temp);
				printf("\n");
				printf("================================\n\n");
				printf("1. 대출신청\n2. 대출 예약\n3. 초기메뉴로 이동\n\n");
				printf("사용할 번호를 입력하세요:");
				scanf("%d", &select2);
				printf("\n================================\n\n");
				switch (select2) {
					case 1:
						if (temp->key.borrowed==1) {
							printf("현재 대출중인 도서입니다, 예약해주십시오.\n");
							break;
						}
						printf("대출 신청자의 이름을 입력하세요:");
						scanf("%s", reserver.name);
						borrow(temp, reserver);
						printf("\n대출되었습니다.\n");
						break;

					case 2:
						if (is_full(&temp->key.reserved_man)) {
							printf("대출을 예약할 수 없습니다.\n");
							break;
						}
						printf("대출 예약자의 이름을 입력하세요:");
						scanf("%s", reserver.name);
						borrow(temp, reserver);
						printf("\n대출이 예약되었습니다.\n");
						break;

					case 3:
						break;
				}
				break;

			case 4:
				printf("반납할 도서의 ISBN을 입력하세요:");
				scanf("%s", e.ISBN);
				temp = search(head, e);
				printf("\n");
				if (is_empty(&temp->key.reserved_man)) {
					printf("해당 도서는 대출되어있지 않습니다.\n");
				}
				return_book(temp);
				printf("반납되었습니다.\n");
				break;

			case 5:
				printf("프로그램을 종료합니다.\n");
				break;
		}
	} while (select != 5);

	FILE* fp2 = fopen("도서관리.txt", "w");
	fputs("ISBN		서가번호	도서제목\n", fp2);
	temp = head->right;
	do {
		fprintf(fp2, "%s	ajou%04d	%s\n", temp->key.ISBN, temp->key.num, temp->key.book_name);
		temp = temp->right;
	} while (temp != head);

	fclose(fp2);
	free(head);

	return 0;
}

void stack_init(stack* s) {
	s->top = -1;
}

void push(stack* s, int num) {
	s->book_num[++(s->top)] = num;
}

int pop(stack* s) {
	return s->book_num[(s->top)--];
}

int compare(element e1, element e2) {
	return strcmp(e1.ISBN, e2.ISBN);
}

void init_queue(queue* q) {
	q->front = 0;
	q->rear = 0;
}

int is_full(queue* q) {
	return ((q->rear + 1) % MAX_QUEUE_SIZE == q->front);
}

int is_empty(queue* q) {
	return (q->front == q->rear);
}

void enqueue(queue* q, que_node qnode) {
	q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
	q->que_name[q->rear] = qnode;
}

void dequeue(queue* q) {
	q->front = (q->front + 1) % MAX_QUEUE_SIZE;
}

void init(Lnode* head) {
	head->left = head;
	head->right = head;
}

void insert(Lnode* before, element key) {
	Lnode* temp = (Lnode*)malloc(sizeof(Lnode));
	temp->key = key;
	temp->left = before;
	temp->right = before->right;
	before->right->left = temp;
	before->right = temp;
	temp->key.man_num = 0;
}

void delet(Lnode* head, Lnode* removed) {
	if (removed == head) return;
	removed->left->right = removed->right;
	removed->right->left = removed->left;
	free(removed);
}

Lnode* search(Lnode* head, element key) {
	Lnode* temp = head->right;
	
	do {
		if (!compare(temp->key, key)) return temp;
		temp = temp->right;
	} while (temp != head);
	return NULL;
}

Lnode* search_booknum(Lnode* head, int num) {
	Lnode* temp = head->right;

	do {
		if (temp->key.num > num) {
			temp = temp->left;
			return temp;
		}
		temp = temp->right;
	} while (temp != head);

	return NULL;
}

void borrow(Lnode* point, que_node node) {
	point->key.borrowed = 1;
	enqueue(&point->key.reserved_man, node);
	point->key.man_num++;
}

void return_book(Lnode* point) {
	point->key.borrowed = 0;
	dequeue(&point->key.reserved_man);
	point->key.man_num--;
}

void print_node(Lnode* head) {
	Lnode* temp = head;
	if (head == NULL)
		return;
	printf("ISBN: %s\n", temp->key.ISBN);
	printf("서가 번호: ajou%04d\n", temp->key.num);
	printf("도서 제목: %s\n", temp->key.book_name);
	if (temp->key.borrowed==1)
		printf("대출 현황: 대출중\n");
	else
		printf("대출 현황: 대출가능\n");
	printf("대출 예약자 수: %d명\n", temp->key.man_num);
}

