#ifndef _TEST_HPP_
#define _TEST_HPP_

#include <iostream>
#include "LuaReg.hpp"

class Test {
public:
  Test() {}
  ~Test() {}

public:
  int GetData() { return m_data; }
  void SetData(int value){ m_data = value; }
  const char* GetStr(){return "test";}
  int FuncTest(int n, const char* pcszParam = "");
  int FuncTest1(const char* pcszParam, void* pvParam = NULL);
  int FuncTest2(void* Param);
  int FuncTest3(int n, void* Param);

  int RelaySender(int nOp, int nParam, const char* cszParam, void* pvParam = NULL) 
  {
	  return 0;
  }

public:
  int m_data;
  int m_data2;
  char* m_pData;

public:
  DEF_LUA_CLASS_BEGIN(Test)
 //   DEF_METHOD(SetData)
 //   DEF_METHOD(GetData)
	//DEF_METHOD(GetStr)
	DEF_METHOD(FuncTest)
	DEF_METHOD(FuncTest1)
	DEF_METHOD(FuncTest2)
	DEF_METHOD(FuncTest3)
	//DEF_METHOD(RelaySender)
    DEF_MEMBER(m_data)
    DEF_MEMBER(m_data2)
  DEF_END
};

int Test::FuncTest(int n, const char* pcszParam/* = ""*/)
{
	std::cout << "FuncTest ---" << n << "," << pcszParam << std::endl;
	return 0;
}

int Test::FuncTest1(const char* pcszParam, void* pvParam)
{
	std::cout << "FuncTest1 ---" << pcszParam << std::endl;
	return 0;
}

int Test::FuncTest2(void* Param)
{
	std::cout << "FuncTest2 ---" << std::endl;
	return 0;
}

int Test::FuncTest3(int n, void* Param)
{
	std::cout << "FuncTest3 ---" << n << ", " << std::endl;
	return 0;
}

#endif // _TEST_HPP_
