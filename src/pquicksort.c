/* quicksort.c - Fast sorting of adlist
*
* Copyright(C) 2019 - 2020, sun shuo <sun.shuo@surparallel.org>
* All rights reserved.
*
* This program is free software : you can redistribute it and / or modify
* it under the terms of the GNU Affero General Public License as
* published by the Free Software Foundation, either version 3 of the
* License, or(at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program.If not, see < https://www.gnu.org/licenses/>.
*/

#include "plateform.h"
#include "padlist.h"
#include "psds.h"
#include "pquicksort.h"

int plg_SortDefaultUshortPtrCmp(void* vv1, void* vv2) {

	unsigned short** v1 = vv1;
	unsigned short** v2 = vv2;
	if (*v1 == 0) {
		return 1;
	} else if (*v2 == 0) {
		return -1;
	}

	if (**v1 > **v2) {
		return -1;
	} else if (**v1 == **v2) {
		return 0;
	} else {
		return 1;
	}
}

int plg_SortDefaultSdsCmp(sds* v1, sds* v2) {
	if (plg_sdsLen(*v1) > plg_sdsLen(*v2)) {
		return 1;
	} else if (plg_sdsLen(*v1) == plg_sdsLen(*v2)) {
		return plg_sdsCmp(*v1, *v2);
	} else {
		return -1;
	}
}

int plg_SortDefaultUintCmp(unsigned int* v1, unsigned int* v2) {
	if (*v1 > *v2) {
		return 1;
	} else if (*v1 == *v2) {
		return 0;
	} else {
		return -1;
	}
}

int plg_SortDefaultIntCmp(int* v1, int* v2) {
	if (*v1 > *v2) {
		return 1;
	} else if (*v1 == *v2) {
		return 0;
	} else {
		return -1;
	}
}

void ListQuicksort(struct listNode **head, struct listNode **tail, CMPFUN cfun)
{
	if (*head == NULL) {
		return;
	}
	if (*head == *tail) {
		return;
	}

	//init
	struct listNode *pivot = *head;
	struct listNode *currentN = (*head)->next;
	struct listNode *rightTail = NULL;
	struct listNode *leftTail = NULL;
	struct listNode *left = NULL;
	struct listNode *right = NULL;
	int loop = 1;

	pivot->next = NULL;
	pivot->prev = NULL;
	
	do {
		if (currentN == *tail) {
			loop = 0;
		}

		if (cfun(currentN->value, pivot->value) > 0) {
			if (right == NULL) {
				right = currentN;
				currentN = currentN->next;
				right->next = NULL;
				right->prev = NULL;
				rightTail = right;
			} else{
				rightTail->next = currentN;
				currentN = currentN->next;
				rightTail->next->next = NULL;
				rightTail->next->prev = rightTail;
				rightTail = rightTail->next;
			}
		} else{
			if (left == NULL) {
				left = currentN;
				currentN = currentN->next;
				left->next = NULL;
				left->prev = NULL;
				leftTail = left;
			} else{
				leftTail->next = currentN;
				currentN = currentN->next;
				leftTail->next->next = NULL;
				leftTail->next->prev = leftTail;
				leftTail = leftTail->next;
			}
		}
		
		if (loop == 0) {
			break;
		}

		if (currentN == *tail) {
			loop -= 1;
		}
	} while (1);

	ListQuicksort(&left, &leftTail, cfun);
	ListQuicksort(&right, &rightTail, cfun);

	//pivot add to left

	if (left != NULL) {
		*head = left;
		*tail = leftTail;
	}

	if (pivot != NULL) {
		if (left == NULL) {
			*head = pivot;
			*tail = pivot;
		} else {
			(*tail)->next = pivot;
			pivot->prev = *tail;
			*tail = pivot;
		}
	}

	if (right != NULL) {
		(*tail)->next = right;
		right->prev = *tail;
		*tail = rightTail;
	}

	return;
}

void plg_SortList(struct list *list, CMPFUN cfun) {

	//must be
	if (cfun == NULL) {
		return;
	}

	//set and start
	ListQuicksort(&list->head, &list->tail, cfun);
}

//arrary quicksort
void ArrarySwap(char* array, int length, int left, int right, void* temp) {
	memcpy(temp, array + length * left, length);
	memcpy(array + length * left, array + length * right, length);
	memcpy(array + length * right, temp, length);
}

int ArraryPartition(char* array, int length, int left, int right, int pivot_index, void* pivot_value, void* temp, CMPFUN cfun)
{
	memcpy(pivot_value, array + pivot_index * length, length);
	int store_index = left;
	int i;

	ArrarySwap(array, length, pivot_index, right, temp);
	for (i = left; i < right; i++)
		if (cfun(array + i * length, pivot_value) <= 0) {
			ArrarySwap(array, length, i, store_index, temp);
			++store_index;
		}
	ArrarySwap(array, length, store_index, right, temp);
	return store_index;
}

void ArraryQuicksort(char* array, int length, int left, int right, void* pivot_value, void* temp, CMPFUN cfun)
{
	int pivot_index = left;
	int pivot_new_index;

loop:
	if (right > left) {
		pivot_new_index = ArraryPartition(array, length, left, right, pivot_index, pivot_value, temp, cfun);
		ArraryQuicksort(array, length, left, pivot_new_index - 1, pivot_value, temp, cfun);
		pivot_index = left = pivot_new_index + 1;
		goto loop;
	}
}

void plg_SortArrary(void* array, int length, int size, CMPFUN cfun) {
	//must be
	if (cfun == NULL) {
		return;
	}

	void* temp = malloc(length);
	void* pivot_value = malloc(length);
	
	//set and start
	ArraryQuicksort(array, length, 0, size - 1, pivot_value, temp, cfun);

	free(pivot_value);
	free(temp);
}

/*
int cmp(int* v1, int* v2) {
	if (*v1 > *v2) {
		return 1;
	}else if (*v1 == *v2) {
		return 0;
	} else {
		return -1;
	}
}

int main(int argc, char *argv[])
{
int length = 10;
int array[length] = {0}
plg_SortArrary(array, sizeof(int), 0, length - 1, cmp);
return 0;
}
*/