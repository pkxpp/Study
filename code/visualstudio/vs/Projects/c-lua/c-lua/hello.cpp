/*
 * hello.cpp
 * 
 * 
 * Author:
 */

#include <iostream>
#include <cstdio>
#include <string>


using namespace std;

//#include "lua.hpp"
#ifdef _USE_LUA_JIT
#include "..\\include\\luajit\\lua.hpp"

//#pragma comment( lib, "..\\lib\\luajit\\luajit.lib")
//#pragma comment( lib, "..\\lib\\luajit\\lua51.lib")
//#pragma comment( lib, "F:\\study\\lua\\LuaJIT-2.0.5\\src\\luajit.lib")
#else
#include "..\\include\\lua515\\lua.hpp"

//#ifdef _DEBUG
//#pragma comment( lib, "..\\lib\\lua515\\Lua515D.lib")
//#else
//#pragma comment( lib, "..\\lib\\lua515\\Lua515.lib")
//#endif //_DEBUG

#endif // _USE_LUA_JIT

struct info{
	string name;
	int age;
};

typedef struct info info;
 
int main(int argc, char **argv){
	int status;
	string RootPath, NewFilePath, OldFilePath, ResultFileName;
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	//lua_State *L = lua_open();

	// lua math.floor(19.9 * 100) = 1989
	int a = floor(19.9 * 100);

	/* Load the file containing the script we are going to run */
	//status = luaL_loadfile(L, "script/main.lua");
	status = luaL_loadfile(L, "script/coroutine_xpcall.lua");
	if (status) {
		/* If something went wrong, error message is at the top of */
		/* the stack */
		fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
		exit(1);
	}
	status = lua_pcall(L, 0, 0, 0);

	//info employee;
	//status = luaL_dofile(L, "./test.lua");
	//if (status) {
	//	/* If something went wrong, error message is at the top of */
	//	/* the stack */
	//	fprintf(stderr, "Couldn't do file: %s\n", lua_tostring(L, -1));
	//	exit(1);
	//}

//	lua_getglobal(L, "name");
//	lua_getglobal(L, "age");

//	employee.name = lua_tostring(L, -2);
//	employee.age = lua_tonumber(L, -1);

//	std::cout << employee.name << endl;
//	std::cout << employee.age << endl;

	//luaL_dofile(L, "main.lua");
	//lua_getglobal(L, "RootPath");
	//lua_getglobal(L, "NewFilePath");
	//lua_getglobal(L, "OldFilePath");
	//lua_getglobal(L, "ResultFileName");

	//RootPath = lua_tostring(L, -4);
	//NewFilePath = lua_tostring(L, -3);
	//OldFilePath = lua_tostring(L, -2);
	//ResultFileName = lua_tostring(L, -1);

	//cout << RootPath << endl;
	//cout << NewFilePath << endl;
	//cout << OldFilePath << endl;
	//cout << ResultFileName << endl;

	getchar();
	return 0;
}

//static int l_sin(lua_State *L){
//	double d = lua_tonumber(L, 1);
//	lua_pushnumber(L, sin(d));
//	return 1;
//}

//向下取整
//double my_floor(double x)
//{
//	register double ret;
//	unsigned short int temp1, temp2;
//
//	__asm__("fnstcw %0" : "=m" (temp1));
//	temp2 = (temp1 & 0xf3ff) | 0x0400; /* rounding down */
//	__asm__("fldcw %0" : : "m" (temp2));
//	__asm__("frndint" : "=t" (ret) : "0" (x));
//	__asm__("fldcw %0" : : "m" (temp1));
//
//	return ret;
//}