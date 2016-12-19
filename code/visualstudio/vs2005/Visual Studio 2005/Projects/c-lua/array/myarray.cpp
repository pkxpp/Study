#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <Error.h>
#include <lua.hpp>
extern "C" {
#include <lauxlib.h>
#include <lualib.h>
}

#pragma comment( lib, "D:\\Program Files (x86)\\Lua\\5.1\\lib\\lua5.1.lib")
//待注册的C函数，该函数的声明形式在上面的例子中已经给出。
//需要说明的是，该函数必须以C的形式被导出，因此extern "C"是必须的。
//函数代码和上例相同，这里不再赘述。
extern "C" int add(lua_State* L) 
{
	double op1 = luaL_checknumber(L,1);
	double op2 = luaL_checknumber(L,2);
	lua_pushnumber(L,op1 + op2);
	return 1;
}

extern "C" int sub(lua_State* L)
{
	double op1 = luaL_checknumber(L,1);
	double op2 = luaL_checknumber(L,2);
	lua_pushnumber(L,op1 - op2);
	return 1;
}

extern "C" int l_map(lua_State* L)
{
	int i, n;
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TFUNCTION);
	n = luaL_getn(L, 1);
	for (i=1; i <= n; i++)
	{
		lua_pushvalue(L,2);
		lua_rawgeti(L, 1, i);
		lua_call(L, 1, 1);
		lua_rawseti(L, 1, i);
	}
	return 0;
}

static int l_split(lua_State *L){
	const char *s = luaL_checkstring(L,1);
	const char *sep = luaL_checkstring(L, 2);
	const char *e;
	int i = 1;
	lua_newtable(L);
	while ((e = strchr(s, *sep)) != NULL)
	{
		lua_pushlstring(L, s, e-s);
		lua_rawseti(L, -2, i++);
		s = e + 1;
	}
	lua_pushstring(L, s);
	lua_rawseti(L, -2, i);
	lua_pushstring(L, s);

	return 1;

}

//27.3 闭包
static int counter(lua_State *L);
int newCounter(lua_State *L){
	lua_pushnumber(L, 0);
	lua_pushcclosure(L, &counter, 1);
	return 1;
}
static int counter(lua_State *L){
	double val = lua_tonumber(L, lua_upvalueindex(1));
	lua_pushnumber(L, ++val);
	lua_pushvalue(L, -1);
	lua_replace(L, lua_upvalueindex(1));
	return 1;
}

//luaL_Reg结构体的第一个字段为字符串，在注册时用于通知Lua该函数的名字。
//第一个字段为C函数指针。
//结构体数组中的最后一个元素的两个字段均为NULL，用于提示Lua注册函数已经到达数组的末尾。
/*
static luaL_Reg mylibs[] = { 
	{"add", add},
	{"sub", sub},
	{"map", l_map},
	{"split", l_split},
	{"newCounter", newCounter},
	{NULL, NULL} 
}; */

//该C库的唯一入口函数。其函数签名等同于上面的注册函数。见如下几点说明：
//1. 我们可以将该函数简单的理解为模块的工厂函数。
//2. 其函数名必须为luaopen_xxx，其中xxx表示library名称。Lua代码require "xxx"需要与之对应。
//3. 在luaL_register的调用中，其第一个字符串参数为模块名"xxx"，第二个参数为待注册函数的数组。
//4. 需要强调的是，所有需要用到"xxx"的代码，不论C还是Lua，都必须保持一致，这是Lua的约定，
//   否则将无法调用。
/*
extern "C" __declspec(dllexport)
int luaopen_mystudylib(lua_State* L) 
{
	const char* libName = "mystudylib";
	luaL_register(L,libName,mylibs);
	return 1;
}*/

/*
extern "C" __declspec(dllexport)
int luaopen_mystudylib(lua_State *L)
{
	//luaL_openlib(L, "mystudylib", mylibs, 0);
	luaL_register(L,"mystudylib",mylibs);
	return 1;
}*/

//////////////////////////////////////////////////////////////////////////
//2014/03/18/ 28 character
typedef struct NumArray{
	int size;
	double values[1];
}NumArray;

/*
static int newarray(lua_State *L)
{
	int n = luaL_checkint(L, 1);
	size_t nbytes = sizeof(NumArray) + (n-1)*sizeof(double);
	NumArray *a = (NumArray *)lua_newuserdata(L, nbytes);
	a->size = n;
	return 1;
}

static int setarray(lua_State *L){
	NumArray *a = (NumArray *)lua_touserdata(L, 1);
	int index = luaL_checkint(L, 2);
	double nValue = luaL_checknumber(L, 3);

	luaL_argcheck(L, a != NULL, 1, "'array' expected");
	luaL_argcheck(L, 1 <= index && index <= a->size, 2, "index out of range");

	a->values[index-1] = nValue;
	return 0;
}

static int getarray(lua_State *L){
	NumArray *a = (NumArray *)lua_touserdata(L, 1);
	int index = luaL_checkint(L, 2);

	luaL_argcheck(L, a!= NULL, 1, "'array' expected");
	luaL_argcheck(L, 1 <= index && index <= a->size, 2, "index out of range");
	lua_pushnumber(L, a->values[index - 1]);
	return 1;
}

static int getsize(lua_State *L){
	NumArray *a = (NumArray*)lua_touserdata(L, 1);
	luaL_argcheck(L, a != NULL, 1, "'array' expected");
	lua_pushnumber(L, a->size);

	return 1;
}*/

// 28.2
/*
static int newarray(lua_State *L)
{
	int n = luaL_checkint(L, 1);
	size_t nbytes = sizeof(NumArray) + (n-1)*sizeof(double);
	NumArray *a = (NumArray *)lua_newuserdata(L, nbytes);

	luaL_getmetatable(L, "LuaBook.array");
	lua_setmetatable(L, -2);
	a->size = n;
	return 1;
}

static NumArray *checkarray(lua_State *L){
	void *ud = luaL_checkudata(L, 1, "luaBook.array");
	luaL_argcheck(L, ud != NULL, 1, "'array' expected");
	return (NumArray*)ud;
}

static double *getelem(lua_State *L){
	NumArray *a = checkarray(L);
	int index = luaL_checkint(L, 2);

	luaL_argcheck(L, 1<=index && index <= a->size, 2, "index out of ranget");

	return &a->values[index-1];
}
static int setarray(lua_State *L){
	double nValue = luaL_checknumber(L, 3);

	*getelem(L) = nValue;
	return 0;
}

static int getarray(lua_State *L){
	lua_pushnumber(L, *getelem(L));
	return 1;
}

static int getsize(lua_State *L){
	NumArray *a = checkarray(L);
	lua_pushnumber(L, a->size);

	return 1;
}

static const struct luaL_reg arraylib [] = {
	{"new", newarray},
	{"set", setarray},
	{"get", getarray},
	{"size", getsize},
	{NULL, NULL},
};

extern "C" __declspec(dllexport)
int luaopen_array(lua_State *L){
	luaL_newmetatable(L, "LuaBook.array");
	luaL_register(L,"array",arraylib);
	return 1;
}*/

//////////////////////////////////////////////////////////////////////////
//28.3
/*
static int newarray(lua_State *L)
{
	int n = luaL_checkint(L, 1);
	size_t nbytes = sizeof(NumArray) + (n-1)*sizeof(double);
	NumArray *a = (NumArray *)lua_newuserdata(L, nbytes);

	luaL_getmetatable(L, "LuaBook.array");
	lua_setmetatable(L, -2);
	a->size = n;
	return 1;
}

static NumArray *checkarray(lua_State *L){
	void *ud = luaL_checkudata(L, 1, "LuaBook.array");
	luaL_argcheck(L, ud != NULL, 1, "'array' expected");
	return (NumArray*)ud;
}

static double *getelem(lua_State *L){
	NumArray *a = checkarray(L);
	int index = luaL_checkint(L, 2);

	luaL_argcheck(L, 1<=index && index <= a->size, 2, "index out of ranget");

	return &a->values[index-1];
}
static int setarray(lua_State *L){
	double nValue = luaL_checknumber(L, 3);

	*getelem(L) = nValue;
	return 0;
}

static int getarray(lua_State *L){
	lua_pushnumber(L, *getelem(L));
	return 1;
}

static int getsize(lua_State *L){
	NumArray *a = checkarray(L);
	//NumArray *a = (NumArray *)luaL_checkudata(L, 1, "LuaBook.array"); //同样增加参数类型和metatable的检查
	lua_pushnumber(L, a->size);

	return 1;
}

int array2string(lua_State *L){
	NumArray *a = checkarray(L);
	lua_pushfstring(L, "array(%d)", a->size);
	return 1;
}

static const struct luaL_reg arraylib_f [] = {
	{"new", newarray},
	{NULL, NULL},
};

static const struct luaL_reg arraylib_m [] = {
	{"__tostring", array2string},
	{"set", setarray},
	{"get", getarray},
	{"size", getsize},
	{NULL, NULL},
};

extern "C" __declspec(dllexport)
int luaopen_array(lua_State *L){
	luaL_newmetatable(L, "LuaBook.array");

	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_register(L,NULL,arraylib_m);
	luaL_register(L,"array",arraylib_f);

	return 1;
}*/

/////////////////////////////////////////////////////////
//29.1

static int dir_iter(lua_State *L);

static int l_dir (lua_State *L){
	const char *path = luaL_checkstring(L, 1);

	DIR **d = (DIR **)lua_newuserdata(L, sizeof(DIR *));

	/* set its metable */
	luaL_getmetatable(L, "LuaBook.dir");
	lua_setmetatable(L, -2);

	/* try to open the given directory */
	*d = opendir(path);
	if (*d == NULL)
	{
		luaL_error(L, "cannot open %s: %s", path, strerror(errno));
	}
	/* creates and returns the iterator function */
	lua_pushcclosure(L, dir_iter, 1);
}