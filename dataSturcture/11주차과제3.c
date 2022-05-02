#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

#define INF 10000
#define MAX_VERTEX 10

typedef struct Gtype {
	int n;
	int weight[MAX_VERTEX][MAX_VERTEX];
}Gtype;

int distance[MAX_VERTEX][MAX_VERTEX];

void allcosts(Gtype* g) {
	int i, j, k;

	for (i = 0; i < g->n; i++) {
		for (j = 0; j < g->n; j++) {
			distance[i][j] = g->weight[i][j];
		}
	}

	for (k = 0; k < g->n; k++) {
		for (i = 0; i < g->n; i++) {
			for (j = 0; j < g->n; j++) {
				if (distance[i][k] + distance[k][j] < distance[i][j])
					distance[i][j] = distance[i][k] + distance[k][j];
			}
		}
	}
}

int main() {
	Gtype g = { 6, {
	{0,50,45,10,INF,INF},
	{INF,0,10,15,INF,INF},
	{INF,INF,0,INF,30,INF},
	{20,INF,INF,0,15,INF},
	{INF,20,35,INF,0,INF},
	{INF,INF,INF,INF,3,0}}
	};

	allcosts(&g);

	printf("========================\n");
	for (int i = 0; i < g.n; i++) {
		for (int j = 0; j < g.n; j++) {
			if (distance[i][j] == INF)
				printf("%4c", '*');
			else
				printf("%4d", distance[i][j]);
		}
		printf("\n");
	}
	printf("========================\n");

	return 0;
}