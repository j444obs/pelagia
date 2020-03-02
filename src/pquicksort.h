/* quicksort.h - Fast sorting of adlist
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

#ifndef __QUICKSORT_H
#define __QUICKSORT_H

typedef int(*CMPFUN)(void*, void*);
void plg_SortList(struct list *list, CMPFUN cfun);
void plg_SortArrary(void* array, int length, int size, CMPFUN cfun);

int plg_SortDefaultUintCmp(unsigned int* v1, unsigned int* v2);
int plg_SortDefaultIntCmp(int* v1, int* v2);
int plg_SortDefaultSdsCmp(char** v1, char** v2);
int plg_SortDefaultUshortPtrCmp(void* vv1, void* v2);

#endif