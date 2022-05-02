#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

int partition(int list[], int left, int right) {
	int pivot;
	int low, high, temp;

	low = left;
	high = right+1;
	pivot = list[left];

	do {
		do {
			low++;
		} while (list[low] < pivot);
		do {
			high--;
		} while (list[high] > pivot);
		if (low < high) {
			temp = list[low];
			list[low] = list[high];
			list[high] = temp;
		}
	} while (low < high);

	temp = list[left];
	list[left] = list[high];
	list[high] = temp;

	return high;
}

void quick_sort(int list[], int left, int right) {
	if (left < right) {
		int pivot = partition(list, left, right);
		quick_sort(list, left, pivot - 1);
		quick_sort(list, pivot + 1, right);
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

	quick_sort(arr, 0, length - 1);

	printf("Quick Sort: ");
	for (int i = 0; i < length; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");

	return 0;
}
