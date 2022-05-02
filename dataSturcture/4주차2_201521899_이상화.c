#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#define MAX_SIZE 50

typedef struct Tnode {
	int data;
	struct Tnode* left, * right;
}Tnode;

typedef struct que {
	struct Tnode* data[MAX_SIZE];
	int front, rear;
}que;

void error(char* message) {
	printf("%s\n", message);
	exit(1);
}

void init_que(que* ptr) {
	ptr->front = ptr->rear = 0;
}

int que_empty(que* ptr) {
	return(ptr->front == ptr->rear);
}

int que_full(que* ptr) {
	return ((ptr->rear + 1) % MAX_SIZE == ptr->front);
}

void enque(que* ptr, Tnode* node) {
	if (que_full(ptr)) {
		error("큐가 포화상태입니다.");
	}
	ptr->rear = (ptr->rear + 1) % MAX_SIZE;
	ptr->data[ptr->rear] = node;
}

Tnode* deque(que* ptr) {
	if (que_empty(ptr)) {
		error("큐가 비어있습니다.");
	}
	ptr->front = ((ptr->front + 1) % MAX_SIZE);
	return ptr->data[ptr->front];
}

void level_order(Tnode* root){
	que q;

	init_que(&q);

	if (root == NULL) return;
	enque(&q, root);
	while (!que_empty(&q)) {
		root = deque(&q);
		printf(" [%d] ", root->data);
		if (root->left)
			enque(&q, root->left);
		if (root->right)
			enque(&q, root->right);
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

	printf("루트에 들어갈 값:");
	scanf("%d", &num);
	make_root(num);
	printf("레벨2의 노드에 들어갈 값:");
	scanf("%d", &num);
	Tnode* L = addchild(root, num);
	scanf("%d", &num);
	Tnode* R = addchild(root, num);
	printf("레벨3의 노드에 들어갈 값:");
	scanf("%d", &num);
	Tnode* LL = addchild(L, num);
	scanf("%d", &num);
	Tnode* LR= addchild(L, num);
	scanf("%d", &num);
	Tnode* RL = addchild(R, num);
	scanf("%d", &num);
	addchild(R, num);
	printf("레벨4의 노드에 들어갈 값:");
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
	printf("Level order traversal의 결과:");
	level_order(root);
	return 0;
}