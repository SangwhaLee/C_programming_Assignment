#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>

#define MAX_SIZE 51;
#define TRUE 1
#define FALSE 0;

typedef struct _stack {
	int arr[MAX_SIZE];
	int top;
} Stack;

void Stackinit(Stack* st) {
	st->top = -1;
}

void push(char key, Stack* st) {
	if (st->top>=50) return;

	st->arr[++(st->top)] = key;
}

int pop(Stack* st) {
	if (st->top == -1) return -1;

	return st->arr[st->top--];
}


int main() {
	Stack st;
	char key[51];
	int back;
	int ans = -1;
	int i = 0;

	Stackinit(&st);

	printf("input string (q입력시 종료)\n");
	
	do {
		scanf("%c", &key[i]);
	} while (key[i++] != 'q');

	i = 0;
	while (key[i] != 'q') {
		if (key[i] == '(' || key[i] == '{' || key[i] == '[') {
			push(key[i],&st);
		}
		else {
			back = pop(&st);
			if (key[i] == ')') {
				if (back == '(') ans = 1;
				else {
					ans = 0;
					printf("')'가 문제입니다.\n");
					break;
				}
			}
			else if (key[i] == '}') {
				if (back == '{') ans = 1;
				else {
					ans = 0;
					printf("'}'가 문제입니다.\n");
					break;
				}
			}
			else {
				if (back == '[') ans = 1;
				else {
					ans = 0;
					printf("']'가 문제입니다.\n");
					break;
				}
			}
		}
		i++;
	}

	if (ans == -1) {
		printf("q\n");
	}
	else if (ans == 0) {
		printf("This string is unbalanced");
	}
	else {
		printf("This string is balanced");
	}

	return 0;
}



