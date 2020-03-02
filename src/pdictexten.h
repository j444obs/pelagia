/* dictExten.h - Return the data set composed of the dictExtens 
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

#ifndef __RESULT_H
#define __RESULT_H

//return pDictExten
void* plg_DictExtenCreate();
void* plg_DictExtenSubCreate(void* pDictExten, void* key, unsigned int keyLen);

void plg_DictExtenDestroy(void* pDictExten);
int plg_DictExtenAdd(void* pDictExten, void* key, unsigned int keyLen, void* value, unsigned int valueLen);
void plg_DictExtenDel(void* pDictExten, void* key, unsigned int keyLen);
void* plg_DictExtenSub(void* entry);
int plg_DictExtenSize(void* pDictExten);

//create iterator
void* plg_DictExtenGetIterator(void* pDictExten);
void plg_DictExtenReleaseIterator(void* iter);

//return entry
void* plg_DictExtenFind(void* pDictExten, void* key, unsigned int keyLen);
void* plg_DictExtenNext(void* iter);
void* plg_DictExtenGetHead(void* pDictExten);
int plg_DictExtenIsSub(void* entry);

//reurn buffer
void* plg_DictExtenValue(void* entry, unsigned int *valueLen);
void* plg_DictExtenKey(void* entry, unsigned int *keyLen);

//help for 'C' ansi
void* plg_DictExtenSubCreateForChar(void* pDictExten, char* key);
int plg_DictExtenAddForChar(void* pDictExten, char* key, void* value, unsigned int valueLen);
void plg_DictExtenDelForChar(void* pDictExten, char* key);
void* plg_DictExtenFindForChar(void* pDictExten, char* key);
int plg_DictExtenAddForCharWithInt(void* pDictExten, char* key, int value);
int plg_DictExtenAddForCharWithUInt(void* pDictExten, char* key, unsigned int value);
int plg_DictExtenAddForCharWithShort(void* pDictExten, char* key, short value);
int plg_DictExtenAddForCharWithLL(void* pDictExten, char* key, long long value);
int plg_DictExtenAddForCharWithDouble(void* pDictExten, char* key, double value);

#endif