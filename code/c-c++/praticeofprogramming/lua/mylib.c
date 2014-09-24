/* mylib.c */
#include <math.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

static int l_sin(lua_State *L)
{
	double d = luaL_checknumber(L, 1);
	lua_pushnumber(L, sin(d));
	return 1;	/* number of results */
}

static const struct luaL_Reg mylib [] = {
	{"lsin", l_sin},
	{NULL, 	 NULL},	/* must the end like this */
};

int luaopen_mylib(lua_State *L)
{
	//luaL_register(L, "mylib", mylib);
	luaL_openlib(L, "mylib", mylib, 0);
	return 1;
}
/* end of mylib.c */
