/* cmp.c - hash cmp
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
#include <string.h>
#include "pcmp.h"

/* Determine whether id1 or id2 is closer to ref */
int plg_XorCmp(const unsigned char *id1, const unsigned char *id2, const unsigned char *ref) {
	int i;
	for (i = 0; i < 20; i++) {
		unsigned char xor1, xor2;
		if (id1[i] == id2[i])
			continue;
		xor1 = id1[i] ^ ref[i];
		xor2 = id2[i] ^ ref[i];
		if (xor1 < xor2)
			return -1;
		else
			return 1;
	}
	return 0;
}

int plg_HashCmp(const unsigned char * id1, const unsigned char * id2) {
	/* Memcmp is guaranteed to perform an unsigned comparison. */
	return memcmp(id1, id2, 20);
}