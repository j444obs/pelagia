/* disk.h
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
#ifndef __CACHE_H
#define __CACHE_H

//API
void* plg_CacheCreateHandle(void* pDiskHandle);
void plg_CacheDestroyHandle(void* pvCacheHandle);

//safe api for run
unsigned int plg_CacheTableAdd(void* pvCacheHandle, char* sdsTable, char* sdsKey, void* value, unsigned int length);
unsigned int plg_CacheTableDel(void* pvCacheHandle, char* sdsTable, char* sdsKey);
int plg_CacheTableFind(void* pvCacheHandle, char* sdsTable, char* sdsKey, void* pDictExten, short recent);
unsigned int plg_CacheTableLength(void* pvCacheHandle, char* sdsTable, short recent);
unsigned int plg_CacheTableAddIfNoExist(void* pvCacheHandle, char* sdsTable, char* sdsKey, void* value, unsigned int length);
unsigned int plg_CacheTableIsKeyExist(void* pvCacheHandle, char* sdsTable, char* sdsKey, short recent);
unsigned int plg_CacheTableRename(void* pvCacheHandle, char* sdsTable, char* sdsKey, char* sdsNewKey);
void plg_CacheTableLimite(void* pvCacheHandle, char* sdsTable, char* sdsKey, unsigned int left, unsigned int right, void* pDictExten, short recent);
void plg_CacheTableOrder(void* pvCacheHandle, char* sdsTable, short order, unsigned int limite, void* pDictExten, short recent);
void plg_CacheTableRang(void* pvCacheHandle, char* sdsTable, char* sdsBeginKey, char* sdsEndKey, void* pDictExten, short recent);
void plg_CacheTablePattern(void* pvCacheHandle, char* sdsTable, char* sdsBeginKey, char* sdsEndKey, char* pattern, void* pDictExten, short recent);
unsigned int plg_CacheTableMultiAdd(void* pvCacheHandle, char* sdsTable, void* pDictExten);
void plg_CacheTableMultiFind(void* pvCacheHandle, char* sdsTable, void* pKeyDictExten, void* pValueDictExten, short recent);
unsigned int plg_CacheTableRand(void* pvCacheHandle, char* sdsTable, void* pDictExten, short recent);
void plg_CacheTableClear(void* pvCacheHandle, char* sdsTable);

//set
unsigned int plg_CacheTableSetAdd(void* pvCacheHandle, char* sdsTable, char* sdsKey, char* sdsValue);
void plg_CacheTableSetRang(void* pvCacheHandle, char* sdsTable, char* sdsKey, char* sdsBeginValue, char* sdsEndValue, void* pDictExten, short recent);
void plg_CacheTableSetLimite(void* pvCacheHandle, char* sdsTable, char* sdsKey, char* sdsValue, unsigned int left, unsigned int right, void* pDictExten, short recent);
unsigned int plg_CacheTableSetLength(void* pvCacheHandle, char* sdsTable, char* sdsKey, short recent);
unsigned int plg_CacheTableSetIsKeyExist(void* pvCacheHandle, char* sdsTable, char* sdsKey, char* sdsValue, short recent);
void plg_CacheTableSetMembers(void* pvCacheHandle, char* sdsTable, char* sdsKey, void* pDictExten, short recent);
unsigned int plg_CacheTableSetRand(void* pvCacheHandle, char* sdsTable, char* sdsKey, void* pDictExten, short recent);
void plg_CacheTableSetDel(void* pvCacheHandle, char* sdsTable, char* sdsKey, void* pValueDictExten);
unsigned int plg_CacheTableSetPop(void* pvCacheHandle, char* sdsTable, char* sdsKey, void* pDictExten, short recent);
unsigned int plg_CacheTableSetRangCount(void* pvCacheHandle, char* sdsTable, char* sdsKey, char* sdsBeginValue, char* sdsEndValue, short recent);
unsigned int plg_CacheTableSetUion(void* pvCacheHandle, char* sdsTable, void* pSetDictExten, void* pKeyDictExten, short recent);
unsigned int plg_CacheTableSetUionStore(void* pvCacheHandle, char* sdsTable, void* pSetDictExten, char* sdsKey);
unsigned int plg_CacheTableSetInter(void* pvCacheHandle, char* sdsTable, void* pSetDictExten, void* pKeyDictExten, short recent);
unsigned int plg_CacheTableSetInterStore(void* pvCacheHandle, char* sdsTable, void* pSetDictExten, char* sdsKey);
unsigned int plg_CacheTableSetDiff(void* pvCacheHandle, char* sdsTable, void* pSetDictExten, void* pKeyDictExten, short recent);
unsigned int plg_CacheTableSetDiffStore(void* pvCacheHandle, char* sdsTable, void* pSetDictExten, char* sdsKey);
unsigned int plg_CacheTableSetMove(void* pvCacheHandle, char* sdsTable, char* sdsSrcKey, char* sdsDesKey, char* sdsValue);

int plg_CacheCommit(void* pvCacheHandle);
int plg_CacheRollBack(void* pvCacheHandle);
void plg_CacheFlush(void* pvCacheHandle);

//config
void plg_CacheSetInterval(void* pvCacheHandle, unsigned int interval);
void plg_CacheSetPercent(void* pvCacheHandle, unsigned int percent);

unsigned int plg_CacheTableMembersWithJson(void* pvCacheHandle, char* sdsTable, void* jsonRoot, short recent);
#endif