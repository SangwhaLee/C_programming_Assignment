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
		printf("이 노드는 이미 가득 차있습니다.\n");
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

	printf("루트에 들어갈 값을 입력:");
	scanf("%d", &num);
	make_root(num);
	printf("레벨 2의 노들에 들어갈 값을 입력:");
	scanf("%d", &num);
	Tnode* L = addchild(root, num);
	scanf("%d", &num);
	Tnode* R = addchild(root, num);
	printf("레벨 3의 노들에 들어갈 값을 입력:");
	scanf("%d", &num);
	Tnode* LL = addchild(L, num);
	scanf("%d", &num);
	Tnode* LR = addchild(L, num);
	scanf("%d", &num);
	Tnode* RL = addchild(R, num);
	scanf("%d", &num);
	addchild(R, num);
	printf("레벨 4의 노들에 들어갈 값을 입력:");
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

	printf("Inorder traversal의 결과:");
	inorder(root);
	printf("\n");
	printf("Preorder traversal의 결과:");
	preorder(root);
	printf("\n");
	printf("Postorder traversal의 결과:");
	postorder(root);
	printf("\n");

	return 0;
}
