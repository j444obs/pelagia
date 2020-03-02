/* memorylist.c - Specific memory manager
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

#ifndef __MEMORYPOOL_H
#define __MEMORYPOOL_H

void* plg_MemPoolCreate(unsigned int size, unsigned int count);
void plg_MemPoolDestroy(void* pMemoryPoolHandle);
void* plg_MemPoolMalloc(void*  pMemoryPoolHandle);
void plg_MemPoolFree(void* pMemoryPoolHandle, void* ptr);

#endif