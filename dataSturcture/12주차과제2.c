#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

void insertion_sort(int arr[], int length) {
	int i, j, key;

	printf("정렬 전의 목록: ");
	for (i = 0; i < length; i++)
		printf("%d ", arr[i]);
	printf("\n");

	for (i = 1; i < length; i++) {
		key = arr[i];
		for (j = i - 1; j >= 0 && arr[j] > key; j--) {
			arr[j + 1] = arr[j];
		}
		arr[j + 1] = key;
	}

	printf("Insertion sort: ");
	for (i = 0; i <length; i++)
		printf("%d ", arr[i]);
	printf("\n");
}

int main() {
	int numarr[15] = { 1,300,200,104,101,91,88,60,15,31,12,26,82,51,6 };
	int length = sizeof(numarr) / sizeof(int);

	insertion_sort(numarr, length);
	
	return 0;

}