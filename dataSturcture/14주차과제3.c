#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define M 26
#define MAX_STR 15
#define empty(item) (strlen(item.word)==0)
#define equal(item1, item2) (!strcmp(item1.word, item2.word))

typedef struct bucket {
	char word[MAX_STR];
}bucket;

bucket hash[M];

int hash_function(char *key) {
	int hash_index = transform(key)%M;
	return hash_index;
}

int transform(char word[]) {
	int length=0;
	for (int i = 0; i < strlen(word); i++) {
		length += word[i];
	}
	return length;
}

void init_table(bucket hash[]) {
	for (int i = 0; i < M; i++)
		hash[i].word[0] = NULL;
}

void hash_add(bucket item, bucket hash[]) {
	int i, hash_value;
	hash_value = hash_function(item.word);
	i = hash_function(item.word);
	while (!empty(hash[i])){
		if (equal(item, hash[i])) {
			printf("이미 저장된 단어입니다.\n");
			exit(1);
		}
		i = (i + 1) % M;
		if (i == hash_value) {
			printf("테이블이 가득 차있습니다.");
			exit(1);
		}
	}
	hash[i] = item;
}

void hash_search(bucket item, bucket hash[]) {
	int i, hash_value;
	hash_value = hash_function(item.word);
	i = hash_function(item.word);
	while (!empty(hash[i])) {
		if (equal(hash[i],item)) {
			printf("탐색 %s: 위치 = %d\n", item.word, i);
			return;
		}
		i = (i + 1) % M;
		if (i == hash_value) {
			printf("해당 값은 테이블내에 없습니다.");
			return;
		}
	}
}

void hash_print(bucket hash[]) {
	int i;
	printf("====================================\n");
	for (i = 0; i < M; i++) {
		printf("[%d] %s\n", i, hash[i].word);
	}
	printf("=====================================\n");
}

int main() {
	char strarr[11][11] = { "apple","berry","grape","raisn","banana","watermelon","grapefruit","orange","kiwi","pear","peach" };
	bucket e;

	for (int i = 0; i < 11; i++) {
		strcpy(e.word, strarr[i]);
		hash_add(e, hash);
	}
	hash_print(hash);
	for (int i = 0; i < 11; i++) {
		strcpy(e.word, strarr[i]);
		hash_search(e, hash);
	}
	return 0;
}


