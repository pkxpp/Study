#ifndef _LUA_WRAPPER_HPP_
#define _LUA_WRAPPER_HPP_

#include "LuaRegDef.hpp"
#include "LuaRegClass.hpp"

DECL_NAMESPACE_LUAREG_BEGIN

class LuaReg {
public:
  bool Init();

  void Clear();

  void DoScript(const char* file);

public:
  template<typename T>
  void RegisterClass(const char* className) {
    RegisterClassToLua<T>(L, className);
  }

  template<typename F>
  void RegisterMethod(const char* funcName, F func) {
    RegisterMethodToLua(L, funcName, func);
  }

  template<typename T, typename V>
  void RegisterMember(const char* varName, V var) {
    RegisterMemberToLua<T>(L, varName, var);
  }

  template<typename T>
  void RegisterObject(const char* name, T* object) {
    RegisterObjectToLua<T>(L, name, object);
  }

private:
  lua_State* L;
};

DECL_NAMESPACE_LUAREG_END

extern NS_LUAREG::LuaReg* g_luaReg;
extern bool LuaInit();
extern void LuaClear();

#endif // _LUA_WRAPPER_HPP_
