#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

#define INF 10000
#define MAX_VERTEX 7

typedef struct Gtype {
	int n;
	int weight[MAX_VERTEX][MAX_VERTEX];
}Gtype;

int distance[MAX_VERTEX];
int found[MAX_VERTEX];
int route[MAX_VERTEX][MAX_VERTEX];
int check[MAX_VERTEX];

int choose(int distance[], int n, int found[]) {
	int i, min, minpos;
	min = INT_MAX;
	minpos = -1;
	for (i = 0; i < n; i++) {
		if (distance[i] < min && !found[i]) {
			min = distance[i];
			minpos = i;
		}
	}
	return minpos;  
}

void route_init(int route[][MAX_VERTEX]) {
	int i, j;
	for (i = 0; i < MAX_VERTEX; i++) {
		for (j = 0; j < MAX_VERTEX; j++) {
			route[i][j] = INF;
		}
	}
}

int shortest_path(Gtype* g, int start) {
	int i,j;
	int u, w;
	for (i = 0; i < g->n; i++) {
		distance[i] = g->weight[start][i];
		found[i] = 0;
		check[i] = 1;
		route[i][0] = start;
	}
	found[start] = 1;
	distance[start] = 0;
	for (i = 0; i < g->n; i++) {
		u = choose(distance, g->n, found);
		found[u] = 1;
		for (w = 0; w < g->n; w++)
			if (!found[w])
				if (distance[u] + g->weight[u][w] < distance[w]) {
					if (i == 0) {
						route[w][check[w]] = u;
						check[w]++;
					}
					else {
						for (j = 0; j < (check[u] + 1); j++) {
							route[w][j] = route[u][j];
							route[w][j + 1] = u;
							check[w]++;
						}
					}
					distance[w] = distance[u] + g->weight[u][w];
				}
	}
}

int main() {
	char end;
	Gtype g = { 7,{
	{0,10,7,INF,15,INF,INF},
	{10,0,INF,5,8,INF,INF},
	{7,INF,0,INF,6,INF,INF},
	{0,5,INF,0,12,6,INF},
	{15,8,6,12,0,INF,3},
	{INF,INF,INF,6,INF,0,1},
	{INF,INF,INF,INF,3,1,0}
	} };

	printf("도착지역을 입력하세요:");
	scanf("%c", &end);
	getchar();

	route_init(route);

	shortest_path(&g,0);
	printf("Vertex A에서 Vertex %c로 가는 최단 경로는", end);
	for (int i = 0; i < MAX_VERTEX; i++) {
		if (route[end - 'A'][i] != INF)
			printf(" %c", route[end - 'A'][i] + 'A');
	}
	printf(" %c이며, 이때 최단 거리는 %d입니다.",end, distance[end - 'A']);

	return 0;

}