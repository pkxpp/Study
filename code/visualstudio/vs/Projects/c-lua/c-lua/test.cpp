// test.c

//

// c++
extern "C"
{
//#include "D:\\Program Files (x86)\\Lua\\5.1\\include\\lua.h"
//#include "D:\\Program Files (x86)\\Lua\\5.1\\include\\lauxlib.h"
//#include "D:\\Program Files (x86)\\Lua\\5.1\\include\\lualib.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}



//lib
//#pragma comment( lib, "D:\\Program Files (x86)\\Lua\\5.1\\lib\\lua5.1.lib")

// global variables

lua_State *L;

double f( double x, double y){
	double ret;
	lua_getglobal( L, "f");
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_call( L, 2, 1);
	ret = lua_tonumber(L, -1);
	lua_pop( L, 1);
	return ret;
}

// main
int main(void){
	int error;
	L = lua_open();
	luaopen_base(L);
	luaL_openlibs(L);
//	error = luaL_loadfile(L, "test1.lua");
	luaL_loadfile(L, "test1.lua");  
	lua_pcall(L, 0, LUA_MULTRET, 0);
	double ret = f( 10, 3.4);
	printf( "ret = %f\n", ret);
	getchar();
	lua_close(L);
	return 1;
}