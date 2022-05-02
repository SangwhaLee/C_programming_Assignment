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

void set_init(int n) {//��� �θ��尪�� -1�� ���ձԸ��� 1�� ����
	for (int i = 0; i < n; i++) {
		parent[i] = -1;
		num[i] = 1;
	}

}

int set_find(int cur) {//-1�̸� �ڱ��ڽ��� �ƴϸ� �θ��� ���� -1�� ���ö����� Ÿ�� �ö󰣴�.
	if (parent[cur] == -1)
		return cur;
	while (parent[cur] != -1) cur = parent[cur];
	return cur;
}

void set_union(int a, int b) {
	if (num[a] < num[b]) {//������ �Ը��� �������� ū�𿡰� ���δ�.
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
	//���� ���ε��� ��尡 �ڽ��� �θ��� ���� �۴ٸ�
	while ((i != 1) && (item.weight < h->heap_edge[i / 2].weight)) {
		//�ڽź��� ���� �θ��尡 ���ö����� ������ ��ġ�� �ű��.
		h->heap_edge[i] = h->heap_edge[i / 2];
		i /= 2;
	}
	h->heap_edge[i] = item;
}

Edge delete_min_heap(Heap* h) {
	int parent, child;
	Edge item, temp;

	item = h->heap_edge[1];//����� ���
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

	printf("Kruska��s algorithm minimum cost spanning tree : T = { ");
	while (edge_cnt < (n - 1)) {//������ ���� n-1���� �ǵ��� �Ѵ�.
		e = delete_min_heap(h);//������ ������ ���´�.
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

	printf("��� ������ �����ϱ�ڽ��ϱ�?:");
	scanf("%d", &vertex_cnt);

	do {	
		printf("���ϰ��� �ϴ� ������ �糡 ������ �Ҹ��� �Է����ּ���:");
		scanf("%d %d %d", &e.start, &e.end, &e.weight);

		insert_min_heap(h, e);
		printf("\n������ �� �߰��Ͻðڽ��ϱ�?(1.�߰�, 2.���� �� ���):");
		scanf("%d", &select);
		printf("\n");
	} while (select != 2);

	kruskal(h, vertex_cnt);
	printf("}\n");
	return 0;
}