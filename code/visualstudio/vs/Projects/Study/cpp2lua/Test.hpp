#ifndef _TEST_HPP_
#define _TEST_HPP_

#include "LuaReg.hpp"

class Test {
public:
  Test() {}
  ~Test() {}

public:
  int GetData() { return m_data; }
  void SetData(int value){ m_data = value; }
  const char* GetStr(){return "test";}

public:
  int m_data;
  int m_data2;

public:
  DEF_LUA_CLASS_BEGIN(Test)
    DEF_METHOD(SetData)
    DEF_METHOD(GetData)
	DEF_METHOD(GetStr)
    DEF_MEMBER(m_data)
    DEF_MEMBER(m_data2)
  DEF_END
};

#endif // _TEST_HPP_
