#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define INF 10000
#define MAX_VERTEX 15

typedef struct Edge {
	int u;
	int v;
}Edge;

typedef struct K_Edge {
	int start, end, weight;
}K_Edge;

typedef struct Gtype {
	int n;
	int weight[MAX_VERTEX][MAX_VERTEX];
}Gtype;

typedef struct Heap {
	int heap_size;
	K_Edge heap_edge[2 * MAX_VERTEX];
}Heap;

int parent[MAX_VERTEX];
int prim_selected[MAX_VERTEX];
int sollin_selected[MAX_VERTEX][MAX_VERTEX];
int distance[MAX_VERTEX];
Edge store[2 * MAX_VERTEX];
int capacity = 0;

int compare(const void* a, const void* b) {
	K_Edge* x = (K_Edge*)a;
	K_Edge* y = (K_Edge*)b;
	return (x->weight - y->weight);
}

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


int min_vertex(int n) {
	int v=0, i;
	for (i = 0; i < n; i++) {
		if (!prim_selected[i]) {//아직 선택된적이 없는 정점을 순서에서 하나 고름
			v = i;
			break;
		}
	}
	for (i = 0; i < n; i++) {
		if (!prim_selected[i] && (distance[i] < distance[v])) v = i;
	}

	return v;
}

void min_edge(Gtype* g, int start) {
	int i, j;
	int vnum = 1;
	int min;
	int edge_accepted = 0;
	int index1 = 0, index2 = 0;
	int k = 0;
	Edge temp;

	for (i = 0; i < g->n; i++) {
		prim_selected[i] = 0;
	}
	prim_selected[start] = 1;
	while (edge_accepted != (g->n) - 1) {
		min = INF;
		for (i = 0; i < g->n; i++) {
			if (!prim_selected[i]) continue;
			for (j = 0; j < g->n; j++) {
				if (!prim_selected[j] && g->weight[i][j] < min && g->weight[i][j] != 0) {
					min = g->weight[i][j];
					index1 = i;
					index2 = j;
				}
			}
		}
		prim_selected[index2] = 1;
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
		prim_selected[u] = 0;
	}
	distance[start] = 0;
	for (i = 0; i < g->n; i++) {
		u = min_vertex(g->n);
		prim_selected[u] = 1;
		if (distance[u] == INF) return;
		for (v = 0; v < g->n; v++) {
			if (g->weight[u][v] != INF)
				if (!prim_selected[v] && g->weight[u][v] < distance[v]) {
					distance[v] = g->weight[u][v];
				}
		}
	}
}

void prim_tree(Gtype* g, int start) {

	prim(g, start);

	int sum = 0;
	min_edge(g, start);
	printf("Edge: ");
	for (int i = 0; i < (g->n) - 1; i++) {
		printf("(%d,%d) ", store[i].u, store[i].v);
		sum += g->weight[store[i].u][store[i].v];
	}
	printf("\nMinimum cost: %d\n", sum);
}

void First_stage(Gtype* g) {
	int i, j;
	int min;
	int index1 = 0, index2 = 0;
	Edge e;

	for (i = 0; i < g->n; i++) {
		min = INF;
		for (j = 0; j < g->n; j++) {
			if (g->weight[i][j] < min && g->weight[i][j] != 0 && !sollin_selected[i][j]) {
				min = g->weight[i][j];
				index1 = i;
				index2 = j;
			}
		}
		sollin_selected[index1][index2] = 1;
		e.u = index1;
		e.v = index2;
		store[capacity++] = e;
	}
}

void delete_duplicate() {
	int i, j, k;
	int u, v;
	Edge temp;

	for (i = 0; i < capacity; i++) {
		u = store[i].u;
		v = store[i].v;
		for (j = 0; j < capacity; j++) {
			if (store[j].u == v && store[j].v == u) {
				if (j == capacity - 1) {
					capacity--;
					break;
				}
				for (k = j; k < capacity - 1; k++) {
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
	int index1 = 0, index2 = 0;
	int min;
	int uval, vval;
	Edge e;

	while (capacity != (g->n) - 1) {
		min = INF;
		for (i = 0; i < g->n; i++) {
			for (j = 0; j < g->n; j++) {
				uval = set_find(i);
				vval = set_find(j);
				if (!sollin_selected[i][j] && g->weight[i][j] < min && g->weight[i][j] != 0 && uval != vval) {
					min = g->weight[i][j];
					index1 = i;
					index2 = j;
				}
			}
		}
		sollin_selected[index1][index2] = 1;
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
	delete_duplicate();
	Second_stage(g);

	int sum = 0;
	printf("Edge: ");
	for (int i = 0; i < (g->n) - 1; i++) {
		printf("(%d,%d) ", store[i].u, store[i].v);
		sum += g->weight[store[i].u][store[i].v];
	}
	printf("\nMinimum cost: %d\n", sum);
}

void init(Heap* h) {
	h->heap_size = 0;
}

void Kruskal_Edge(Heap* h, Gtype* g, int n) {
	int i, j;
	K_Edge e;
	for (i = 0; i < n; i++) {
		for (j = i; j < n; j++) {
			if (g->weight[i][j] != 0 && g->weight[i][j] != INF) {
				e.start = i;
				e.end = j;
				e.weight = g->weight[i][j];
				h->heap_edge[h->heap_size++] = e;
			}
		}
	}
}

void Kruskal(Heap* h, int n) {
	int edge_accpeted = 0;
	int uset, vset;
	K_Edge e;
	int sum = 0;

	set_init(h->heap_size);

	qsort(h->heap_edge, h->heap_size, sizeof(K_Edge), compare);

	int i = 0;

	printf("Edge: ");

	while (edge_accpeted < (n - 1)) {
		e = h->heap_edge[i];
		uset = set_find(e.start);
		vset = set_find(e.end);
		if (uset != vset) {
			printf("(%d,%d) ", e.start, e.end);
			sum += e.weight;
			edge_accpeted++;
			set_union(uset, vset);
		}
		i++;
	}
	printf("\nMinimum cost: %d\n", sum);
}	

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

int bellman(Gtype* g, int start) {
	int i, j;
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

	for (int u = 0; u < g->n; u++) {
		for (int v = 0; v < g->n; v++) {
			if (distance[v] > distance[u] + g->weight[u][v]) {
				printf("a negative cycle exists");
				exit(1);
			}
		}
	}
}

void print_bellman(Gtype* g) {
	int end;
	double start;
	double end2;
	printf("도착지역을 입력하세요:");
	scanf("%d", &end);

	start = clock();
	route_init(route);
	bellman(g, 0);
	end2 = clock();
	printf("0번 vertex에서 %d번 vertex까지 shortest path: ", end);
	for (int i = 0; i < MAX_VERTEX; i++) {
		if (route[end][i] != INF)
			printf("%d ", route[end][i]);
	}
	printf("%d\n0번 vertex에서 %d번 vertex까지 shortest path의 길이: %d\n", end, end, distance[end]);
	printf("Complexity: %f(sec)\n", (end2 - start)/1000);
}

int all_route[MAX_VERTEX][MAX_VERTEX];
int all_distance[MAX_VERTEX][MAX_VERTEX];

void allcosts(Gtype* g) {
	int i, j, k;

	for (i = 0; i < g->n; i++) {
		for (j = 0; j < g->n; j++) {
			all_distance[i][j] = g->weight[i][j];
		}
	}

	for (k = 0; k < g->n; k++) {
		for (i = 0; i < g->n; i++) {
			for (j = 0; j < g->n; j++) {
				if (all_distance[i][k] + all_distance[k][j] < all_distance[i][j]) {
					all_distance[i][j] = all_distance[i][k] + all_distance[k][j];
					all_route[i][j] = k;
				}
			}
		}
	}
}

void Print_allcost(Gtype* g) {
	int src, dest;
	int i, j, k = 0;
	int arr[MAX_VERTEX];
	double start = 0;
	double end;

	for (i = 0; i < MAX_VERTEX; i++) {
		for (j = 0; j < MAX_VERTEX; j++) {
			all_route[i][j] = i;
		}
	}

	printf("출발 vertex를 입력하세요: ");
	scanf("%d", &src);
	printf("도착 vertex를 입력하세요: ");
	scanf("%d", &dest);

	start = clock();
	allcosts(g);
	

	for (i = 0; i < MAX_VERTEX; i++) {
		arr[i] = src;
	}

	i = src;
	j = dest;

	while (all_route[i][j] != src) {
		arr[k++] = all_route[i][j];
		j = all_route[i][j];
	}

	end = clock();

	printf("%d번 vertex에서 %d번 vertex까지 shortest path: ", src, dest);
	for (i = k; i >= 0; i--) {
		printf("%d ", arr[i]);
	}
	printf("%d", dest);
	printf("\n%d번 vertex에서 %d번 vertex까지 shortest path까지의 길이: %d\n", src, dest, all_distance[src][dest]);
	printf("Complexity: %f(sec)\n", (end - start)/1000);
}

#define MAX_WORD 15

void bubblesort(char word[][MAX_WORD], int n) {
	int i, j;
	char temp[15];

	for (i = 0; i < n - 1; i++) {
		for (j = 0; j < n - 1 - i; j++) {
			if (strcmp(word[j], word[j + 1]) == 1) {
				strcpy(temp, word[j]);
				strcpy(word[j], word[j + 1]);
				strcpy(word[j + 1], temp);
			}
		}
	}

	printf("Bubble sort algorithm을 이용한 정렬 결과: ");
	for (i = 0; i < n; i++)
		printf("%s ", word[i]);
	printf("\n");
}

int partition(char word[][MAX_WORD], int left, int right) {
	char pivot[15];
	int low, high;
	char temp[15];

	low = left;
	high = right + 1;
	strcpy(pivot, word[left]);

	do {
		do {
			low++;
		} while (strcmp(word[low], pivot) == -1);
		do {
			high--;
		} while (strcmp(word[high], pivot) == 1);
		if (low < high) {
			strcpy(temp, word[low]);
			strcpy(word[low], word[high]);
			strcpy(word[high], temp);
		}
	} while (low < high);
	strcpy(temp, word[left]);
	strcpy(word[left], word[high]);
	strcpy(word[high], temp);

	return high;
}

void quick_sort(char word[][MAX_WORD], int left, int right) {
	if (left < right) {
		int pivot = partition(word, left, right);
		quick_sort(word, left, pivot - 1);
		quick_sort(word, pivot + 1, right);
	}
}

void print_quick(char word[][MAX_WORD], int left, int right, int chance) {
	quick_sort(word, left, right);
	printf("Quick sort algorithm을 이용한 정렬 결과\n");
	for (int i = 0; i < chance; i++) {
		printf("%s ", word[i]);
	}
	printf("\n");
}

char sort[19][MAX_WORD];

void merge(char word[][MAX_WORD], int left, int mid, int right) {
	int i, j, k, l;
	i = left, j = mid + 1, k = left;

	while (i <= mid && j <= right) {
		if (strcmp(word[i], word[j]) != 1) {
			strcpy(sort[k++], word[i++]);
		}
		else
			strcpy(sort[k++], word[j++]);
	}
	if (i > mid) {
		for (l = j; l <= right; l++) {
			strcpy(sort[k++], word[l]);
		}
	}
	else
		for (l = i; l <= mid; l++) {
			strcpy(sort[k++], word[l]);
		}
	for (l = left; l <= right; l++)
		strcpy(word[l], sort[l]);
}

void merge_sort(char word[][MAX_WORD], int left, int right) {
	int mid;
	if (left < right) {
		mid = (left + right) / 2;
		merge_sort(word, left, mid);
		merge_sort(word, mid + 1, right);
		merge(word, left, mid, right);
	}
}

void print_merge(char word[][MAX_WORD], int left, int right, int chance) {
	merge_sort(word, left, right);
	printf("Merge sort algorithm을 이용한 정렬 결과\n");
	for (int i = 0; i < chance; i++) {
		printf("%s ", word[i]);
	}
	printf("\n");
}

typedef struct heap_word {
	char word[MAX_WORD];
}heap_word;

typedef struct W_heap {
	heap_word heap[20];
	int heap_size;
}W_heap;

#define getparent(x) (x-1)/2

void heap(char word[][MAX_WORD], int num) {
	char temp[MAX_WORD];
	for (int i = 1; i < num; i++) {
		int child = i;
		while (child > 0) {
			int root = getparent(child);
			if (strcmp(word[root], word[child]) == -1) {
				strcpy(temp, word[root]);
				strcpy(word[root], word[child]);
				strcpy(word[child], temp);
			}
			child = root;
		}
	}
}

void total_heapsort(char word[][MAX_WORD], int num) {
	heap(word, num);
	char temp[MAX_WORD];
	for (int i = num; i >= 0; i--) {
		strcpy(temp, word[i]);
		strcpy(word[i], word[0]);
		strcpy(word[0], temp);

		heap(word, i);
	}
}

void print_heapsort(char word[][MAX_WORD], int num) {
	total_heapsort(word, num);
	printf("Heap sort algorithm을 이용한 정렬 결과\n");
	for (int i = 0; i < num + 1; i++) {
		printf("%s ", word[i]);
	}
	printf("\n");
}

int main() {
	Gtype g = { 8,{
	{0,7,5,INF,INF,2,INF,INF},
	{7,0,INF,6,2,INF,INF,1},
	{5,INF,0,2,INF,5,INF,INF},
	{INF,INF,2,0,INF,INF,2,8},
	{INF,2,INF,INF,0,2,INF,4},
	{2,INF,5,INF,2,0,4,INF},
	{INF,INF,INF,2,INF,4,0,4},
	{INF,1,INF,8,4,INF,4,0}}
	};

	char word[20][MAX_WORD] = { "apple" ,"berry" ,"grape" ,"raisin" ,"banana" ,"watermelon" ,"grapefruit", "orange" ,"kiwi" ,"pear" ,"peach" ,"cherry" ,"pineapple" ,"coconut" ,"lemon" ,"citron" ,"fig" ,"celery" };

	int select = 0;
	int sel2 = 0;
	int sel3;
	int sel4;
	char addsel=0;
	Heap *h;
	h = (Heap*)malloc(sizeof(Heap));
	int chance = 0;
	int i = 0;
	double start = 0;
	double end;
	while (word[i][0] != NULL) {
		chance++;
		i++;
	}

	do {
		printf("[초기메뉴]\n");
		printf("1. Minimum cost spanning tree algorithm\n");
		printf("2. Shortest path algotirhm\n");
		printf("3. Sorting algorithm\n");
		printf("4. 프로그램 종료\n\n");
		printf("사용할 번호를 입력하세요: ");
		scanf("%d", &select);

		switch (select)
		{
		case 1: 
			do{
				printf("[Minimum cost spanning tree algorithm 상세메뉴]\n\n");
				printf("1. Kruskal's algorithm\n");
				printf("2. Prim's algorithm\n");
				printf("3. Sollin's algorithm\n");
				printf("4. 초기메뉴로 이동\n\n");
				printf("사용할 번호를 입력하세요: ");
				scanf("%d", &sel2);
				switch (sel2) {
					case 1:
						start=clock();
						init(h);
						Kruskal_Edge(h, &g, g.n);
						Kruskal(h, g.n);
						end = clock();
						printf("Complexity: %f(sec)\n", (end-start)/1000);
						printf("\n다른 알고리즘을 이용하시겠습니까? (Y/N) ");
						getchar();
						scanf("%c", &addsel);
						getchar();
						if (addsel == 'N' || addsel == 'n') {
							sel2 = 4;
						}
						break;

					case 2:
						start = clock();
						prim_tree(&g, 0);
						end = clock();
						printf("Complexity: %f(sec)\n", (end - start) / 1000);
						printf("\n다른 알고리즘을 이용하시겠습니까? (Y/N) ");
						getchar();
						scanf("%c", &addsel);
						getchar();
						if (addsel == 'N' || addsel == 'n') {
							sel2 = 4;
						}
						break;

					case 3:
						start = clock();
						total_sollin(&g);
						end = clock();
						printf("Complexity: %f(sec)\n", (end - start) / 1000);
						printf("\n다른 알고리즘을 이용하시겠습니까? (Y/N) ");
						getchar();
						scanf("%c", &addsel);
						getchar();
						if (addsel == 'N' || addsel == 'n') {
							sel2 = 4;
						}
						break;

					case 4:
						break;
				}
			} while (sel2 != 4);
			break;
		case 2:
			do {
				printf("[Shortest path algotirhmt 상세메뉴]\n\n");
				printf("1. Bellman-ford's algorithm\n");
				printf("2. All pairs shortest path's algorithm\n");
				printf("3. 초기메뉴로 이동\n\n");
				printf("사용할 번호를 입력하세요: ");
				scanf("%d", &sel3);
				switch (sel3)
				{
				case 1:
					print_bellman(&g);
					printf("\n다른 알고리즘을 이용하시겠습니까? (Y/N) ");
					getchar();
					scanf("%c", &addsel);
					getchar();
					if (addsel == 'N' || addsel == 'n') {
						sel3 = 3;
					}
					break;
				case 2:
					Print_allcost(&g);
					printf("\n다른 알고리즘을 이용하시겠습니까? (Y/N) ");
					getchar();
					scanf("%c", &addsel);
					getchar();
					if (addsel == 'N' || addsel == 'n') {
						sel3 = 3;
					}
					break;
				case 3:
					break;
				}
			} while (sel3 != 3);
			break;
		case 3:
			do {
				printf("[Sorting algorithm 상세메뉴]\n\n");
				printf("1. Bubble sort algorithm\n");
				printf("2. Quick sort algorithm\n");
				printf("3. Merge sort algorithm\n");
				printf("4. Heap sort algorithm\n");
				printf("5. 초기메뉴로 이동\n\n");
				printf("사용할 번호를 입력하세요: ");
				scanf("%d", &sel4);
				switch (sel4)
				{
				case 1:
					start = clock();
					bubblesort(word,chance);
					end = clock();
					printf("Complexity: %f(sec)\n", (end - start) / 1000);
					printf("\n다른 알고리즘을 이용하시겠습니까? (Y/N) ");
					getchar();
					scanf("%c", &addsel);
					getchar();
					if (addsel == 'N' || addsel == 'n') {
						sel4 = 5;
					}
					break;
				case 2:
					start = clock();
					print_quick(word, 0, chance - 1, chance);
					end = clock();
					printf("Complexity: %f(sec)\n", (end - start) / 1000);
					printf("\n다른 알고리즘을 이용하시겠습니까? (Y/N) ");
					getchar();
					scanf("%c", &addsel);
					getchar();
					if (addsel == 'N' || addsel == 'n') {
						sel4 = 5;
					}
					break;
				case 3:
					start = clock();
					print_merge(word, 0, chance - 1, chance);
					end = clock();
					printf("Complexity: %f(sec)\n", (end - start) / 1000);
					printf("\n다른 알고리즘을 이용하시겠습니까? (Y/N) ");
					getchar();
					scanf("%c", &addsel);
					getchar();
					if (addsel == 'N' || addsel == 'n') {
						sel4 = 5;
					}
					break;
				case 4:
					start = clock();
					print_heapsort(word, chance - 1);
					end = clock();
					printf("Complexity: %f(sec)\n", (end - start) / 1000);
					printf("\n다른 알고리즘을 이용하시겠습니까? (Y/N) ");
					getchar();
					scanf("%c", &addsel);
					getchar();
					if (addsel == 'N' || addsel == 'n') {
						sel4 = 5;
					}
					break;
				case 5:
					break;
				}
			} while (sel4 != 5);
		}

	} while (select != 4);

	return 0;
}
