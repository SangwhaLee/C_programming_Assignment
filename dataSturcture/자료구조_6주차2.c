#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct {
	char name[11];
	char date[9];
	char area[21];
}element;

typedef struct Tnode {
	element key;
	struct Tnode* left, *right;
}Tnode;

int compare(element e1, element e2) {
	return strcmp(e1.name, e2.name);
}

Tnode* search(Tnode* root, element key) {
	Tnode* temp = root;
	while (temp != NULL) {
		if (compare(key, temp->key) == 0)
			return temp;
		else if (compare(key, temp->key) < 0)
			temp = temp->left;
		else if (compare(key, temp->key) > 0)
			temp = temp->right;
	}
	return temp;
}

Tnode* make_node(element item) {
	Tnode* temp = (Tnode*)malloc(sizeof(Tnode));
	temp->key = item;
	temp->left = temp->right = NULL;
	return temp;
}

Tnode* insert(Tnode* root, element item) {
	if (root == NULL) return make_node(item);

	if (compare(item, root->key) < 0)
		root->left = insert(root->left, item);
	else if (compare(item, root->key) > 0)
		root->right = insert(root->right, item);
	return root;
}

Tnode* find_min(Tnode* root) {
	Tnode* current = root;
	while (current->left != NULL)
		current = current->left;
	return current;
}

Tnode* delete_node(Tnode* root, element key) {
	if (root == NULL) return root;

	if (compare(key, root->key) < 0)
		root->left = delete_node(root->left, key);
	if (compare(key, root->key) > 0)
		root->right = delete_node(root->right, key);
	else {
		if (root->left == NULL) {
			Tnode* temp = root->right;
			free(root);
			return temp;
		}
		else if (root->right == NULL) {
			Tnode* temp = root->left;
			free(root);
			return temp;
		}
		Tnode* temp = find_min(root->right);
		root->key = temp->key;
		root->right = delete_node(root->right, temp->key);
	}
	return root;
}

void print_search(Tnode* root, element key) {
	Tnode* temp = root;
	temp = search(temp, key);
	if (temp == NULL) {
		printf("�ش�Ǵ� ����� �����ϴ�.\n");
		return;
	}
	else {
		printf("�̸�: %s\n", temp->key.name);
		printf("��û��¥: %s\n", temp->key.date);
		printf("��û����: %s\n", temp->key.area);
	}
}

void change_search(Tnode* root, element key) {
	Tnode* temp = root;
	temp = search(temp, key);
	if (temp == NULL) {
		printf("�ش�Ǵ� ����� �����ϴ�.\n");
		return;
	}
	temp->key = key;
	print_search(root, key);
}


int main() {
	int order;
	element e;
	Tnode* root = NULL;
	Tnode* temp;
	char tp_arr[21] = { 0 };
	printf("������ ���� ���� ���α׷�\n\n");


	do {
		printf("�Է�(1), �˻�(2), ��������(3), �������(4), ����(5)\n");
		scanf("%d", &order);
		getchar();
		switch (order) {
		case 1:
			printf("������ �Է��Ͻʽÿ�\n");
			printf("�̸�(���� 10���̳�)\n");
			gets(e.name);
			printf("��û��¥(YYYYMMDD)\n");
			gets(e.date);
			printf("��û����(���� 20���̳�)\n");
			gets(e.area);
			root = insert(root, e);
			break;

		case 2:
			printf("�˻��� �̸��� �Է��Ͻʽÿ�.");
			gets(e.name);
			print_search(root, e);
			break;

		case 3:
			printf("������ ������ �Է��Ͻʽÿ�.(�̸�, ����)\n");
			gets(e.name);
			temp = search(root, e);
			e = temp->key;
			gets(tp_arr);
			if (tp_arr[0] > 'a' && tp_arr[0] < 'z' || tp_arr[0]>'A' && tp_arr[0] < 'Z') {
				strcpy(e.area, tp_arr);
			}
			else strcpy(e.date, tp_arr);
			change_search(root, e);
			break;

		case 4:
			printf("����� �̸��� �Է��Ͻʽÿ�.\n");
			gets(e.name);
			root = delete_node(root, e);
			printf("������ ��ҵǾ����ϴ�.\n");
			break;

		case 5:
			break;
		}
	} while (order != 5);
	printf("�����մϴ�.\n");
	return 0;
}