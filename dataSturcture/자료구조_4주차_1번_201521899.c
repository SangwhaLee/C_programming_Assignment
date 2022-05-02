#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>

typedef struct Tnode {
	int data;
	struct Tnode* left, * right;
}Tnode;


void inorder(Tnode* root) {
	if (root != NULL) {
		inorder(root->left);
		printf("[%d] ", root->data);
		inorder(root->right);
	}
}

void preorder(Tnode* root) {
	if (root != NULL) {
		printf("[%d] ", root->data);
		preorder(root->left);
		preorder(root->right);
	}
}

void postorder(Tnode* root) {
	if (root != NULL) {
		postorder(root->left);
		postorder(root->right);
		printf("[%d] ", root->data);
	}
}

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


int main() {
	int num;

	printf("��Ʈ�� �� ���� �Է�:");
	scanf("%d", &num);
	make_root(num);
	printf("���� 2�� ��鿡 �� ���� �Է�:");
	scanf("%d", &num);
	Tnode* L = addchild(root, num);
	scanf("%d", &num);
	Tnode* R = addchild(root, num);
	printf("���� 3�� ��鿡 �� ���� �Է�:");
	scanf("%d", &num);
	Tnode* LL = addchild(L, num);
	scanf("%d", &num);
	Tnode* LR = addchild(L, num);
	scanf("%d", &num);
	Tnode* RL = addchild(R, num);
	scanf("%d", &num);
	addchild(R, num);
	printf("���� 4�� ��鿡 �� ���� �Է�:");
	scanf("%d", &num);
	addchild(LL, num);
	scanf("%d", &num);
	addchild(LL, num);
	scanf("%d", &num);
	addchild(LR, num);
	scanf("%d", &num);
	addchild(LR, num);
	scanf("%d", &num);
	addchild(RL, num);
	scanf("%d", &num);
	addchild(RL, num);

	printf("Inorder traversal�� ���:");
	inorder(root);
	printf("\n");
	printf("Preorder traversal�� ���:");
	preorder(root);
	printf("\n");
	printf("Postorder traversal�� ���:");
	postorder(root);
	printf("\n");

	return 0;
}
