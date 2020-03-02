/* dictset.h - index structure
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

#ifndef __DICT_SET_H
#define __DICT_SET_H

typedef struct _DictSet
{
	void* dictSet;
	void* sType;
	short sPool;
	unsigned int size;
}*PDictSet, DictSet;

void* plg_DictSetCreate(void *type, short pool, void *sType, short sPool);
void plg_DictSetAdd(PDictSet pDictSet, void* key, void* value);
void plg_DictSetDelValue(PDictSet pDictSet, void* key, void* value);
unsigned char plg_DictSetIn(PDictSet pDictSet, void* key, void* value);
void* plg_DictSetValue(PDictSet pDictSet, void* key);
void plg_DictSetDel(PDictSet pDictSet, void* key);
void plg_DictSetDestroy(PDictSet pDictSet);
void plg_DictSetDup(PDictSet pDictSetDest, PDictSet pDictSetSrc);
unsigned int plg_DictSetSize(PDictSet pDictSet);
void* plg_DictSetDict(PDictSet pDictSet);
void plg_DictSetEmpty(PDictSet pDictSet);
#endif