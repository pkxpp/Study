#include "LuaReg.hpp"

DECL_NAMESPACE_LUAREG_BEGIN

bool LuaReg::Init() {
  L = luaL_newstate();
  if (nullptr == L) {
    return false;
  }
  luaL_openlibs(L);
  return true;
}

void LuaReg::Clear() {
  lua_close(L);
}

void LuaReg::DoScript(const char* file) {
 luaL_dofile(L, "Test.lua");
}

DECL_NAMESPACE_LUAREG_END

NS_LUAREG::LuaReg* g_luaReg = NULL;

bool  LuaInit() {
  g_luaReg = new NS_LUAREG::LuaReg;
  return g_luaReg->Init();
}

void LuaClear() {
  g_luaReg->Clear();
  delete g_luaReg;
  g_luaReg = nullptr;
}