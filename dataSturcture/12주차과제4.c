#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

void Selection(int arr[], int n) {
	int i, j, min, temp;

	printf("정렬 전의 목록: ");
	for (i = 0; i < n; i++)
		printf("%d ", arr[i]);
	printf("\n");

	for (i = 0; i < n - 1; i++) {
		min = i;
		for (j = i+1; j < n; j++) {
			if (arr[j] < arr[min])
				min = j;
		}
		temp = arr[i];
		arr[i] = arr[min];
		arr[min] = temp;
	}

	printf("Selection sort: ");
	for (i = 0; i < n; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

int main() {
	int numarr[15] = { 1, 300, 200, 104, 101, 91, 88, 60, 15, 31, 12, 26, 82, 51, 6 };
	int length = sizeof(numarr) / sizeof(int);

	Selection(numarr, length);

	return 0;
}