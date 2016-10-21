#include <stdio.h>
#include "mylib.h"

#define NUM	20

enum CMP_RES {
	CMP_RES_EQUAL,
	CMP_RES_GREATER,
	CMP_RES_LESS,
};

typedef int (*CMP_CB)(void *data_l, void *data_r);
typedef int (*SWAP_CB)(void *data_l, void *data_r);
typedef void *(*VISIT_CB)(void *data, int id);

typedef struct sort_cfg {
	VISIT_CB	visit;
	CMP_CB		cmp;
	SWAP_CB		swap;
	int 		order;
	int 		num;
	void 		*data;
}Sort_Cfg;

static int _quick_sort(void *data, int start, int end, VISIT_CB visit, CMP_CB cmp, SWAP_CB swap)
{
	int ref = start, left = ref, right = end;

	P_VALID_CHECK_RET(data, -1);
	P_VALID_CHECK_RET(visit, -1);
	P_VALID_CHECK_RET(cmp, -1);
	P_VALID_CHECK_RET(swap, -1);

	if (start > end)
		return 0;

	while (left != right) {
		// Begin in last right, find less than ref.
		while (cmp(visit(data, right), visit(data, ref)) != CMP_RES_LESS && right > left) 
			right--;
		// Begin in frist left, find big than ref.
		while (cmp(visit(data, left), visit(data, ref)) != CMP_RES_GREATER && left < right)
			left++;
		if (left < right)
			swap(visit(data, left), visit(data, right));
	}

	swap(visit(data, right), visit(data, ref));
	_quick_sort(data, start, right-1, visit, cmp, swap);
	_quick_sort(data, right+1, end, visit, cmp, swap);
	return 0;
}

int quick_sort(Sort_Cfg *cfg)
{
	P_VALID_CHECK_RET(cfg, -1);
	P_VALID_CHECK_RET(cfg->visit, -1);
	P_VALID_CHECK_RET(cfg->cmp, -1);
	P_VALID_CHECK_RET(cfg->swap, -1);
	P_VALID_CHECK_RET(cfg->data, -1);
	_quick_sort(cfg->data, 0, cfg->num-1, cfg->visit, cfg->cmp, cfg->swap);
	return 0;
}

void *visit(void *data, int id)
{
	P_VALID_CHECK_RET(data, NULL);
	return &((int*)data)[id];
}

int cmp(void *l, void *r) 
{
	int res = 0;
	P_VALID_CHECK_RET(l, -1);
	P_VALID_CHECK_RET(r, -1);
	res	= *((int *)l) -  *((int *)r);
	if (res == 0)
		return CMP_RES_EQUAL;
	else if (res > 0)
		return CMP_RES_GREATER;
	else
		return CMP_RES_LESS;
	return -1;
}

int swap(void *l, void *r) 
{
	int tmp	= 0;

	P_VALID_CHECK_RET(l, -1);
	P_VALID_CHECK_RET(r, -1);
	if (l == r)
		return 0;
	tmp	= *((int *)l);
	*((int *)l)	= *((int *)r);
	*((int *)r)	= tmp;
	return 0;
}

void array_print(int *data, int num)
{
	int i = 0;
	P_VALID_CHECK_ACT(data, return);
	for (i = 0; i < num; i++) {
		printf("%d ", data[i]);
	}
	printf("\n");
}

int main(void) 
{
	int i = 0, data[NUM] = {0};
	Sort_Cfg cfg = {
		.visit	= visit,
		.cmp	= cmp,
		.swap	= swap,
		.order	= 0,
		.num	= ARRAY_SIZE_GET(data),
		.data	= data,
	};

	// generate random numbers.
	DEBUG("Generate %d numbers to sort:\n", NUM);
	for (i = 0; i < ARRAY_SIZE_GET(data); i++) {
		data[i]	= my_rand(0, 100);
		printf("%d ", data[i]);
	}
	printf("\n");
	quick_sort(&cfg);
	DEBUG("After sort:\n");
	array_print(data, ARRAY_SIZE_GET(data));
	return 0;
}	
