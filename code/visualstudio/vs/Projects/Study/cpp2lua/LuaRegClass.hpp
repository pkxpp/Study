#ifndef _LUA_REG_CLASS_HPP_
#define _LUA_REG_CLASS_HPP_

#include "LuaRegDef.hpp"

#include <new>
#include <cassert>

DECL_NAMESPACE_LUAREG_BEGIN

/* set metatable for class */

template<typename T>
inline int MetaGet(lua_State* L) {
  lua_getmetatable(L, 1);
  lua_pushvalue(L, 2);
  lua_rawget(L, -2);
  if (lua_isuserdata(L, -1)) {
    lua_remove(L, -2);
    void* memory = lua_touserdata(L, -1);
    GenericMember* member = static_cast<GenericMember*>(memory);
    member->Get(L);
  } else if (lua_isnil(L, -1)) {
    //lua_remove(L, -1);
  }
  lua_remove(L, -2);
  return 1;
}

template<typename T>
inline int MetaSet(lua_State* L) {
  lua_getmetatable(L, 1);
  lua_pushvalue(L, 2);
  lua_rawget(L, -2);
  if (lua_isuserdata(L, -1)) {
    void* memory = lua_touserdata(L, -1);
    GenericMember* member = static_cast<GenericMember*>(memory);
    member->Set(L);
  } else if (lua_isnil(L, -1)) {
    lua_pushvalue(L, 2);
    lua_pushvalue(L, 3);
    lua_rawset(L, -4);
  }
  lua_settop(L, 3);
  return 0;
}

template<typename T>
inline int DoGc(lua_State* L) {
  return 0;
}

template<typename T>
void SetMetatable(lua_State* L, const char* className) {
  luaL_newmetatable(L, className);

  lua_pushliteral(L, "__index");
  lua_pushcfunction(L, MetaGet<T>);
  lua_settable(L, -3);

  lua_pushliteral(L, "__newindex");
  lua_pushcfunction(L, MetaSet<T>);
  lua_settable(L, -3);

  lua_pushliteral(L, "__gc");
  lua_pushcfunction(L, DoGc<T>);
  lua_settable(L, -3);

  lua_setmetatable(L, -2);
}

/* register class type */

template<typename T>
void RegisterClassToLua(lua_State* L, const char* className) {
  ClassInfo<T>::Name(className);
  lua_newtable(L);
  SetMetatable<T>(L, className);
  lua_setglobal(L, className);
}

/* register class method */

struct GenericMethod {
  virtual int Call(lua_State *L) = 0;
};

#define DEF_METHOD_DATA(N) \
template<TYPENAME_LIST_##N> \
struct MethodData##N : public GenericMethod { \
  typedef R (T::*Func)(PARAM_TYPE_LIST_##N); \
  Func m_func; \
  MethodData##N(Func func) : m_func(func) {} \
  int Call(lua_State* L) { \
    T* obj = ReadToCpp<T*>(L, 1); \
    R result =(obj->*m_func)(ARG_LIST_##N); \
    PushToLua<R>(L, result); \
    return 1; \
  } \
};

#define DEF_VOID_METHOD_DATA(N) \
template<TYPENAME_VOID_LIST_##N> \
struct MethodData##N<void, TYPE_VOID_LIST_##N> : public GenericMethod { \
  typedef void (T::*Func)(PARAM_TYPE_LIST_##N); \
  Func m_func; \
  MethodData##N(Func func) : m_func(func) {} \
  int Call(lua_State* L) { \
    T* obj = ReadToCpp<T*>(L, 1); \
    (obj->*m_func)(ARG_LIST_##N); \
    return 0; \
  } \
};

template<typename T>
inline int ProxyMethodCall(lua_State *L) {
  int index = lua_upvalueindex(1);
  void* memory = lua_touserdata(L, index);
  GenericMethod* method = static_cast<GenericMethod*>(memory);
  return method->Call(L);
}

#define DEF_REGISTER_METHOD(N) \
template<TYPENAME_LIST_##N> \
inline void RegisterMethodToLua(lua_State* L, const char* funcName, \
  typename R (T::*func)(PARAM_TYPE_LIST_##N)) { \
  luaL_getmetatable(L, ClassInfo<T>::Name()); \
  lua_pushstring(L, funcName); \
  typedef typename MethodData##N<TYPE_LIST_##N> MethodData_t; \
  void* memory = lua_newuserdata(L, sizeof(MethodData_t)); \
  new(memory) MethodData_t(func); \
  lua_pushcclosure(L, &ProxyMethodCall<T>, 1); \
  lua_rawset(L, -3); \
  lua_pop(L, 1); \
}

DEF_METHOD_DATA(0)
DEF_METHOD_DATA(1)
DEF_METHOD_DATA(2)
DEF_METHOD_DATA(3)
DEF_METHOD_DATA(4)
DEF_METHOD_DATA(5)
DEF_METHOD_DATA(6)
DEF_METHOD_DATA(7)
DEF_METHOD_DATA(8)
DEF_METHOD_DATA(9)

DEF_VOID_METHOD_DATA(0)
DEF_VOID_METHOD_DATA(1)
DEF_VOID_METHOD_DATA(2)
DEF_VOID_METHOD_DATA(3)
DEF_VOID_METHOD_DATA(4)
DEF_VOID_METHOD_DATA(5)
DEF_VOID_METHOD_DATA(6)
DEF_VOID_METHOD_DATA(7)
DEF_VOID_METHOD_DATA(8)
DEF_VOID_METHOD_DATA(9)

DEF_REGISTER_METHOD(0)
DEF_REGISTER_METHOD(1)
DEF_REGISTER_METHOD(2)
DEF_REGISTER_METHOD(3)
DEF_REGISTER_METHOD(4)
DEF_REGISTER_METHOD(5)
DEF_REGISTER_METHOD(6)
DEF_REGISTER_METHOD(7)
DEF_REGISTER_METHOD(8)
DEF_REGISTER_METHOD(9)

/* register class member */
struct GenericMember {
  virtual int Get(lua_State* L) = 0;
  virtual int Set(lua_State* L) = 0;
};

template<typename T, typename V>
struct MemberData : public GenericMember {
  typedef V T::*Var;
  Var m_var;
  MemberData(Var var) : m_var(var) {}
  int Get(lua_State* L) {
    T* obj = ReadToCpp<T*>(L, 1);
    V member = obj->*m_var;
    PushToLua(L, member);
    return 1;
  }
  int Set(lua_State* L) {
    T* obj = ReadToCpp<T*>(L, 1);
    V& member = obj->*m_var;
    member = ReadToCpp<V>(L, 3);
    return 0;
  }
};

template<typename T, typename P>
void RegisterMemberToLua(lua_State* L, const char* varName, P T::*var) {
  luaL_getmetatable(L, ClassInfo<T>::Name());
  typedef MemberData<T, P> MemberData_t;
  lua_pushstring(L, varName);
  void* memory = lua_newuserdata(L, sizeof(MemberData_t));
  new(memory) MemberData_t(var);
  lua_rawset(L, -3);
  lua_pop(L, 1);
}

/* register class object */

template<typename T>
void RegisterObjectToLua(lua_State* L, const char* name, T* object) {
  typedef UserData<T*> UserData_t;
  void* memory = lua_newuserdata(L, sizeof(UserData_t));
  new(memory) UserData_t(object);
  luaL_getmetatable(L, ClassInfo<T>::Name());
  lua_setmetatable(L, -2);
  lua_setglobal(L, name);
}

DECL_NAMESPACE_LUAREG_END

/* macros for register a class */

#define OFFSET(Class, Var) (int(&((Class*)0)->Var))

#define DEF_LUA_CLASS_BEGIN(Class) \
static void RegisterToLua(const char* className) { \
  typedef Class T; \
  g_luaReg->RegisterClass<T>(className);

#define DEF_METHOD(Func) \
  g_luaReg->RegisterMethod(#Func, &T::Func);

#define DEF_MEMBER(Var) \
  g_luaReg->RegisterMember<T>(#Var, &T::Var);

#define DEF_END \
}

#define REGISTER_LUA_CALSS(Class) \
Class::RegisterToLua(#Class);

#endif // _LUA_REG_CLASS_HPP_
