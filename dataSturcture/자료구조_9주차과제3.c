#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#define MAX_VERTEX 10

int parent[MAX_VERTEX];
int num[MAX_VERTEX];

typedef struct Edge {
	int start, end, weight;
}Edge;

typedef struct Heap {
	int heap_size;
	Edge heap_edge[2 * MAX_VERTEX];
}Heap;

void set_init(int n) {//모든 부모노드값을 -1로 집합규모값은 1로 설정
	for (int i = 0; i < n; i++) {
		parent[i] = -1;
		num[i] = 1;
	}

}

int set_find(int cur) {//-1이면 자기자신을 아니면 부모노드 값이 -1이 나올때까지 타고 올라간다.
	if (parent[cur] == -1)
		return cur;
	while (parent[cur] != -1) cur = parent[cur];
	return cur;
}

void set_union(int a, int b) {
	if (num[a] < num[b]) {//집합의 규모중 작은놈을 큰놈에게 붙인다.
		parent[a] = b;
		num[a] += num[b];
	}
	else {
		parent[b] = a;
		num[b] += num[a];
	}
}

void init(Heap* h) {
	h->heap_size = 0;
}

void insert_min_heap(Heap* h, Edge item) {
	int i;
	i = ++(h->heap_size);
	//만약 새로들어온 노드가 자신의 부모노드 보다 작다면
	while ((i != 1) && (item.weight < h->heap_edge[i / 2].weight)) {
		//자신보다 작은 부모노드가 나올때까지 앞으로 위치를 옮긴다.
		h->heap_edge[i] = h->heap_edge[i / 2];
		i /= 2;
	}
	h->heap_edge[i] = item;
}

Edge delete_min_heap(Heap* h) {
	int parent, child;
	Edge item, temp;

	item = h->heap_edge[1];//출력할 노드
	temp = h->heap_edge[(h->heap_size)--];//
	parent = 1;
	child = 2;
	while (child <= h->heap_size) {
		if ((child < h->heap_size) && (h->heap_edge[child].weight) > h->heap_edge[child + 1].weight)
			child++;
		if (temp.weight <= h->heap_edge[child].weight) break;
		h->heap_edge[parent] = h->heap_edge[child];
		parent = child;
		child *= 2;
	}
	h->heap_edge[parent] = temp;
	return item;
}

void kruskal(Heap* h, int n) {
	int edge_cnt=0;
	int u, v;
	int i = 0;
	Edge e;

	set_init(h->heap_size);

	printf("Kruska’s algorithm minimum cost spanning tree : T = { ");
	while (edge_cnt < (n - 1)) {//간선의 수가 n-1개가 되도록 한다.
		e = delete_min_heap(h);//힙에서 가지고 나온다.
		u = set_find(e.start);
		v = set_find(e.end);
		if (u != v) {
			printf("(%d,%d) ", e.start, e.end);
			edge_cnt++;
			set_union(u, v);
		}
		i++;
	}
}

int main() {
	Heap* h;
	Edge e;
	int select;
	int vertex_cnt=0;

	h = (Heap*)malloc(sizeof(Heap));
	init(h);

	printf("몇개의 정점을 설정하기겠습니까?:");
	scanf("%d", &vertex_cnt);

	do {	
		printf("더하고자 하는 간선의 양끝 정점과 소모값을 입력해주세요:");
		scanf("%d %d %d", &e.start, &e.end, &e.weight);

		insert_min_heap(h, e);
		printf("\n간선을 더 추가하시겠습니까?(1.추가, 2.종료 및 출력):");
		scanf("%d", &select);
		printf("\n");
	} while (select != 2);

	kruskal(h, vertex_cnt);
	printf("}\n");
	return 0;
}