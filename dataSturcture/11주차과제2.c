#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

#define INF 10000
#define MAX_VERTEX 10

typedef struct Gtype {
	int n;
	int weight[MAX_VERTEX][MAX_VERTEX];
}Gtype;

int distance[MAX_VERTEX];
int income[MAX_VERTEX];
int pathlength = 1;

void BellmanFord(Gtype* g,int src) {

	for (int i = 0; i < g->n; i++)
		distance[i] = g->weight[src][i];

	for (int i = 0; i < g->n; i++) {
		for (int j = 0; j < g->n; j++) {
			if (g->weight[j][i] != 0 && g->weight[j][i] != INF) {
				income[i]=1;
				break;
			}
		}
	}

	while (pathlength != (g->n) - 1) {
		for (int u = 0; u < g->n; u++) {
			if (u != src&& income[u] == 1) {
				for (int i = 0; i < g->n; i++) {
					if (distance[u] > distance[i] + g->weight[i][u])
						distance[u] = distance[i] + g->weight[i][u];
				}
			}
		}
		pathlength++;
	}

	for (int u = 0; u < g->n; u++) {
		for (int v = 0; v < g->n; v++) {
			if (distance[v] > distance[u] + g->weight[u][v]) {
				printf("a negative cycle exists");
				exit(1);
			}
		}
	}
}

int main() {
	Gtype g = { 6,{
	{0,10,INF,20,INF,INF},
	{INF,0,60,INF,INF,INF},
	{INF,INF,0,INF,-30,20},
	{INF,20,INF,0,30,INF},
	{INF,INF,INF,INF,0,10},
	{INF,INF,INF,INF,INF,INF}}
	};
	int dest;

	BellmanFord(&g, 0);

	printf("도착 vertex를 입력하세요:");
	scanf("%d", &dest);

	printf("Vertex 0번에서 Vertex %d번까지 shortest path의 길이는 %d입니다.", dest, distance[dest]);

	return 0;
}
