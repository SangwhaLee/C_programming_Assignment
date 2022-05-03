#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "util.h"

int is_large(int num1, int num2)
{
	return num1 > num2;
}

int sum_x(int x1, int x2)
{
	int sum = 0;
	/* Fill this function */
	sum = x1+x2;
	return sum;
}

void sub_y(int y1, int y2, int *sub)
{
	/* Fill this function */
	int* y1_ad = &y1;
	int* y2_ad = &y2;
	*sub = *y1_ad - *y2_ad;
}

// You have to allocate memory for pointer members of "struct Point_ref"
// Hint: Use malloc()
void Point_val_to_Point_ref(struct Point_val *P1, struct Point_ref *P2)
{
	P2->x = (int*)malloc(sizeof(int));
	P2->y = (int*)malloc(sizeof(int));
	*P2->x = P1->x;
	*P2->y = P1->y;
	/* Fill this function */
}

void Point_ref_to_Point_val(struct Point_ref *P1, struct Point_val *P2)
{
	/* Fill this function */
	P2->x = *P1->x;
	P2->y = *P1->y;
}

int calc_area1(struct Point_val *P1, struct Point_val *P2)
{
	/* Fill this function */
	int sub_x=0;
	int sub_y=0;
	if (is_large(P2->x ,P1->x))
		sub_x = P2->x-P1->x;
	else
		sub_x = P1->x - P2->x;
	
	if(is_large(P2->y,P1->y))
		sub_y = P2->y - P1->y;
	else
		sub_y = P1->y - P2->y;
	int area = 0;
	area = sub_x*sub_y;
	return area;
}

void calc_area2(struct Point_ref *P1, struct Point_ref *P2, int *area)
{
	/* Fill this function */
	int sub_x=0;
	int sub_y=0;
	if (is_large(*P2->x , *P1->x))
		sub_x = *P2->x-*P1->x;
	else
		sub_x = *P1->x - *P2->x;
	
	if(is_large(*P2->y,*P1->y))
		sub_y = *P2->y - *P1->y;
	else
		sub_y = *P1->y - *P2->y;

	*area = sub_x*sub_y;
}

char* reverse(char *word)
{
	/* Fill this function */
	char* first = word;
	char* last = word+(strlen(word)-1);
	while(first<last){
		char temp = *first;
		*first = *last;
		*last = temp;

		first++;
		last--;
	}
	return word;
}
