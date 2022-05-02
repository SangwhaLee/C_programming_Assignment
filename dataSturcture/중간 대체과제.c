#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_QUEUE_SIZE 4//�ִ� 3����� ���� ����+ 1���� ������
#define MAX_STACK_SIZE 100

typedef struct stack {
	int top;
	int book_num[MAX_STACK_SIZE];
}stack;

typedef struct que_node {//�������� �̸��� ����
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
	int num;//������ȣ
	int borrowed;//���⿩��
	queue reserved_man;
	int man_num;
}element;

typedef struct Lnode {//���������� ��������Ʈ�� ����
	element key;
	struct Lnode* left, * right;
}Lnode;

int compare(element e1, element e2);//�� ����ü�� ISBN��
void init(Lnode* head);//��� ��� �ʱ�ȭ
void insert(Lnode* before, element key);//��������Ʈ ����
void delet(Lnode* head, Lnode* removed);//��������Ʈ ����
Lnode* search(Lnode* head, element key);//�˻�
void borrow(Lnode* point, que_node node);//���� �Լ�
void return_book(Lnode* point);//�ݳ�
void print_node(Lnode* head);//���ϴ� �ش� ����� ������ ���
void init_queue(queue* q);//ť �ʱ�ȭ
int is_full(queue* q);
int is_empty(queue* q);
void enqueue(queue* q, que_node qnode);
void dequeue(queue* q);
void stack_init(stack* s);//���� �ʱ�ȭ
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

	FILE* fp = fopen("�������.txt", "r");

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
	
	printf("�������� ���α׷�\n");
	
	do {
		printf("================================\n\n");
		printf("1. �űԵ��� �߰�\n");
		printf("2. ���� ����\n3. ���� �˻�\n4. ���� �ݳ�\n5. ���α׷� ����\n");
		printf("\n����� ��ȣ�� �Է��ϼ���:");
		scanf("%d", &select);
		printf("================================\n\n");
		switch(select){
			case 1:
				printf("�űԵ����� ISBN�� �Է��ϼ���:");
				scanf("%s", e.ISBN);
				printf("\n�űԵ����� ������ �Է��ϼ���:");
				scanf("%s", e.book_name);
				if (s.top == -1) {//������ ������ ������ȣ�� ���� ���ÿ� ���� �����ÿ� �߰��ɶ� �׳� ������ȣ ���������� �߰�
					e.num = book_num++;
					insert(head->left, e);
				}
				else {
					e.num = pop(&s);
					temp = search_booknum(head, e.num);
					insert(temp, e);
				}
				printf("\n����Ǿ����ϴ�.\n");
				break;

			case 2:
				printf("������ ������ ISBN�� �Է��ϼ���:");
				scanf("%s", e.ISBN);
				temp = search(head, e);
				if (temp == NULL) {
					printf("�˻��� ��ġ�ϴ� ������ �����ϴ�. ������ �� �����ϴ�.\n");
					break;
				}
				else if (temp->key.borrowed == 1) {
					printf("�ش� ������ ���� �������Դϴ�. ������ �� �����ϴ�.\n");
					break;
				}
				print_node(temp);
				printf("�����Ͻðڽ��ϱ�?(Y/N):");
				getchar();
				lastcheck = getchar();
				if (lastcheck == 'y' || lastcheck == 'Y') {
					push(&s, temp->key.num);
					printf("%s ������ �����Ǿ����ϴ�.\n", temp->key.book_name);
					delet(head, temp);
				}
				else {
					printf("\n����Ͽ����ϴ�.\n");
					break;
				}
				break;

			case 3:
				printf("�˻��� ������ ISBN�� �Է��ϼ���:");
				scanf("%s", e.ISBN);
				temp = search(head, e);
				if (temp == NULL) {
					printf("�˻��ϰ��� �ϴ� ������ �������� �ʽ��ϴ�.\n");
					break;
				}
				print_node(temp);
				printf("\n");
				printf("================================\n\n");
				printf("1. �����û\n2. ���� ����\n3. �ʱ�޴��� �̵�\n\n");
				printf("����� ��ȣ�� �Է��ϼ���:");
				scanf("%d", &select2);
				printf("\n================================\n\n");
				switch (select2) {
					case 1:
						if (temp->key.borrowed==1) {
							printf("���� �������� �����Դϴ�, �������ֽʽÿ�.\n");
							break;
						}
						printf("���� ��û���� �̸��� �Է��ϼ���:");
						scanf("%s", reserver.name);
						borrow(temp, reserver);
						printf("\n����Ǿ����ϴ�.\n");
						break;

					case 2:
						if (is_full(&temp->key.reserved_man)) {
							printf("������ ������ �� �����ϴ�.\n");
							break;
						}
						printf("���� �������� �̸��� �Է��ϼ���:");
						scanf("%s", reserver.name);
						borrow(temp, reserver);
						printf("\n������ ����Ǿ����ϴ�.\n");
						break;

					case 3:
						break;
				}
				break;

			case 4:
				printf("�ݳ��� ������ ISBN�� �Է��ϼ���:");
				scanf("%s", e.ISBN);
				temp = search(head, e);
				printf("\n");
				if (is_empty(&temp->key.reserved_man)) {
					printf("�ش� ������ ����Ǿ����� �ʽ��ϴ�.\n");
				}
				return_book(temp);
				printf("�ݳ��Ǿ����ϴ�.\n");
				break;

			case 5:
				printf("���α׷��� �����մϴ�.\n");
				break;
		}
	} while (select != 5);

	FILE* fp2 = fopen("��������.txt", "w");
	fputs("ISBN		������ȣ	��������\n", fp2);
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
	printf("���� ��ȣ: ajou%04d\n", temp->key.num);
	printf("���� ����: %s\n", temp->key.book_name);
	if (temp->key.borrowed==1)
		printf("���� ��Ȳ: ������\n");
	else
		printf("���� ��Ȳ: ���Ⱑ��\n");
	printf("���� ������ ��: %d��\n", temp->key.man_num);
}

