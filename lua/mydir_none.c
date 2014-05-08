#include <math.h>
#include <dirent.h>
#include <errno.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

/* forward declaration */
static int dir_iter(lua_State *L);

static int l_dir(lua_State *L){
	const char *path = luaL_checkstring(L, 1);

	/* create a userdatum */
	DIR **d = (DIR **)lua_newuserdata(L, sizeof(DIR *));

	/* set its metatable */
	lua_getmetatable(L, "LuaBook.dir");
	lus_setmetatable(L, -2);
	
	/* try to pen then given dirctory */
	*d = opendir(path);
	if (*d == NULL)
	luaL_error(L, "cannot open %s: %s", path, strerror(errno));
	
	/* creates and returns the iterator function */
	lua_pushcclosure(L, dir_iter, 1);
	return 1;
}

static int dir_iter(lua_State *L){
	DIR *d = *(DIR **)lua_touserdata(L, lua_upvalueindex(1));
	struct dirent *entry;
	if (( entry = readdir(d)) != NULL){
		lua_pushstring(L, entry->d_name);
		return 1;
	}
	else return 0;	/* no more valuse to return */
}

static int dir_gc(lua_State *L){
	DIR *d = *(DIR **)lua_touserdata(L, 1);
	if (d) closedir(d);
	return 0;
}

//int luaopen_dir (lua_State *L){
//	luaL_newmetatable(L, "LuaBook.dir");
	
	/* set its __Gc field */
//	lua_pushstring(L, "__gc");
//	lua_pushcfunction(L, dir_gc);
//	lua_settable(L, -3);
	
	/* register the 'dir' function */
//	lua_pushcfunction(L, l_dir);
//	lua_setglobal(L, "dir");

//	return 0;
//}
static const struct luaL_Reg mydir[] = {
	{"mydir", l_dir},
	{NULL, NULL},
};

int luaopen_mydir (lua_State *L){
	luaL_newmetatable(L, "LuaBook.dir");
	
	/* set its __Gc field */
	lua_pushstring(L, "__gc");
	lua_pushcfunction(L, dir_gc);
	lua_settable(L, -3);
	
	/* register the 'dir' function */
//	lua_pushcfunction(L, l_dir);
//	lua_setglobal(L, "dir");
	luaL_openlib(L, "mydir", mydir, 0);

	return 0;
}
