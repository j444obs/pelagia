/* memorypool.h - Specific memory manager
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
#include "pmemorypool.h"
#include "pbitarray.h"

typedef struct _MemoryPoolHead
{
	void* next;
	unsigned int length;
	unsigned char bit[];
}*PMemoryPoolHead, MemoryPoolHead;

typedef struct _MemoryPoolHandle
{
	PMemoryPoolHead head;
	unsigned int size;
	unsigned int count;
}*PMemoryPoolHandle, MemoryPoolHandle;

void* plg_MemPoolCreate(unsigned int size, unsigned int count) {
	PMemoryPoolHandle pMemoryPoolHandle = (PMemoryPoolHandle)calloc(1, sizeof(MemoryPoolHandle));
	pMemoryPoolHandle->size = size;
	pMemoryPoolHandle->count = count;
	return pMemoryPoolHandle;
}

void plg_MemPoolDestroy(void* pvMemoryPoolHandle) {

	PMemoryPoolHandle pMemoryPoolHandle = pvMemoryPoolHandle;
	PMemoryPoolHead pMemoryPoolHead = (PMemoryPoolHead)pMemoryPoolHandle->head;
	do {
		if (pMemoryPoolHead == 0) {
			break;
		}
		void* next = pMemoryPoolHead->next;
		free(pMemoryPoolHead);
		pMemoryPoolHead = (PMemoryPoolHead)next;
	} while (1);
	free(pMemoryPoolHandle);
}

void* plg_MemPoolMalloc(void* pvMemoryPoolHandle) {

	//find
	PMemoryPoolHandle pMemoryPoolHandle = pvMemoryPoolHandle;
	PMemoryPoolHead pMemoryPoolHead = (PMemoryPoolHead)pMemoryPoolHandle->head;
	unsigned int headLength = sizeof(MemoryPoolHead) + pMemoryPoolHandle->count / 8 + (pMemoryPoolHandle->count % 8 ? 1 : 0);
	unsigned int bitSize = pMemoryPoolHandle->count / 8 + (pMemoryPoolHandle->count % 8 ? 1 : 0);
	unsigned int bitLength;
	if (pMemoryPoolHead != 0) {
		bitLength = pMemoryPoolHead->length / 8 + (pMemoryPoolHead->length % 8 ? 1 : 0);
	}

	do {
		if (pMemoryPoolHead == 0) {
			break;
		}

		if (pMemoryPoolHandle->count - pMemoryPoolHead->length > 0) {
			for (unsigned int i = bitLength; i < bitSize; i++) {
				if (pMemoryPoolHead->bit[i] != UCHAR_MAX) {
					for (unsigned int l = i * 8; l < (i + 1) * 8; l++) {
						if (l < pMemoryPoolHandle->count && plg_BitArrayIsIn(pMemoryPoolHead->bit, l) == 0) {
							void* cur = (unsigned char*)pMemoryPoolHead + headLength;
							plg_BitArrayAdd(pMemoryPoolHead->bit, l);
							pMemoryPoolHead->length += 1;
							return (unsigned char*)cur + pMemoryPoolHandle->size * l;
						}
					}
				}
			}

			for (unsigned int i = 0; i < bitLength; i++) {
				if (pMemoryPoolHead->bit[i] != UCHAR_MAX) {
					for (unsigned int l = i * 8; l < (i + 1) * 8; l++) {
						if (l < pMemoryPoolHandle->count && plg_BitArrayIsIn(pMemoryPoolHead->bit, l) == 0) {
							void* cur = (unsigned char*)pMemoryPoolHead + headLength;
							plg_BitArrayAdd(pMemoryPoolHead->bit, l);
							pMemoryPoolHead->length += 1;
							return (unsigned char*)cur + pMemoryPoolHandle->size * l;
						}
					}
				}
			}
		}

		pMemoryPoolHead = (PMemoryPoolHead)pMemoryPoolHead->next;
	} while (1);

	//create
	unsigned int pageLength = headLength + pMemoryPoolHandle->count * pMemoryPoolHandle->size;
	pMemoryPoolHead = malloc(pageLength);
	PMemoryPoolHead ptrHead = pMemoryPoolHandle->head;
	pMemoryPoolHead->next = ptrHead;
	pMemoryPoolHead->length = 0;
	pMemoryPoolHandle->head = pMemoryPoolHead;

	void* ptr = (unsigned char*)pMemoryPoolHead + headLength;
	plg_BitArrayAdd(pMemoryPoolHead->bit, 0);
	pMemoryPoolHead->length += 1;
	return (unsigned char*)ptr;
}

void plg_MemPoolFree(void* pvMemoryPoolHandle, void* ptr) {

	//find
	PMemoryPoolHandle pMemoryPoolHandle = pvMemoryPoolHandle;
	PMemoryPoolHead pMemoryPoolHead = (PMemoryPoolHead)pMemoryPoolHandle->head;
	unsigned int headLength = sizeof(MemoryPoolHead) + pMemoryPoolHandle->count / 8 + (pMemoryPoolHandle->count % 8 ? 1 : 0);
	unsigned int pageLength = headLength + pMemoryPoolHandle->size * pMemoryPoolHandle->count;
	PMemoryPoolHead ptrPrev = 0;
	do {
		if (pMemoryPoolHead == 0) {
			break;
		}

		void* pageEnd = (unsigned char*)pMemoryPoolHead + pageLength;
		if (ptr >= (void*)pMemoryPoolHead && ptr < pageEnd) {
			unsigned int cur = ((unsigned char*)ptr - (unsigned char*)pMemoryPoolHead - headLength) / pMemoryPoolHandle->size;
			plg_BitArrayClear(pMemoryPoolHead->bit, cur);
			pMemoryPoolHead->length -= 1;
			break;
		}
		ptrPrev = pMemoryPoolHead;
		pMemoryPoolHead = (PMemoryPoolHead)pMemoryPoolHead->next;
	} while (1);

	if (pMemoryPoolHead) {
		if (pMemoryPoolHead->length == 0) {
			if (ptrPrev != 0) {
				ptrPrev->next = pMemoryPoolHead->next;
			} else {
				pMemoryPoolHandle->head = pMemoryPoolHead->next;
			}

			free(pMemoryPoolHead);
		}
	}
}