#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>

typedef struct {
	char name[15];
}Que;

Que arr[6];
int front = -1;
int back = -1;

void addq(Que qu) {
	back = (back + 1) % 6;
	if (front== back) return;

	arr[back] = qu;
}

Que qpop() {
	if (front == back) return;

	front = (front + 1) % 6;
	return arr[front];
}

int main() {
	Que qu;
	int select=0;
	int select2=0;
	int temp;
	int ans = 0;
	char name[15];

	while (1) {
		printf("입력대기\n");
		printf("1. 병원 도착\n2. 병원 퇴장\n3. 서비스 종료\n");
		scanf("%d", &select);
		if (select == 1) {
			printf("1. 대기자 등록\n2. 취소\n");
			scanf("%d", &select2);
			if (select2 == 1) {
				temp = (back + 1) % 6;
				if (temp == front) {
					printf("Service is not available\n");
					continue;
				}
				printf("이름 입력:");
				scanf("%s", name);
				strcpy(qu.name, name);
				addq(qu);
			}
			else continue;
		}
		else if (select == 2) {
			if (front == back) {
				printf("There is no patient waiting\n");
				continue;
			}
			qu = qpop();
			printf("환자: %s\n", qu.name);
		}
		else break;
	}

	return 0;
}