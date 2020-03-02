/* listdict.h - for cache
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

#ifndef __LISTDICT_H
#define __LISTDICT_H
typedef int(*LTCMPFUN)(void*, void*);
typedef struct _ListDict {
	list* list;
	dict* dict;
	LTCMPFUN cfun;
}*PListDict, ListDict;

void* plg_ListDictCreateHandle(dictType *type, short dictPool, short listPool, LTCMPFUN cfun, void* privateDate);
void plg_ListDictDestroyHandle(void* pListDict);
int plg_ListDictAdd(void* pListDict, void* key, void* value);
void plg_ListDictDel(void* pListDict, void* key);
dict* plg_ListDictDict(void* pListDict);
list* plg_ListDictList(void* pListDict);
list* plg_ListDictSortList(void* pListDict);
void* plg_ListDictGetVal(dictEntry * entry);
void plg_ListDictEmpty(void* pListDict);

#define listdictAddWithUint(d, key, val) do {\
	unsigned int* mkey = malloc(sizeof(unsigned int));\
	*mkey = key;\
	if (DICT_ERR == plg_ListDictAdd(d, mkey, val)) {\
		free(mkey);\
					}\
} while (0);

#endif