#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

#define BUCKETS 10
#define MAX_QUE 50

typedef struct Que {
	int data[MAX_QUE];
	int front, rear;
}Que;

void init_que(Que* q) {
	q->front = q->rear = 0;
}

int is_empty(Que* q) {
	return (q->front == q->rear);
}

int is_full(Que* q) {
	return ((q->rear + 1) % MAX_QUE == q->front);
}

void enqueue(Que* q, int item) {
	if (is_full(q)) {
		printf("큐가 가득 차있습니다.");
		exit(1);
	}
	q->rear = (q->rear + 1) % MAX_QUE;
	q->data[q->rear] = item;
}

int dequeue(Que* q) {
	if (is_empty(q)) {
		printf("큐가 비어있습니다.");
		exit(1);
	}
	q->front = (q->front + 1) % MAX_QUE;
	return q->data[q->front];
}

void radix_sort(int list[], int n, int digits) {
	int i,j,k;
	Que que[BUCKETS];
	int phase = 1;
	
	for (i = 0; i < BUCKETS; i++)
		init_que(&que[i]);

	for (j = 0; j < digits; j++) {
		for (i = 0; i < n; i++) {
			enqueue(&que[(list[i] / phase) % 10], list[i]);
		}
		for (k = i = 0; k < BUCKETS; k++) {
			while (!is_empty(&que[k]))
				list[i++] = dequeue(&que[k]);
		}
		phase *= 10;
	}
}

int find_digit(int arr[], int length) {
	int max = -1;
	int digit = 0;
	for (int i = 0; i < length; i++) {
		if (arr[i] > max)
			max = arr[i];
	}
	do {
		max /= 10;
		digit++;
	} while (max != 0);

	return digit;
}

int main() {
	int arr[15] = { 1,300,200,104,101,91,88,60,15,31,12,26,82,51,6000 };
	int length = sizeof(arr) / sizeof(int);

	int digit = find_digit(arr, length);

	printf("정렬하기 전의 데이터: ");
	for (int i = 0; i < length; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n\n");

	radix_sort(arr, length, digit);

	printf("Radix Sort: ");
	for (int i = 0; i < length; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");

	return 0;
}