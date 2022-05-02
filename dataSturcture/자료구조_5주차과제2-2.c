#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>

typedef struct Tnode {
	int left_thread;
	int right_thread;
	int data;
	struct Tnode* left, * right;
}Tnode;


Tnode* insucc(Tnode* tree) {
	Tnode* temp;
	temp = tree->right;
	if (!tree->right_thread) {
		while (!temp->left_thread)
			temp = temp->left;
	}
	return temp;
}

void tinorder(Tnode* tree) {
	Tnode* temp = tree;
	for (;;) {
		temp = insucc(temp);
		if (temp == tree) break;
		printf("%3d", temp->data);
	}
}

void insertright(Tnode* s, Tnode* r) {
	Tnode* temp;
	r->right = s->right;
	r->right_thread = s->right_thread;
	r->left = s;
	r->left_thread = 1;
	s->right = r;
	s->right_thread = 0;
	if (!r->right_thread) {
		temp = insucc(r);
		temp->left = r;
	}
}

void insertleft(Tnode* s, Tnode* l) {
	Tnode* temp;
	l->left = s->left;
	l->left_thread = s->left_thread;
	l->right = s;
	l->right_thread = 1;
	s->left = l;
	s->left_thread = 0;
	if (!l->left_thread) {
		temp = insucc(l);
		temp->left = l;
	}
}

Tnode* init(void) {
	Tnode* root = (Tnode*)malloc(sizeof(Tnode));

	root->data = NULL;
	root->left = NULL;
	root->right = NULL;
	root->left_thread = 0;
	root->right_thread = 0;

	return root;
}

Tnode* make_node(int data) {
	Tnode* temp = (Tnode*)malloc(sizeof(Tnode));
	temp->data = data;
	return temp;
}

int main() {
	int num;
	Tnode* root = init();
	Tnode* tree = (Tnode*)malloc(sizeof(Tnode));
	printf("루트에 들어갈 값:");
	scanf("%d", &num);
	tree->data = num;
	tree->left = root;
	tree->right = root;
	tree->left_thread = 1;
	tree->right_thread = 1;
	root->left = tree;
	root->right = root;
	printf("\n레벨 2의 노드에 들어갈 값을 입력하시오:");
	scanf("%d", &num);
	Tnode* L = make_node(num);
	insertleft(tree, L);
	scanf("%d", &num);
	Tnode* R = make_node(num);
	insertright(tree, R);
	printf("\n레벨 3의 노드에 들어갈 값을 입력하시오:");
	scanf("%d", &num);
	Tnode* LL = make_node(num);
	insertleft(L, LL);
	scanf("%d", &num);
	Tnode* LR = make_node(num);
	insertright(L, LR);
	scanf("%d", &num);
	Tnode* RL = make_node(num);
	insertleft(R, RL);
	scanf("%d", &num);
	Tnode* RR = make_node(num);
	insertright(R, RR);
	printf("\n레벨 4의 노드에 들어갈 값을 입력하시오:");
	scanf("%d", &num);
	Tnode* LLL = make_node(num);
	insertleft(LL, LLL);
	scanf("%d", &num);
	Tnode* LLR = make_node(num);
	insertright(LL, LLR);
	scanf("%d", &num);
	Tnode* LRL = make_node(num);
	insertleft(LR, LRL);
	scanf("%d", &num);
	Tnode* LRR = make_node(num);
	insertright(LR, LRR);
	scanf("%d", &num);
	Tnode* RLL = make_node(num);
	insertleft(RL, RLL);
	scanf("%d", &num);
	Tnode* RLR = make_node(num);
	insertright(RL, RLR);
	printf("\n");

	tinorder(root);

	return 0;
}