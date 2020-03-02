/* lvm.c - load lua
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
#include "plvm.h"
#include "plauxlib.h"
#include "pelog.h"
#include "plibsys.h"
#include "plualib.h"
#include "plua.h"

typedef struct _lVMHandle
{
	void* hInstance;
	lua_State *luaVM;
}*PlVMHandle, lVMHandle;

void* plg_LvmGetInstance(void* pvlVMHandle) {
	PlVMHandle plVMHandle = pvlVMHandle;
	return plVMHandle->hInstance;
}

void* plg_LvmGetL(void* pvlVMHandle) {
	PlVMHandle plVMHandle = pvlVMHandle;
	return plVMHandle->luaVM;
}

void* plg_LvmCheckSym(void *lib, const char *sym) {

	if (!lib) {
		elog(log_error, "plg_LvmCheckSym.lib");
		return 0;
	}

	void* fun = plg_SysLibSym(lib, sym);
	if (!fun) {
		elog(log_error, "plg_LvmCheckSym.plg_SysLibSym:%s", sym);
		return 0;
	} else {
		return fun;
	}
}

#define NORET
#define FillFun(h, n, r)n p##n = plg_LvmCheckSym(h, #n);if (!p##n) {return r;}

void* plg_LvmLoad(const char *path) {

	void* hInstance = plg_SysLibLoad(path, 0);
	if (hInstance == NULL) {
		elog(log_error, "plg_LvmLoad.plg_SysLibLoad:%s", path);
		return 0;
	}

	luaL_newstate funNewstate = (luaL_newstate)plg_SysLibSym(hInstance, "luaL_newstate");
	if (!funNewstate) {
		elog(log_error, "plg_LvmLoad.plg_SysLibSym.luaL_newstate");
		return 0;
	}

	lua_State *luaVM = (*funNewstate)();

	luaL_openlibs funOpenlibs = (luaL_openlibs)plg_SysLibSym(hInstance, "luaL_openlibs");
	if (!funOpenlibs) {
		elog(log_error, "plg_LvmLoad.plg_SysLibSym.luaL_openlibs");
		return 0;
	}

	funOpenlibs(luaVM);

	PlVMHandle plVMHandle = malloc(sizeof(lVMHandle));
	plVMHandle->hInstance = hInstance;
	plVMHandle->luaVM = luaVM;
	return plVMHandle;
}

void plg_LvmDestory(void* pvlVMHandle) {

	PlVMHandle plVMHandle = pvlVMHandle;
	if (plVMHandle == 0) {
		return;
	}

	lua_close funClose = (lua_close)plg_SysLibSym(plVMHandle->hInstance, "lua_close");
	if (!funClose) {
		elog(log_error, "plg_LvmLoad.plg_SysLibSym.lua_close");
	}

	funClose(plVMHandle->luaVM);

	plg_SysLibUnload(plVMHandle->hInstance);
	free(plVMHandle);
}

int plg_LvmCallFile(void* pvlVMHandle, char* file, char* fun, void* value, short len) {

	PlVMHandle plVMHandle = pvlVMHandle;
	FillFun(plVMHandle->hInstance, lua_getfield, 0);
	FillFun(plVMHandle->hInstance, luaL_loadfilex, 0);
	FillFun(plVMHandle->hInstance, lua_pcall, 0);
	FillFun(plVMHandle->hInstance, lua_pushlstring, 0);
	FillFun(plVMHandle->hInstance, lua_isnumber, 0);
	FillFun(plVMHandle->hInstance, lua_tonumber, 0);
	FillFun(plVMHandle->hInstance, lua_settop, 0);
	FillFun(plVMHandle->hInstance, lua_tolstring, 0);

	if (pluaL_loadfilex(plVMHandle->luaVM, file, NULL)){
		elog(log_error, "plg_LvmCallFile.pluaL_loadfilex:%s", plua_tolstring(plVMHandle->luaVM, -1, NULL));
		plua_settop(plVMHandle->luaVM, -2);
		return 0;
	}

	//load fun
	if (plua_pcall(plVMHandle->luaVM, 0, LUA_MULTRET, 0)) {
		printf("\nFATAL ERROR:%s\n\n", plua_tolstring(plVMHandle->luaVM, -1, NULL));
		elog(log_error, "plg_LvmCallFile.plua_pcall:%s lua:%s", file, plua_tolstring(plVMHandle->luaVM, -1, NULL));
		plua_settop(plVMHandle->luaVM, -2);
		return 0;
	}

	//call fun
	plua_getfield(plVMHandle->luaVM, LUA_GLOBALSINDEX, fun);
	plua_pushlstring(plVMHandle->luaVM, value, len);

	if (plua_pcall(plVMHandle->luaVM, 1, LUA_MULTRET, 0)) {
		elog(log_error, "plg_LvmCallFile.plua_pcall:%s lua:%s", fun, plua_tolstring(plVMHandle->luaVM, -1, NULL));
		plua_settop(plVMHandle->luaVM, -2);
		return 0;
	}

	double ret = 0;
	if (plua_isnumber(plVMHandle->luaVM, -1))
	{
		ret = plua_tonumber(plVMHandle->luaVM, -1);
	}

	//lua_settop(L, -(n)-1)
	plua_settop(plVMHandle->luaVM, -2);
	return ret;
}

void* plg_LvmMallocForBuf(void* p, int len, char type) {
	char* r = 0;
	r = malloc(len + 1);
	r[0] = type;
	memcpy((r + 1), p, len);
	return r;
}

void* plg_LvmMallocWithType(void* vL, void* instance, int nArg, size_t* len) {

	lua_State* L = vL;
	FillFun(instance, lua_type, 0);
	FillFun(instance, luaL_checknumber, 0);
	FillFun(instance, luaL_checklstring, 0);

	int t = plua_type(L, nArg);
	char* p = 0;

	if (t == LUA_TNUMBER) {
		*len = sizeof(lua_Number) + 1;
		p = malloc(*len);
		lua_Number r = pluaL_checknumber(L, nArg);
		memcpy((p + 1), (char*)&r, sizeof(lua_Number));
		p[0] = LUA_TNUMBER;

		return p;
	} else if (t == LUA_TSTRING) {
		size_t sLen;
		const char* s = pluaL_checklstring(L, nArg, &sLen);
		*len = sLen + 1;
		p = malloc(*len);

		memcpy((p + 1), s, sLen);
		p[0] = LUA_TSTRING;

		return p;
	}

	return 0;
}


#undef FillFun
#undef NORET