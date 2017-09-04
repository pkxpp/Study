#ifndef _TEST_HPP_
#define _TEST_HPP_

#include <iostream>
#include <vector>
#include "LuaReg.hpp"

class Base {
public:
	int Foo(){
		return 1;
	}
};

class Test : public Base {
public:
  Test() {
	  strcpy(m_szData, "Array String.");
  }
  ~Test() {}

public:
  int GetData() { return m_data; }
  void SetData(int value){ m_data = value; }
  const char* GetStr(){return "test";}
  int FuncTest(int n, const char* pcszParam = "");
  int FuncTest1(const char* pcszParam, void* pvParam = NULL);
  int FuncTest2(void* Param);
  int FuncTest3(int n, void* Param);
  bool IsExist(int nData);
  //int FuncTest3(int n, char* szString, bool b);
  void PushData(int nData);
  const char* GetName(){return m_szData;}

  int RelaySender(int nOp, int nParam, const char* cszParam, void* pvParam = NULL) 
  {
	  return 0;
  }

public:
  int m_data;
  int m_data2;
  char* m_pData;
  char m_szData[32];
  std::vector<int> m_vecData;

public:
  DEF_LUA_CLASS_BEGIN(Test)
 //   DEF_METHOD(SetData)
 //   DEF_METHOD(GetData)
	//DEF_METHOD(GetStr)
	DEF_METHOD(FuncTest)
	DEF_METHOD(FuncTest1)
	DEF_METHOD(FuncTest2)
	DEF_METHOD(FuncTest3)
	DEF_METHOD(IsExist)
	DEF_METHOD(PushData)
	DEF_METHOD(GetName)
	//DEF_METHOD(Foo)  //Error!!!!!!!!!!!!!!!!!!!!!!!!
	//DEF_METHOD(RelaySender)
    DEF_MEMBER(m_data)
    DEF_MEMBER(m_data2)
	//DEF_MEMBER(m_szData) // Error!!!!!!!!!!!!!!!!!error C2075: “member”: 数组初始化需要大括号
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

//int Test::FuncTest3(int n, char* szString, bool b)
//{
//	std::cout << "FuncTest333 ---" << n << ", " << std::endl;
//	return 0;
//}

bool Test::IsExist(int nData)
{
	std::vector<int>::iterator it = m_vecData.begin();
	for (; it != m_vecData.end();++it)
	{
		if (*it == nData )
			return true;
	}

	return false;
}


void Test::PushData(int nData)
{
	m_vecData.push_back(nData);
}
#endif // _TEST_HPP_
