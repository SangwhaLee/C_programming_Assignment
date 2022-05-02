#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>

typedef struct Tnode {
	int data;
	struct Tnode *left, * right;
}Tnode;

Tnode* root;

void make_root(int data) {
	Tnode* temp;
	temp = (Tnode*)malloc(sizeof(Tnode));
	temp->data = data;
	root = temp;
	root->data = data;
	root->left = NULL;
	root->right = NULL;
}

Tnode* addchild(Tnode* parent, int data) {
	if ((parent->left != NULL && parent->right != NULL)) {
		printf("�� ���� �̹� ���� ���ֽ��ϴ�.\n");
		return 0;
	}
	Tnode* temp;
	temp = (Tnode*)malloc(sizeof(Tnode));
	temp->left = NULL;
	temp->right = NULL;
	temp->data = data;
	if (parent->left == NULL)
		parent->left = temp;
	else if (parent->right == NULL)
		parent->right = temp;
	return temp;
}

int preorder(Tnode* root, int num) {
	int check1 = 0;
	int check2 = 0;
	int check3 = 0;
	int result = 0;
	if (root != NULL) {
		if (root->data < num) {
			printf("%d���� ���� ���: %d\n",num, root->data);
			check1 = 1;
		}
		check2= preorder(root->left, num);
		check3= preorder(root->right, num);
	}
	result = check1 + check2 + check3;
	return result;
}

int main() {
	int data;
	int num;
	int check;
	printf("��Ʈ�� �� ���� �Է�:");
	scanf("%d", &data);
	make_root(data);
	printf("\n���� 2�� ��忡 �� ���� �Է�:");
	scanf("%d", &data);
	Tnode* L = addchild(root, data);
	scanf("%d", &data);
	Tnode* R = addchild(root, data);
	printf("\n���� 3�� ��忡 �� ���� �Է�:");
	scanf("%d", &data);
	Tnode* LL = addchild(L, data);
	scanf("%d", &data);
	Tnode* LR = addchild(L, data);
	scanf("%d", &data);
	Tnode* RL = addchild(R, data);
	scanf("%d", &data);
	addchild(R, data);
	printf("\n���� 4�� ��忡 �� ���� �Է�:");
	for (int i = 0; i < 2; i++) {
		scanf("%d", &data);
		addchild(LL, data);
	}
	for (int i = 0; i < 2; i++) {
		scanf("%d", &data);
		addchild(LR, data);
	}
	for (int i = 0; i < 2; i++) {
		scanf("%d", &data);
		addchild(RL, data);
	}

	printf("\n���ذ��� �Է��Ͻÿ�:");
	scanf("%d", &num);
	printf("\n");

	check = preorder(root, num);

	if (check == 0) printf("�Է��� %d���� ���� ���� ���� ���� �����ϴ�.", num);

	return 0;
}