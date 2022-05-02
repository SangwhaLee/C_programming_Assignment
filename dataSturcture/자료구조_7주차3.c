#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

#define MAX_VERTEX 7
int visited[MAX_VERTEX];
typedef struct Gnode {
	char vertex;
	struct Gnode* link;
}Gnode;

typedef struct Gtype {
	int n;
	Gnode* adj_list[MAX_VERTEX];
}Gtype;

void init(Gtype* g) {
	int v;
	g->n = 0;
	for (v = 0; v < MAX_VERTEX; v++)
		g->adj_list[v] = NULL;
}

void insert_vertex(Gtype* g, int v) {
	if ((g->n) + 1 > MAX_VERTEX) {
		printf("�׷��� ���� ���� �ʰ�\n");
		exit(1);
	}
	g->n++;
}

void insert_edge(Gtype* g, int u, int v) {
	Gnode* node;
	if (u >= g->n || v >= g->n) {
		printf("�ش� ���� ��ȣ ����\n");
		exit(1);
	}
	node = (Gnode*)malloc(sizeof(Gnode));
	node->vertex = v;
	node->link = g->adj_list[u];
	g->adj_list[u] = node;
}

void dfs_list(Gtype* g, int v) {
	Gnode* w=NULL;
	visited[v] = 1;
	printf("%c -> ", (v+65));
	for (w = g->adj_list[v]; w; w = w->link)
		if (!visited[w->vertex])
			dfs_list(g, w->vertex);
}

void print_adglist(Gtype* g) {
	for (int i = 0; i < g->n; i++) {
		Gnode* p = g->adj_list[i];
		printf("���� %d�� ����Ʈ", i);
		while (p != NULL) {
			printf("->%c", p->vertex+'A');
			p = p->link;
		}
		printf("\n");
	}
}

int main() {
	int select;
	char u, v;
	int uval, vval;
	Gtype* p;
	p = (Gtype*)malloc(sizeof(Gtype));
	init(p);
	for (int i = 0; i < MAX_VERTEX; i++)
		insert_vertex(p, i);

	do {
		printf("�Ͻð��� �ϴ� Ȱ���� ���ÿ�(1.���� �߰�, 2.���� �� DFS���):");
		scanf("%d", &select);
		switch (select) {
		case 1:
			printf("\n�߰� �ϰ��� �ϴ� ������ �� ���� ������ �Է����ֽʽÿ�:");
			scanf("%c", &u);
			scanf("%c", &v);
			uval = u;
			vval = v;
			printf("%d\n", uval);
			printf("%d\n", vval);
			insert_edge(p, uval, vval);
			insert_edge(p, vval, uval);
			printf("\n�߰� �Ǿ����ϴ�.\n");
			break;

		case 2:
			break;
		}
	}while (select != 2);

	dfs_list(p, 0);
	printf("\n");
	free(p);
	return 0;
}
