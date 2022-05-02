#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#define MAX_VERTEX 10
#define INF 1000

typedef struct Gtype {
	int n;
	int weight[MAX_VERTEX][MAX_VERTEX];
}Gtype;

typedef struct Edge {
	int u;
	int v;
}Edge;

int parent[MAX_VERTEX];
int selected[MAX_VERTEX][MAX_VERTEX];
Edge store[MAX_VERTEX];
int capacity=0;

void set_init(int n) {
	for (int i = 0; i < n; i++) {
		parent[i] = -1;
	}
}

int set_find(int vertex) {
	if (parent[vertex] == -1)
		return vertex;
	while (parent[vertex] != -1) vertex = parent[vertex];
	return vertex;
}

void set_union(int a, int b) {
	int root1 = set_find(a);
	int root2 = set_find(b);
	if (root1 != root2)
		parent[root1] = root2;
}

void First_stage(Gtype* g) {
	int i, j;
	int min;
	int index1=0, index2=0;
	Edge e;

	for (i = 0; i < g->n; i++) {
		min = INF;
		for (j = 0; j < g->n; j++) {
			if (g->weight[i][j] < min && g->weight[i][j] != 0 && !selected[i][j]) {
				min = g->weight[i][j];
				index1 = i;
				index2 = j;
			}
		}
		selected[index1][index2] = 1;
		e.u = index1;
		e.v = index2;
		store[capacity++] = e;
	}
}

void delete_duplicate() {
	int i, j,k;
	int u, v;
	Edge temp;

	for (i = 0; i < capacity; i++) {
		u = store[i].u;
		v = store[i].v;
		for (j = 0; j < capacity; j++) {
			if (store[j].u == v && store[j].v == u) {
				if (j == capacity-1) {
					capacity--;
					break;
				}
				for (k = j; k < capacity-1; k++) {
					temp = store[k];
					store[k] = store[k + 1];
					store[k + 1] = temp;
				}
				capacity--;
			}
		}
	}
}

void Second_stage(Gtype* g) {
	int i, j;
	int index1=0, index2=0;
	int min;
	int uval, vval;
	Edge e;

	while (capacity != (g->n) - 1) {
		min = INF;
		for (i = 0; i < g->n; i++) {
			for (j = 0; j < g->n; j++) {
				uval = set_find(i);
				vval = set_find(j);
				if (!selected[i][j] && g->weight[i][j] < min && g->weight[i][j] != 0 && uval != vval) {
					min = g->weight[i][j];
					index1 = i;
					index2 = j;
				}
			}
		}
		selected[index1][index2]=1;
		e.u = index1;
		e.v = index2;
		store[capacity++] = e;
	}
}

void total_sollin(Gtype* g) {
	int uval;
	int vval;
	
	set_init(g->n);
	First_stage(g);
	delete_duplicate();
	for (int i = 0; i < capacity; i++) {
		uval = set_find(store[i].u);
		vval = set_find(store[i].v);
		if (uval != vval) set_union(uval, vval);
	}
	Second_stage(g);

	printf("T={ ");
	for (int i = 0; i < (g->n) - 1; i++) {
		printf("(%d,%d) ", store[i].u, store[i].v);
	}
	printf("}\n");
}

int main() {
	Gtype g = { 7,{
	{0,3,17,6,1000,1000,1000},
	{3,0,1000,5,1000,1000,12},
	{17,1000,0,1000,10,8,1000},
	{6,5,1000,0,9,1000,1000},
	{1000,1000,10,9,0,4,2},
	{1000,1000,8,1000,4,0,14},
	{1000,12,1000,1000,2,14,0}}
	};

	total_sollin(&g);

	return 0;
}
