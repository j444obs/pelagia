/* lvm.h
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
#ifndef __LVM_H
#define __LVM_H

#define SHELLING(n) (n - 1)
#define FillFun(h, n, r)n p##n = plg_LvmCheckSym(h, #n);if (!p##n) {return r;}

void* plg_LvmLoad(const char *path);
void plg_LvmDestory(void* plVMHandle);
int plg_LvmCallFile(void* plVMHandle, char* file, char* fun, void* value, short len);
void* plg_LvmCheckSym(void *lib, const char *sym);
void* plg_LvmGetInstance(void* plVMHandle);
void* plg_LvmGetL(void* plVMHandle);
void* plg_LvmMallocForBuf(void* p, int len, char type);
void* plg_LvmMallocWithType(void* L, void* instance, int nArg, size_t* len);
#endif