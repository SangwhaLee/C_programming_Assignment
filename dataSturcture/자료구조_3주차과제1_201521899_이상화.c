#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>

typedef struct ListNode {
	int data;
	struct ListNode* link;
}ListNode;

ListNode* insert_first(ListNode* head, int value) {
	ListNode* p = (ListNode*)malloc(sizeof(ListNode));
	p->data = value;
	p->link = head->link;
	head->link = p;
	return head;
}

int check_min(ListNode* head) {
	int min= head->link->data;
	for (ListNode* p = head; p != NULL; p = p->link) {
		if (p->data < min) min = p->data;
	}
	return min;
}

int check_max(ListNode* head) {
	int max = head->link->data;
	for (ListNode* p = head; p != NULL; p = p->link) {
		if (p->data > max) max = p->data;
	}
	return max;
}

int main() {
	ListNode* head = (ListNode*)malloc(sizeof(ListNode));
	int test;
	int num;
	int i;
	int min, max;

	scanf("%d", &test);
	printf("노드 1의 데이터: ");
	scanf("%d", &num);
	head->data = num;
	head->link = NULL;
	for (i = 1; i < test; i++) {
		printf("\n노드 %d의 데이터:", i + 1);
		scanf("%d", &num);
		insert_first(head, num);
	}
	min = check_min(head);
	max = check_max(head);
	
	printf("max= %d, min= %d", max, min);

	free(head);

	return 0;
}