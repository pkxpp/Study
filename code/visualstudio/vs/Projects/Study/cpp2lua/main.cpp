#include "Test.hpp"

#include <cstdint>
#include <iostream>
#include <windows.h>

//#include "lstate.h"

using namespace std;

Test g_objTest;

void myPrintLuaStack(lua_State *L);
void callLuaFuncDemo(lua_State* L);
int TestLuaStack();

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

void LuaCheckGC()
{
	lua_State* L = g_luaReg->GetLuaState();
	int nMemCount = lua_gc(L, LUA_GCCOUNT, 0);
	nMemCount >>= 4;	//回收虚拟机当前所分配内存大小的16分之1的内存
	if (nMemCount < 2)
	{
		nMemCount = 2;
	}
	lua_gc(L, LUA_GCSTEP, nMemCount);
}

void LuaTest() {
	//REGISTER_LUA_CALSS(Base2);
	//REGISTER_LUA_CALSS(Base);
	//REGISTER_LUA_CALSS(Test);
	//REGISTER_LUA_CALSS(SubTest);
 // 
	//Test* test = new Test();
 // 
	//test->m_data = 1001;
	//test->m_data2 = 1002;
	//g_luaReg->RegisterObject("test", test);

	//Base* base = (Base*)test;
	//g_luaReg->RegisterObject("base", base);
 // 
	//// base2 constructor is private
	//Base2* base2 = Base2::GetInstance();
	//g_luaReg->RegisterObject("base2", base2);

	//lua_State* L = g_luaReg->GetLuaState();
  
	//struct lua_State* pL = (struct lua_State*)L;
	//cout << pL->stacksize << endl;

	// register c++ function to lua
	//int nSize = sizeof(GameScriptFuns)  / sizeof(GameScriptFuns[0]) - 1;
	//for (size_t i = 0; i < nSize; ++i)
	//{
	//	lua_pushcfunction(L, GameScriptFuns[i].func);
	//	lua_setglobal(L, GameScriptFuns[i].name);
	//}

	g_luaReg->DoScript("Test.lua");

	// make a loop
	//int nRetCode = true;
	//int nTick = 0;
	//while (true)
	//{
	//	if (!nRetCode)
	//	{
	//		nRetCode = true;
	//		Sleep(1000);
	//		continue;
	//	}
	//	// todo
	//	cout << "Tick: " << nTick << endl;
	//	LuaCheckGC();

	//	if (nTick %5 == 0)
	//	{
	//		g_luaReg->DoScript("Test1.lua");
	//	}

	//	++nTick;
	//	nRetCode = false;
	//}
}

int main() {
	LuaInit();
	LuaTest();
	//game
	int nCount = 0;
	while(true){
		int bEven = nCount %2 == 0;
		if(!bEven)
		{
			Sleep(1000);
			++nCount;
			continue;
		}
		++nCount;
		// do something
		printf("11111111111111111111\n");
	}
	// game end
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

	// lua stack test
	//TestLuaStack();

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

//////////////////////////////////////////////////////////////////////////
// lua stack
int TestLuaStack(){
	lua_State* L=luaL_newstate();
	if(L){
		luaL_openlibs(L); 
		//载入foo.lua 	    
		if(!luaL_dofile (L,"stack_test.lua"))
		{  
		lua_pushstring(L,"http://blog.163.com/mr_zyf");//为了演示，压入一个字符串
         
			myPrintLuaStack(L);//调用lua中foo函数前，显示栈的信息
			/**
			--栈顶(v)(1)--
			(i:1) string(http://blog.163.com/mr_zyf)
			--栈底--
			**/
			callLuaFuncDemo(L);//调用lua中foo函数
         
			myPrintLuaStack(L); //调用lua中foo函数后，显示栈的信息
		/**
			--栈顶(v)(1)--
			(i:1) string(http://blog.163.com/mr_zyf)
			--栈底--
			**/       
		}else
	printf("error:%s\n",lua_tostring(L,-1));
        
		lua_close(L);      
	}else
		printf("error:new lua_State failed!");
	return 0;
}

//调用lua中foo函数的演示函数
void callLuaFuncDemo(lua_State* L){
	int error;
    lua_getglobal(L,"foo");//压入函数名
    lua_pushnumber(L,1);//压入第一个数值参数：1
    lua_pushnumber(L,2);//压入第二个数值参数：2
    myPrintLuaStack(L);//显示栈的信息
    /**
    --栈顶(v)(4)--
    (i:4) number(2)
    (i:3) number(1)
    (i:2) function((null))
    (i:1) string(http://blog.163.com/mr_zyf)
	--栈底--
	**/

    error=lua_pcall(L,2,2,0);//调用foo函数，输入两个参数，返回两个结果
    if (!error) {
		//检测返回值是否压栈
		if (!lua_isstring(L,-1))
			printf("error:must return a value!\n");
		else
		{
			myPrintLuaStack(L);
			/**
			--栈顶(v)(3)--
			(i:3) string(result val2)
			(i:2) string(result val1!)
			(i:1) string(http://blog.163.com/mr_zyf)
			--栈底--
			调用后发现栈中的函数参数和函数名被自动弹出，返回的结果被压入栈中
			**/
			printf("return:%s\n",lua_tostring(L,-1));
			printf("return:%s\n",lua_tostring(L,-2));
			//注意：如果调用lua_pop(L,-1)，会清空栈
			//lua_pop(L,2);//从栈顶弹出2个元素
			int nMemCount = lua_gc(L, LUA_GCCOUNT, 0);
			lua_gc(L, LUA_GCSTEP, nMemCount);
			printf("return:%s\n",lua_tostring(L,-1));
		}
    }else {     
		printf("error:%s.\n",lua_tostring(L,-1));
		//从栈顶弹出1个元素
		lua_pop(L,1);
    }
}
//显示栈信息的函数
void myPrintLuaStack(lua_State *L){
	int stackTop=lua_gettop(L);//获取栈顶的索引值
	int index,t;
	printf("--栈顶(v)(%d)--\n",stackTop);
	//显示栈中的元素
	for(index=stackTop;index>0;--index)
		{  t=lua_type(L,index);
		printf("(i:%d) %s(%s)\n",index,lua_typename(L,t),lua_tostring(L,index));	     
		}
	printf("--栈底--\n");
}