#pragma once

#include <iostream>
using namespace std;

class ITestBase
{
public:
	virtual void FuncBase() = 0;
};

class ITestInheritedBase : virtual public ITestBase
{
public:
	virtual void FuncInheritedBase() = 0;
};


class TestInheritedA : virtual public ITestBase
{
public:
	TestInheritedA();
	virtual ~TestInheritedA();

	virtual void FuncBase()
	{
		cout << "TestInheritedA::FuncBase" << endl;
	};
};


class TestInheritedB : public ITestInheritedBase, public TestInheritedA
{
public:
	TestInheritedB();
	virtual ~TestInheritedB();

	// 不实现的话报错error C2259: “TestInheritedB”: 不能实例化抽象类
	//virtual void FuncBase()
	//{
	//	cout << "TestInheritedB::FuncBase" << endl;
	//}

	virtual void FuncInheritedBase()
	{
		cout << "TestInheritedB::FuncInheritedBase" << endl;
	};
};

//////////////////////////////////////////////////////////////////////////
void TestInherited();