/*test_prfesa.c pseudo random finite element simulation analysis
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "prfesa.h"
#include "pelagia.h"
#include "ptimesys.h"

typedef struct _Param
{
	void* pEvent;
	short i;
	short dmage;
	short o;
}*PParam, Param;

static int InitRouting(char* value, short valueLen) {

	valueLen += 0;
	PParam pParam = (PParam)value;
	printf("---InitRouting--%d-\n", pParam->i);
	
	char table[256] = { 0 };
	sprintf(table, "t%d", pParam->i);

	short count = 100;
	plg_JobSet(table, strlen(table), "count", strlen("count"), &count, sizeof(short));

	return 1;
}

static int TestRouting(char* value, short valueLen) {
	
	//routing valueLen unused parameter 
	valueLen += 0;
	PParam pParam = (PParam)value;

	for (int i = 0; i < 10; i++)
	{
		short count;
		char table[10] = { 0 };
		sprintf(table, "t%d", i);

		unsigned int len = 0;
		void* ptr = plg_JobGet(table, strlen(table), "count", strlen("count"), &len);

		if (ptr) {
			count = *(short*)ptr;
			free(ptr);

			if (count < 0) {
				plg_EventSend(pParam->pEvent, NULL, 0);
				printf("%d job all pass!\n", pParam->o);
				return 1;
			}
		}
	}

	short count = 0;
	char table[10] = { 0 };
	sprintf(table, "t%d", pParam->i);

	unsigned int len = 0;
	void* ptr = plg_JobGet(table, strlen(table), "count", strlen("count"), &len);
	
	if (ptr) {
		count = *(short*)ptr;
		free(ptr);

		count -= pParam->dmage;	
		plg_JobSet(table, strlen(table), "count", strlen("count"), &count, sizeof(short));
		if (count < 0) {
			//all pass
			plg_EventSend(pParam->pEvent, NULL, 0);
			printf("%d job all pass!\n", pParam->o);
		} else {
			
			int l = rand();
			int c = 0;
			for (int i = 0; i < 10; i++) {
				c = l % 10;
				if (pParam->i == c) {
					continue;
				} else {
					break;
				}
			}

			char order[10] = { 0 };
			sprintf(order, "o%d", c);
			pParam->i = c;
			pParam->dmage = rand() % 1?2:5;
			plg_JobRemoteCall(order, strlen(order), (char*)pParam, sizeof(Param));
		}
	}
	
	printf("---TestRouting--%d--%d--\n", pParam->i, count);
	sleep(0);
	return 1;
}

void PRFESA(void) {

	void* pManage = plg_MngCreateHandle(0, 0);
	void* pEvent = plg_EventCreateHandle();

	plg_MngFreeJob(pManage);

	for (int i = 0; i < 10; i++) {

		char order[10] = { 0 };
		sprintf(order, "i%d", i);
		plg_MngAddOrder(pManage, order, strlen(order), plg_JobCreateFunPtr(InitRouting));

		char table[10] = { 0 };
		sprintf(table, "t%d", i);
		plg_MngAddTable(pManage, order, strlen(order), table, strlen(table));
	}
	
	for (int i = 0; i < 10; i++) {

		char order[10] = { 0 };
		sprintf(order, "o%d", i);
		plg_MngAddOrder(pManage, order, strlen(order), plg_JobCreateFunPtr(TestRouting));

		char table[10] = { 0 };
		sprintf(table, "t%d", i);
		plg_MngAddTable(pManage, order, strlen(order), table, strlen(table));
	}

	plg_MngPrintAllStatus(pManage);
	plg_MngAllocJob(pManage, 1);
	plg_MngPrintAllJobStatus(pManage);
	plg_MngStarJob(pManage);
	printf("\n-----------------manage create-----------------\n");

	for (int i = 0; i < 10; i++) {

		char order[10] = { 0 };
		sprintf(order, "i%d", i);
		Param param;
		param.i = i;
		param.pEvent = pEvent;
		plg_MngRemoteCall(pManage, order, strlen(order), (char*)&param, sizeof(Param));
	}
	
	for (int i = 0; i < 10; i++) {

		char order[10] = { 0 };
		sprintf(order, "o%d", i);
		Param param;
		param.i = i;
		param.o = i;
		param.pEvent = pEvent;
		param.dmage = 1;
		plg_MngRemoteCall(pManage, order, strlen(order), (char*)&param, sizeof(Param));
	}

	printf("\n-----------------manage send o0-----------------\n");

	//Because it is not a thread created by ptw32, ptw32 new cannot release memory leak
	for (int i = 0; i < 10; i++) {
		plg_EventWait(pEvent);
	}
	
	sleep(3);

	unsigned int eventLen;
	void * ptr = plg_EventRecvAlloc(pEvent, &eventLen);
	plg_EventFreePtr(ptr);

	plg_EventDestroyHandle(pEvent);
	plg_MngDestoryHandle(pManage, 0, 0);
	printf("\n-----------------manage destroy!-----------------\n");
}
