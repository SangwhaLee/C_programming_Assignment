#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

#define MAX_SIZE 50

int sort[MAX_SIZE];

void merge(int list[], int left, int mid, int right) {
	int i, j,k,l;
	i = left, j = mid + 1, k=left;

	while (i <= mid && j <= right) {
		if (list[i] <= list[j])
			sort[k++] = list[i++];
		else
			sort[k++] = list[j++];
	}
	if (i > mid) {
		for (l = j; l <= right; l++) {
			sort[k++] = list[l];
		}
	}
	else
		for (l = i; l <= mid; l++) {
			sort[k++] = list[l];
		}
	for (l = left; l <= right; l++)
		list[l] = sort[l];
}

void merge_sort(int list[], int left, int right) {
	int mid;
	if (left < right) {
		mid = (left + right) / 2;
		merge_sort(list, left, mid);
		merge_sort(list, mid+1, right);
		merge(list, left, mid, right);
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

	merge_sort(arr, 0, length - 1);

	printf("Merge Sort: ");
	for (int i = 0; i < length; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");

	return 0;
}