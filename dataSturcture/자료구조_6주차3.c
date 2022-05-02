#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#define MAX_LENGHT 16

int parent[MAX_LENGHT];
int count[MAX_LENGHT];

init_arr() {
	for (int i = 1; i <= 15; i++) {
		parent[i] = i;//ó������ �ڱ��ڽ��� ��Ʈ���� �ȴ�.
		count[i] = 0;//ó������ �ϳ��� �����Ƿ� ���̴� 0���� �����Ѵ�.
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
	j = Find(j);//�� �Է��� ��Ʈ ���� ����
	
	if (i == j) return;//��Ʈ�� ���ٴ� ���� �̹� ���� ����
	if (count[i] < count[j]) {//i�� Ʈ���� ���̰� j���� ������
		parent[i] = j;//i�� Ʈ���� j �ؿ� �Ҽ�
	}
	else {
		parent[j] = i;//�ݴ��� ���
		/*�� Ʈ���� ���̰� ���� ��쿡�� �ᱹ Ŀ���� ���� ��Ʈ+�ڽ��� ���̰� �ǹǷ� +1�� ���ش�*/
		if (count[i] == count[j])
			count[i]++;
	}
}

void print_root() {
	for(int i=1;i<16;i++)
		printf("��� %d�� root�� %d\n", i, parent[i]);
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

	printf("[�� ��尡 ���� ���տ� ���Ͽ����� ���� �ٸ� ���տ� ���Ͽ����� ���θ� ���]\n");
	do {
		printf("�ΰ��� ��� �Է�(����: ���1 ���2):");
		scanf("%d %d", &n1, &n2);
		printf("\n");
		r1 = parent[n1];
		r2 = parent[n2];

		if (r1 == r2)
			printf("��� %d�� ��� %d�� ���� ���տ� ���� �����Դϴ�.\n", n1, n2);
		else
			printf("��� %d�� ��� %d�� ���� �ٸ� ���տ� ���� �����Դϴ�.\n", n1, n2);
		printf("�� �Է��Ϸ��� 1, �׸��Ϸ��� 2�� �Է��ϼ���:");
		scanf("%d", &check);
		printf("\n");
	} while (check != 2);
	return 0;
}