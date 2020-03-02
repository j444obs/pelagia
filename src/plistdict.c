/* listdict.c - for cache
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
#include "pdict.h"
#include "plistdict.h"
#include "pquicksort.h"

void* plg_ListDictCreateHandle(dictType *type, short dictPool, short listPool, LTCMPFUN cfun, void* privateDate){
	PListDict pListDict = malloc(sizeof(ListDict));
	pListDict->list = plg_listCreate(listPool);
	pListDict->dict = plg_dictCreate(type, privateDate, dictPool);
	pListDict->cfun = cfun;
	return pListDict;
}

void plg_ListDictDestroyHandle(void* pvListDict) {
	PListDict pListDict = pvListDict;
	plg_dictRelease(pListDict->dict);
	plg_listRelease(pListDict->list);
	free(pListDict);
}

int plg_ListDictAdd(void* pvListDict, void* key, void* value){
	PListDict pListDict = pvListDict;
	dictEntry * entry = plg_dictFind(pListDict->dict, key);
	if (entry == 0) {
		listNode * node = plg_listAddNodeHead(pListDict->list, value);
		return plg_dictAdd(pListDict->dict, key, node);
	}
	return DICT_ERR;
}

void plg_ListDictDel(void* pvListDict, void* key){
	PListDict pListDict = pvListDict;
	dictEntry * entry = plg_dictFind(pListDict->dict, key);
	if (entry != 0) {
		listNode* node = dictGetVal(entry);
		plg_dictDelete(pListDict->dict, dictGetKey(entry));
		plg_listDelNode(pListDict->list, node);
	}
}

dict* plg_ListDictDict(void* pvListDict) {
	PListDict pListDict = pvListDict;
	return pListDict->dict;
}

list* plg_ListDictList(void* pvListDict) {
	PListDict pListDict = pvListDict;
	return pListDict->list;
}

list* plg_ListDictSortList(void* pvListDict) {
	PListDict pListDict = pvListDict;
	plg_SortList(pListDict->list, pListDict->cfun);
	return pListDict->list;
}

void* plg_ListDictGetVal(dictEntry * entry) {
	listNode* node = dictGetVal(entry);
	return listNodeValue(node);
}

void plg_ListDictEmpty(void* pvListDict) {
	PListDict pListDict = pvListDict;
	plg_dictEmpty(pListDict->dict, NULL);
	plg_listEmpty(pListDict->list);
}