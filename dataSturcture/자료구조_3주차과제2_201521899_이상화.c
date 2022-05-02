#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#define FALSE 0
#define TRUE 1

typedef struct polyNode *polypointer;

typedef struct polyNode {
	int cof;
	int exp;
	polypointer link;
}polyNode;

polypointer avail;

polypointer get_head() {
	polypointer head;
	head = (polypointer)malloc(sizeof(polyNode));
	head->cof = 0;
	head->exp = -1;
	head->link = NULL;

	return head;
}

polypointer get_node() {
	polypointer node;

	if (avail) {
		node = avail;
		avail = avail->link;
	}
	else
		node = (polypointer)malloc(sizeof(polyNode));

	return node;
}

void insert(polypointer head, int cof, int exp) {
	polypointer p, temp;
	p = get_node();
	p->cof = cof;
	p->exp = exp;
	if (head->link == NULL) {
		head->link = p;
		p->link = head;
	}
	else {
		temp = head->link;
		while (temp->link != head)
			temp = temp->link;
		p->link = head;
		temp->link = p;
	}
}

void attach(polypointer* ptr,int cof, int exp) {
	polypointer temp;
	temp = get_node();
	temp->cof = cof;
	temp->exp = exp;
	(*ptr)->link = temp;
	*ptr = temp;
}


int COMPARE(int a, int b) {
	if (a > b) return-1;
	else if (a == b) return 0;
	else return 1;
}

polypointer sub_poly(polypointer head1, polypointer head2) {
	polypointer startA,c, lastC;
	int sum;
	int done = FALSE;
	startA = head1;
	head1 = head1->link;
	head2 = head2->link;
	c = get_node();
	c->exp = -1;
	lastC = c;

	do {
		switch (COMPARE(head1->exp, head2->exp)) {
		case -1:
			attach(&lastC, head1->cof, head1->exp);
			head1 = head1->link;
			break;
		case 0:
			if (startA == head1) done = TRUE;
			else {
				sum = head1->cof - head2->cof;
				if (sum != 0) attach(&lastC, sum, head1->exp);
				head1 = head1->link;
				head2 = head2->link;
			}
			break;
		case 1:
			attach(&lastC, -(head2->cof), head2->exp);
			head2 = head2->link;
		}
	} while (!done);
	lastC->link = c;
	return c;
}

void print_poly(polypointer head) {
	polypointer temp;

	temp = head->link;
	while (temp->link != head->link) {
		printf("%d^%d", temp->cof, temp->exp);
		temp = temp->link;
		if (temp->link != head->link)
			printf(" + ");
	}
}

int main() {
	polypointer a, b, c;
	avail = get_head();
	a = get_head();
	b = get_head();
	int checka = 1, checkb = 1;
	int coef, expon;

	do {
		printf("�Է��ϰ��� �ϴ� ù ���׽��� ���� ����� ������ ���ʴ�� �Է��ϼ���: ");
		scanf("%d %d", &coef, &expon);
		printf("\n");
		insert(a, coef, expon);
		printf("�� �Է��Ͻðڽ��ϱ�? ������ 1 �ƴϸ� 0�� �Է��ϼ���: ");
		scanf("%d", &checka);
		printf("\n");
	} while (checka == 1);

	do {
		printf("�Է��ϰ��� �ϴ� �ι�° ���׽��� ���� ����� ������ ���ʴ�� �Է��ϼ���: ");
		scanf("%d %d", &coef, &expon);
		printf("\n");
		insert(b, coef, expon);
		printf("�� �Է��Ͻðڽ��ϱ�? ������ 1 �ƴϸ� 0�� �Է��ϼ���: ");
		scanf("%d", &checkb);
		printf("\n");
	} while (checkb == 1);

	c= sub_poly(a, b);

	printf("1��° ���׽�: ");
	print_poly(a);

	printf("\n2��° ���׽�: ");
	print_poly(b);

	printf("\n��� ���: ");
	print_poly(c);

	
	return 0;
}