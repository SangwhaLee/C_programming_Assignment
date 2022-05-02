#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_ELEMENTS 11
#define HEAP_FULL(n) (n==MAX_ELEMENTS-1)
#define HEAP_EMPTY(n) (!n)
typedef struct element {
	char name[20];
	int time;
}element;
element heap_arr[MAX_ELEMENTS];
int heap_size=0;


void push(int *heap_size, element item) {
	int i;
	if (HEAP_FULL(*heap_size)) {
		printf("The heap is full");
		exit(1);
	}
	i = ++(*heap_size);
	while ((i != 1) && (item.time > heap_arr[i / 2].time)) {
		heap_arr[i] = heap_arr[i / 2];
		i /= 2;
	}
	heap_arr[i] = item;
}

element pop(int* heap_size, char *str) {
	int parent, child;
	int i = 1;
	element item, temp;
	if (HEAP_EMPTY(*heap_size)) {
		printf("The heap is empty");
		exit(1);
	}
	while (i != (*heap_size) + 1) {
		if (!strcmp(heap_arr[i].name, str)) {
			break;
		}
		i++;
	}
	item = heap_arr[i];
	temp = heap_arr[(*heap_size)--];
	parent = i;
	child = i+1;

	while (child <= *heap_size) {
		if ((child <= *heap_size) && (heap_arr[child].time < heap_arr[child + 1].time))
			child++;
		if (temp.time >= heap_arr[child].time) break;
		heap_arr[parent] = heap_arr[child];
		parent = child;
		child *= 2;
	}
	heap_arr[parent] = temp;
	return item;
}

void heap_print(int* heap_size) {
	int i = 1;
	int j;
	element temp;
	for (i = 1; i < (*heap_size) ; i++) {
		for (j = i; j <= (*heap_size); j++) {
			if (heap_arr[i].time < heap_arr[j].time) {
				temp = heap_arr[i];
				heap_arr[i] = heap_arr[j];
				heap_arr[j] = temp;
			}
		}
	}
	i = 1;
	while (i != (*heap_size) + 1) {
		printf("%s-%d분\n", heap_arr[i].name, heap_arr[i].time);
		i++;
	}
}

int main() {
	int choice;
	element temp;
	char name[11];

	do {
		printf("삽입(1), 삭제(2), 출력(3), 종료(4):");
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			printf("\n해야할 일:");
			scanf("%s", temp.name);
			printf("\n소요 시간(단위:분):");
			scanf("%d", &temp.time);
			push(&heap_size, temp);
			break;

		case 2:
			printf("\n삭제할 일:");
			scanf("%s", name);
			pop(&heap_size, name);
			printf("\n%s가 삭제됨\n", name);
			break;

		case 3:
			heap_print(&heap_size);
			break;

		case 4:
			printf("프로그램을 종료합니다.");
			break;
		}
	} while (choice != 4);
	return 0;
}