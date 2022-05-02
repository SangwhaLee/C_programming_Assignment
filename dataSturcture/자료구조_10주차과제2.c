#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

#define INF 1000
#define MAX_VERTEX 10

typedef struct Edge {
	int u;
	int v;
}Edge;

typedef struct Gtype {
	int n;
	int weight[MAX_VERTEX][MAX_VERTEX];
}Gtype;

int selected[MAX_VERTEX];
int distance[MAX_VERTEX];
Edge store[2*MAX_VERTEX];

int min_vertex(int n) {
	int v, i;
	for (i = 0; i < n; i++) {
		if (!selected[i]) {//아직 선택된적이 없는 정점을 순서에서 하나 고름
			v = i;
			break;
		}
	}
	for (i = 0; i < n; i++) {
		if (!selected[i] && (distance[i] < distance[v])) v = i;
	}

	return v;
}

void min_edge(Gtype* g, int start) {
	int i, j;
	int vnum=1;
	int min;
	int edge_accepted = 0;
	int index1=0, index2=0;
	int k = 0;
	Edge temp;

	for (i = 0; i < g->n; i++) {
		selected[i] = 0;
	}
	selected[start] = 1;
	while (edge_accepted != (g->n) - 1) {
		min = INF;
		for (i = 0; i < g->n ; i++) {
			if (!selected[i]) continue;
			for (j = 0; j < g->n; j++) {
				if (!selected[j] && g->weight[i][j] < min && g->weight[i][j] != 0) {
					min = g->weight[i][j];
					index1 = i;
					index2 = j;
				}
			}
		}
		selected[index2] = 1;
		temp.u = index1;
		temp.v = index2;
		store[k] = temp;
		k++;
		vnum++;
		edge_accepted++;
	}
}

void prim(Gtype* g, int start) {
	int i, v, u;

	for (u = 0; u < g->n; u++) {
		distance[u] = INF;
		selected[u] = 0;
	}
	distance[start] = 0;
	for (i = 0; i < g->n; i++) {
		u = min_vertex(g->n);
		selected[u] = 1;
		if (distance[u] == INF) return;
		printf("%d ", u);
		for (v = 0; v < g->n; v++) {
			if (g->weight[u][v] != INF)
				if (!selected[v] && g->weight[u][v] < distance[v]) {
					distance[v] = g->weight[u][v];
				}
		}
	}
}

void print_tree(Gtype* g, int start) {
	printf("TV={ ");
	prim(g, start);
	printf("}\n");

	min_edge(g, start);
	printf("T={ ");
	for (int i = 0; i < (g->n) - 1; i++)
		printf("(%d,%d) ", store[i].u, store[i].v);
	printf("}");
}

int main() {
	Gtype g = { 7,
	{{0,3,17,6,INF,INF,INF},
	{3,0,INF,5,INF,INF,12},
	{17, INF, 0, INF, 10 ,8,INF},
	{6, 5, INF, 0, 9, INF, INF},
	{INF,INF,10,9,0,4,2},
	{INF,INF,8,INF,4,0,14},
	{INF,12,INF,INF,2,14,0}}
};
	
	print_tree(&g, 0);

	return 0;
}