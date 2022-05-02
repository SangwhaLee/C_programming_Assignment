#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

#define MAX_SIZE 100
typedef struct element {
	int key;
}element;

element heap[MAX_SIZE];

void adjust(element heap[], int root, int n) {
	int child, rootkey;
	element temp;
	temp = heap[root];
	rootkey = heap[root].key;
	child = 2 * root;
	while (child <= n) {
		if (child < n && heap[child].key < heap[child + 1].key)
			child++;
		if (rootkey > heap[child].key)
			break;
		else {
			heap[child / 2] = heap[child];
			child *= 2;
		}
	}
	heap[child / 2] = temp;
}

void heap_sort(element heap[], int n) {
	int i, j;
	element temp;

	for (i = n / 2; i > 0; i--)
		adjust(heap, i, n);
	for (i = n-1 ; i > 0; i--) {
		temp = heap[1];
		heap[1] = heap[i + 1];
		heap[i + 1] = temp;
		adjust(heap, 1, i);
	}
}

void listtoheap(int list[], int n) {
	for (int i = 0; i < n; i++) {
		heap[i + 1].key = list[i];
	}
}

void heaptolist(int list[], int n) {
	for (int i = 1; i < n + 1; i++) {
		list[i - 1] = heap[i].key;
	}
}

int main() {
	int arr[15] = { 1,300,200,104,101,91,88,60,15,31,12,26,82,51,6 };
	int length = sizeof(arr) / sizeof(int);
	
	printf("정렬하기 전의 데이터: ");
	for (int i = 0; i < length; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n\n");

	listtoheap(arr, length );
	heap_sort(heap, length);
	heaptolist(arr, length );

	printf("Heap Sort: ");
	for (int i = 0; i < length; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n\n");

	return 0;
}