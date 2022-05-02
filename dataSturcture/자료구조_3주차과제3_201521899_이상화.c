#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct dlnode {
	int data;
	struct dlnode* llink;
	struct dlnode* rlink;
}dlnode;

void init(dlnode* head) {
	head->rlink = head;
	head->llink = head;
}

dlnode* check_value_insert(dlnode* head, int data) {
	dlnode* temp = head->rlink;
	if (temp == head) return head;
	if (data > temp->data) {
		return head;
	}
	while (temp->data >= data) {
		if (temp->data == data) {
			printf("�ߺ��� �������Դϴ�.\n");
			return NULL;
		}
		temp = temp->rlink;
	}
	temp = temp->llink;

	return temp;
}

dlnode* check_value_delete(dlnode* head, int data) {
	dlnode* temp;
	temp = head->rlink;
	if (temp == head) return head;
	while (temp->data != data) {
		temp = temp->rlink;
	}
	if (temp->data == data) {
		return temp;
	}
	printf("ã�� ���� �����ϴ�.\n");
	return NULL;
}

void insert(dlnode* before, int data) {
	dlnode* temp = (dlnode*)malloc(sizeof(dlnode));
	temp->data = data;
	temp->llink = before;
	temp->rlink = before->rlink;
	before->rlink->llink = temp;
	before->rlink = temp;
	printf("�����߽��ϴ�.\n");
}

void ddelete(dlnode* head, dlnode* removed) {
	if (removed == head) {
		printf("list�� ����ֽ��ϴ�.\n");
		return;
	}
	removed->llink->rlink = removed->rlink;
	removed->rlink->llink = removed->llink;
	free(removed);
	printf("�����߽��ϴ�.\n");
}

void dprint(dlnode* head) {
	dlnode* p;
	for (p = head->rlink; p != head; p = p->rlink) {
		printf("%d", p->data);
		if (p->rlink != head) printf(" , ");
	}
	printf("\n");
}

void reverse_print(dlnode* head) {
	dlnode* p;
	for (p = head->llink; p != head; p = p->llink) {
		printf("%d", p->data);
		if (p->llink != head) printf(" , ");
	}
	printf("\n");
}

int main() {
	dlnode* head = (dlnode*)malloc(sizeof(dlnode));
	dlnode* temp;
	int order;
	int num;
	init(head);

	do {
		printf("�����Ͻ� �� �ִ� ����� 1.\'�߰�\', 2.\'����\', 3.\'���\', 4.\'���� ���\', 5.\'����\' �Դϴ�.: \n");
		scanf("%d", &order);
		switch (order) {
		case 1:
			printf("�߰��ϰ� ���� ������ �Է��ϼ���:");
			scanf("%d", &num);
			printf("\n");
			temp = check_value_insert(head, num);
			if (temp == NULL) continue;
			insert(temp, num);
			break;
		case 2:
			printf("����� ���� ������ �Է��ϼ���:");
			scanf("%d", &num);
			printf("\n");
			temp = check_value_delete(head, num);
			if (temp == NULL) continue;
			ddelete(head, temp);
			break;
		case 3:
			dprint(head);
			break;
		case 4:
			reverse_print(head);
			break;
		case 5:
			break;
		}
	} while (order != 5);

	return 0;
}