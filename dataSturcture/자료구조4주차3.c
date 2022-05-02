#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

typedef struct Tnode {
	int data;
	struct Tnode* left, * right;
}Tnode;

int calcul(Tnode* root) {
	int left_file, right_file;
	if (root == NULL) return 0;

	left_file = calcul(root->left);
	right_file = calcul(root->right);
	return (left_file + right_file + root->data);
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
	int total;
	int C;
	int D;
	
	printf("��Ʈ�� ���� ��:");
	scanf("%d", &num);
	make_root(num);
	printf("\n����2�� ��Ʈ�� ���� ��:");
	scanf("%d", &num);
	Tnode* L = addchild(root, num);
	scanf("%d", &num);
	Tnode* R = addchild(root, num);
	printf("\n���� 3�� ��忡 ���� ��:");
	scanf("%d", &num);
	Tnode* LL = addchild(L, num);
	scanf("%d", &num);
	addchild(L, num);
	scanf("%d", &num);
	Tnode* RL = addchild(R, num);
	scanf("%d", &num);
	Tnode* RR = addchild(R, num);
	printf("\n���� 4�� ��忡 ���� ��:");
	scanf("%d", &num);
	addchild(LL, num);
	scanf("%d", &num);
	addchild(LL, num);
	scanf("%d", &num);
	addchild(RL, num);
	scanf("%d", &num);
	addchild(RL, num);
	scanf("%d", &num);
	addchild(RR, num);
	scanf("%d", &num);
	addchild(RR, num);

	C = calcul(root->left);
	D = calcul(root->right);
	total = calcul(root);

	printf("\nC:�� �뷮:%dM�Դϴ�.\n", C);
	printf("D:�� �뷮:%dM�Դϴ�.\n", D);
	printf("����ǻ���� ��ü�뷮: %dM�Դϴ�.\n", total);

	return 0;
}