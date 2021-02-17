/*
 * hello.cpp
 * 
 * 
 * Author:
 */

#include <iostream>
#include <cstdio>
#include <string>
#include "lua.hpp"

using namespace std;

//#pragma comment( lib, "D:\\Program Files (x86)\\Lua\\5.1\\lib\\lua5.1.lib")

struct info{
	string name;
	int age;
};

typedef struct info info;
 
int main(int argc, char **argv){
	string RootPath, NewFilePath, OldFilePath, ResultFileName;
//	lua_State* L = luaL_newstate();
	lua_State *L = lua_open();
	luaL_openlibs(L);
	info employee;
//	luaL_dofile(L, "./test.lua");
//	lua_getglobal(L, "name");
//	lua_getglobal(L, "age");

//	employee.name = lua_tostring(L, -2);
//	employee.age = lua_tonumber(L, -1);

//	std::cout << employee.name << endl;
//	std::cout << employee.age << endl;

	luaL_dofile(L, "./Config.lua");
	lua_getglobal(L, "RootPath");
	lua_getglobal(L, "NewFilePath");
	lua_getglobal(L, "OldFilePath");
	lua_getglobal(L, "ResultFileName");

	RootPath = lua_tostring(L, -4);
	NewFilePath = lua_tostring(L, -3);
	OldFilePath = lua_tostring(L, -2);
	ResultFileName = lua_tostring(L, -1);

	cout << RootPath << endl;
	cout << NewFilePath << endl;
	cout << OldFilePath << endl;
	cout << ResultFileName << endl;

	getchar();
	return 0;
}

static int l_sin(lua_State *L){
	double d = lua_tonumber(L, 1);
	lua_pushnumber(L, sin(d));
	return 1;
}