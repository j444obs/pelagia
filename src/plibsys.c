/* libsys.c - Lib system related
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
#include "plibsys.h"

#ifdef _WIN32
void plg_SysLibUnload(void *lib) {
	FreeLibrary((HMODULE)lib);
}

void* plg_SysLibLoad(const char *path, int seeglb) {
	return LoadLibraryExA(path, NULL, 0);
}

void* plg_SysLibSym(void *lib, const char *sym) {
	return GetProcAddress((HMODULE)lib, sym);
}
#else
#include <dlfcn.h>
void plg_SysLibUnload(void *lib) {
	dlclose(lib);
}

void* plg_SysLibLoad(const char *path, int seeglb) {
	return dlopen(path, RTLD_NOW | (seeglb ? RTLD_GLOBAL : RTLD_LOCAL));
}

void* plg_SysLibSym(void *lib, const char *sym) {
	return dlsym(lib, sym);
}
#endif // WIN32