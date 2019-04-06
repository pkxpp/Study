
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#ifdef __cplusplus
}
#endif


int main(void)
{
	char buff[256];
	int error;
	lua_State *L = lua_open();	/* open lua */
	luaopen_base(L);			/* open the basic libary */
	luaopen_table(L);			/* open the table library */
	luaopen_io(L);
	luaopen_string(L);
	luaopen_math(L);

	while(fgets(buff, sizeof(buff), stdin) != NULL){
		error = luaL_loadbuffer(L, buff, strlen(buff), 
			"line") || lua_pcall(L, 0, 0, 0);						

		if (error){
			fprintf(stderr, "%s", lua_tostring(L, -1));
			lua_pop(L, 1);		/* pop error message from the stack */ 
		}
	}


	lua_close(L);
	return 0;
}