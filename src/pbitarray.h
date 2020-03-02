/* pbitarray.h
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
#ifndef __BITARRAY_H
#define __BITARRAY_H

unsigned char* plg_BitArrayInit(unsigned int);
void plg_BitArrayAdd(unsigned char *, unsigned int);
int plg_BitArrayIsIn(unsigned char *, unsigned int);
void plg_BitArrayClear(unsigned char *, unsigned int);

#endif