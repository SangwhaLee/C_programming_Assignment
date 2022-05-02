#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#define MAX_VERTEX 10
#define MAX_QUE 7

int visited[MAX_VERTEX];

typedef struct Gnode {
	int vertex;
	struct Gnode* link;
}Gnode;

typedef struct Gtype {
	int n;
	Gnode* adj_list[MAX_VERTEX];
}Gtype;

typedef struct Que{
	int queue[MAX_QUE];
	int front, rear;
}Que;

void queue_init(Que* q) {
	q->front = q->rear = 0;
}

int is_empty(Que* q) {
	return (q->front == q->rear);
}

int is_full(Que* q) {
	return ((q->rear + 1) % MAX_QUE == q->front);
}

void enqueue(Que* q, int vertex) {
	if (is_full(q)) {
		printf("ť�� ���ڸ��� �����ϴ�");
		exit(1);
	}
	q->rear = (q->rear + 1) % MAX_QUE;
	q->queue[q->rear] = vertex;
}

int dequeue(Que* q) {
	if (is_full(q)) {
		printf("ť�� ����ֽ��ϴ�");
		exit(1);
	}
	q->front = (q->front + 1) % MAX_QUE;
	return q->queue[q->front];
}

void graph_init(Gtype* g) {
	g->n = 0;
	for (int i = 0; i < MAX_VERTEX; i++) {
		g->adj_list[i] = NULL;
	}
}

void insert_vertex(Gtype* g, int vertex) {
	if (((g->n) + 1) > MAX_VERTEX) {
		printf("�׷����� ���� ���� �ʰ�");
		return;
	}
	g->n++;
}

void insert_edge(Gtype* g, int u, int v) {
	Gnode* node;
	if (u >= g->n || v >= g->n) {
		printf("���� ��ȣ ����: ������ ������ϴ�.");
		return;
	}
	node = (Gnode*)malloc(sizeof(Gnode));
	node->vertex = v;
	node->link = g->adj_list[u];
	g->adj_list[u] = node;
}

void bfs_list(Gtype* g, int vertex) {
	Gnode* w;
	Que q;

	queue_init(&q);
	visited[vertex] = 1;
	printf("%c ->", vertex+'A');
	enqueue(&q, vertex);
	while (!is_empty(&q)) {
		vertex = dequeue(&q);
		for (w = g->adj_list[vertex]; w; w=w->link) {
			if (!visited[w->vertex]) {
				visited[w->vertex] = 1;
				printf("%c ->", (w->vertex)+'A');
				enqueue(&q, w->vertex);
			}
		}
	}
}

int main() {
	char u, v;
	int select;
	Gtype* g = (Gtype*)malloc(sizeof(Gtype));
	graph_init(g);
	for (int i = 0; i < MAX_QUE; i++) {
		insert_vertex(g, i);
	}
	do {
		printf("������� �ϴ� ������ �糡 ������ ���ʷ� �Է����ּ���:");
		scanf("%c", &u);
		getchar();
		printf("%c", u);
		scanf("%c", &v);
		getchar();
		printf("%c", v);
		printf("\n");
		insert_edge(g, u-'A', v-'A');

		printf("�� �Է��Ͻðڽ��ϱ�?(1.�߰��Է�, 2.���� �� ���):");
		scanf("%d", &select);
		getchar();
		printf("\n");
	} while (select != 2);

	printf("Breadth First Search�� ���:");
	bfs_list(g, 0);
	printf("\n");
	return 0;
}
