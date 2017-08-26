#include "Test.hpp"

#include <cstdint>
#include <iostream>

using namespace std;

USING_NAMESPACE_LUAREG

// eg: 1
template<class T> void f( T);				// (b) 基础模板
template<class T> void f( int, T,double );	// (c) 基础模板重载
template<class T> void f( T*);				// (d) 基础模板重载
template<> void f<int>( int);				// (e) 基础模板(b)特化
void f( double);							// (f) 非模板函数，重载


// eg: 2
template<class T>	 // (a2) 一个基础模版
void func( T );

template<class T>	 // (b2) 另一个基础模版，(a2)的重载版本
void func( T* );		 //     （函数模版没有偏特化；可以用重载替代）

template<>			 // (c2) (b2)模版的全特化
void func<>(int*);

// eg: 3
// (a3) 和之前一样的基础模版
template<class T> 
void f3( T )
{
	cout << "a3 function" << endl;
}
// (c3) 针对(a3)的完全特化
template<>       
void f3<>(int*)
{
	cout << "c3 function" << endl;
}
// (b3) 另一个基础模版，对(a3)的重载
template<class T> 
void f3( T* )
{
	cout << "b3 function" << endl;
}

void LuaTest() {
  REGISTER_LUA_CALSS(Test);
  
  Test* test = new Test();
  
  test->m_data = 1001;
  test->m_data2 = 1002;
  g_luaReg->RegisterObject("test", test);
  g_luaReg->DoScript("Test.lua");
}

int main() {
  LuaInit();
  LuaTest();
  LuaClear();

  // test 1
  //bool b = true;
  //int i = 1;
  //double d = 1.1f;
  //f( b);        // 调用(b)模版 模版参数T = bool
  //f( i, 42, d ); // 调用(c)模版 T = int
  //f( &i);       // 调用(d)模版 T = int
  //f( i);        // 调用(e)模版
  //f( d);        // 调用(f)函数

  //// test 2
  //int n = 2;
  //int *p = &n;
  //func( p);           // 调用(c2)

  //// test3
  //f3(p);

  cout<< "Input Enter key to exit." << endl;
  getchar();
  return 0;
}
 
// (b) 基础模板
template<class T> void f( T)
{
	cout << "(b)" << endl;
}

// (c) 基础模板重载
template<class T> void f( int, T,double )
{
	cout << "(c)" << endl;
}

// (d) 基础模板重载
template<class T> void f( T*)
{
	cout << "(d)" << endl;
}

// (e) 基础模板(b)特化
template<> void f<int>( int)
{
	cout << "(e)" << endl;
}

// (f) 非模板函数，重载
void f( double)
{
	cout << "(f)" << endl;
}

//////////////////////////////////////////////////////////////////////////
 // (a2) 一个基础模版
template<class T>	
void func( T )
{
	cout << "a2 func" << endl;
}

// (b2) 另一个基础模版，(a2)的重载版本（函数模版没有偏特化；可以用重载替代）
template<class T>
void func( T* )
{
	cout << "b2 func" << endl;
}

// (c2) (b2)模版的全特化
template<>
void func<>(int*)
{
	cout << "c2 func" << endl;
}