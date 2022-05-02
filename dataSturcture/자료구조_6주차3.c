#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#define MAX_LENGHT 16

int parent[MAX_LENGHT];
int count[MAX_LENGHT];

init_arr() {
	for (int i = 1; i <= 15; i++) {
		parent[i] = i;//처음에는 자기자신이 루트값이 된다.
		count[i] = 0;//처음에는 하나만 있으므로 높이는 0으로 간주한다.
	}
}

int Find(int i) {
	if (i == parent[i])
		return i;
	else {
		return parent[i] = Find(parent[i]);
	}
}


void Union(int i, int j) {
	i = Find(i);
	j = Find(j);//두 입력의 루트 값을 도출
	
	if (i == j) return;//루트가 같다는 것은 이미 같은 집합
	if (count[i] < count[j]) {//i의 트리의 높이가 j보다 낮으면
		parent[i] = j;//i의 트리를 j 밑에 소속
	}
	else {
		parent[j] = i;//반대의 경우
		/*두 트리의 높이가 같은 경우에는 결국 커지는 쪽은 루트+자신의 높이가 되므로 +1를 해준다*/
		if (count[i] == count[j])
			count[i]++;
	}
}

void print_root() {
	for(int i=1;i<16;i++)
		printf("노드 %d의 root는 %d\n", i, parent[i]);
}


int main() {
	int n1, n2;
	int r1, r2;
	int check;

	init_arr();
	Union(1, 2);
	Union(1, 3);
	Union(1, 4);
	Union(3, 5);
	Union(5, 6);
	Union(7, 8);
	Union(9, 10);
	Union(10, 11);
	Union(12, 14);

	print_root();

	printf("[두 노드가 같은 집합에 속하였는지 서로 다른 집합에 속하였는지 여부를 출력]\n");
	do {
		printf("두개의 노드 입력(형식: 노드1 노드2):");
		scanf("%d %d", &n1, &n2);
		printf("\n");
		r1 = parent[n1];
		r2 = parent[n2];

		if (r1 == r2)
			printf("노드 %d과 노드 %d는 같은 집합에 속한 노드들입니다.\n", n1, n2);
		else
			printf("노드 %d과 노드 %d는 서로 다른 집합에 속한 노드들입니다.\n", n1, n2);
		printf("더 입력하려면 1, 그만하려면 2를 입력하세요:");
		scanf("%d", &check);
		printf("\n");
	} while (check != 2);
	return 0;
}