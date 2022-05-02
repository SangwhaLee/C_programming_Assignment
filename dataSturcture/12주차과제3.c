#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

void bubblesort(int arr[], int n) {
	int i, j, temp;
	
	printf("정렬 전의 목록: ");
	for (i = 0; i < n; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");

	for (i = 0; i < n - 1; i++) {
		for (j = 0; j < n - 1 - i; j++) {
			if (arr[j + 1] < arr[j]) {
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}

	printf("Bubble sort: ");
	for (i = 0; i < n; i++)
		printf("%d ", arr[i]);
	printf("\n");
}

int main() {
	int numarr[15] = { 1, 300, 200, 104, 101, 91, 88, 60, 15, 31, 12, 26, 82, 51, 6 };
	int length = sizeof(numarr) / sizeof(int);

	bubblesort(numarr, length);

	return 0;
}