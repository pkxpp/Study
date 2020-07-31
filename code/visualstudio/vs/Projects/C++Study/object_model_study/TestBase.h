#pragma once

#include <iostream>
#include <string>
#include <Windows.h>
using namespace std;

//////////////////////////////////////////////////////////////////////////
// 参考：https://www.cnblogs.com/zhyg6516/archive/2011/03/07/1971898.html

struct TypeDescriptor
{
	DWORD ptrToVTable;
	DWORD spare;
	char name[];
};

struct RTTICompleteObjectLocator
{
	DWORD signature; //always zero ?
	DWORD offset;    //offset of this vtable in the complete class
	DWORD cdOffset;  //constructor displacement offset
	struct TypeDescriptor* pTypeDescriptor; //TypeDescriptor of the complete class
	struct RTTIClassHierarchyDescriptor* pClassDescriptor; //describes inheritance hierarchy
};

//////////////////////////////////////////////////////////////////////////
// 参考：https://blog.csdn.net/HeXiQuan123/article/details/100534110
// 参考：https://my.oschina.net/u/2243185/blog/423576
class TestBase
{
public:
	TestBase(int i);
	int getI() { return baseI; }

	static void countI() {};

	virtual void print(void) { cout << "Base::print()"; }
	virtual ~TestBase();

private:

	int baseI;

	static int baseS;
};

class TestDerive : public TestBase
{
public:
	TestDerive(int d) :TestBase(1000), DeriveI(d) {};

	//overwrite父类虚函数
	virtual void print(void) { cout << "Drive::print()"; }
	virtual ~TestDerive() {}

	// Derive声明的新的虚函数
	virtual void Drive_print() { cout << "Drive::Drive_print()"; }
	
private:
	int DeriveI;
};

class TestBase2
{
public:
	TestBase2(int i) :base2I(i) {};

	

	int getI() { return base2I; }

	static void countI() {};

	virtual void print(void) { cout << "TestBase2::print()"; }
	virtual ~TestBase2() {}
private:

	int base2I;

	static int base2S;
};

class TestDriveMultiBase :public TestBase, public TestBase2
{
public:

	TestDriveMultiBase(int n) :TestBase(1000), TestBase2(2000), m_nDriveMultiBaseI(n) {};

	virtual void print(void) { cout << "TestDriveMultiBase::print"; }

	virtual void Drive_print() { cout << "TestDriveMultiBase::Drive_print"; }

private:
	int m_nDriveMultiBaseI;
};

//////////////////////////////////////////////////////////////////////////
// diamond 菱形继承
namespace test_derive {
	class B

	{

	public:

		int ib;

	public:

		B(int i = 1) :ib(i) {}

		virtual void f() { cout << "B::f()"; }

		virtual void Bf() { cout << "B::Bf()"; }

	};

	class B1 : public B

	{

	public:

		int ib1;

	public:

		B1(int i = 100) :ib1(i) {}

		virtual void f() { cout << "B1::f()"; }

		virtual void f1() { cout << "B1::f1()"; }

		virtual void Bf1() { cout << "B1::Bf1()"; }



	};

	class B2 : public B

	{

	public:

		int ib2;

	public:

		B2(int i = 1000) :ib2(i) {}

		virtual void f() { cout << "B2::f()"; }

		virtual void f2() { cout << "B2::f2()"; }

		virtual void Bf2() { cout << "B2::Bf2()"; }

	};


	class D : public B1, public B2

	{

	public:

		int id;



	public:

		D(int i = 10000) : id(i) {}

		virtual void f() { cout << "D::f()"; }

		virtual void f1() { cout << "D::f1()"; }

		virtual void f2() { cout << "D::f2()"; }

		virtual void Df() { cout << "D::Df()"; }

	};

}

//////////////////////////////////////////////////////////////////////////
// virtual derive
namespace test_derive {
	class VB1 : virtual public B
	{

	public:

		int ib1;

	public:

		VB1(int i = 100) :ib1(i) {}

		virtual void f() { cout << "B1::f()"; }

		virtual void f1() { cout << "B1::f1()"; }

		virtual void Bf1() { cout << "B1::Bf1()"; }

	};

	class VB2 : virtual public B
	{
	public:
		int ib2;
	public:
		VB2(int i = 1000) :ib2(i) {}
		virtual void f() { cout << "B2::f()"; }
		virtual void f2() { cout << "B2::f2()"; }
		virtual void Bf2() { cout << "B2::Bf2()"; }

	};

	class VD : public VB1, public VB2
	{
	public:
		int id;

	public:
		VD(int i = 10000) : id(i) {}
		virtual void f() { cout << "D::f()"; }
		virtual void f1() { cout << "D::f1()"; }
		virtual void f2() { cout << "D::f2()"; }
		virtual void Df() { cout << "D::Df()"; }
	};
}

//////////////////////////////////////////////////////////////////////////
// test function
void TestObjectModel();
void TestBaseFunction(TestBase&p);
void TestDeriveFunction();
void TestMultiDeriveFunction();
void TestDiamondDeriveFunction();
void TestVirtualDeriveFunction(); 
void TestVirtualDiamondDeriveFunction();