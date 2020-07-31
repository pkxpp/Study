#include "TestBase.h"
#include <typeinfo>
//#include <typeinfo.h>



TestBase::TestBase(int i)
	:baseI(i)
{
}


TestBase::~TestBase()
{
}

//////////////////////////////////////////////////////////////////////////
void TestObjectModel()
{
	// base
	//TestBase b(100);
	//TestBaseFunction(b);

	// derive
	//TestDeriveFunction();

	// multiple derive
	//TestMultiDeriveFunction();

	// diamond derive
	//TestDiamondDeriveFunction();
	

	// virtual derive
	//TestVirtualDeriveFunction();

	// virtual diamond derive
	TestVirtualDiamondDeriveFunction();
}
//
void TestBaseFunction(TestBase&p)
{
	cout << "对象的内存起始地址：" << &p << endl;
	cout << "type_info信息:" << endl;
	// 注意：如果是64位，用int去偏移是不对的，64位下面指针式8位的，要把int换成long
	// (int*)(&p)是虚表vprt的地址，取地址运算*(int*)(&p)才是是虚表vptr
	RTTICompleteObjectLocator str = *((RTTICompleteObjectLocator*)*((int*)*(int*)(&p) - 1));


	string classname(str.pTypeDescriptor->name);
	classname = classname.substr(4, classname.find("@@") - 4);
	cout << "根据type_info信息输出类名:" << classname << endl;

	cout << "虚函数表地址:" << (int *)(&p) << endl;

	//验证虚表
	cout << "虚函数表第一个函数的地址：" << (int *)*((int*)(&p)) << endl;
	cout << "析构函数的地址:" << (int*)*(int *)*((int*)(&p)) << endl;
	cout << "下一个函数的地址:" << (int*)*(((int*)*(int*)(&p) + 1)) << endl;
	cout << "虚函数表中，第二个虚函数即print（）的地址：" << ((int*)*(int*)(&p) + 1) << endl;

	//通过地址调用虚函数print（）
	typedef void(*Fun)(void);
	Fun IsPrint = (Fun)* ((int*)*(int*)(&p) + 1);
	cout << endl;
	cout << "调用了虚函数";
	IsPrint(); //若地址正确，则调用了Base类的虚函数print（）
	cout << endl;

	//输入static函数的地址
	p.countI();//先调用函数以产生一个实例
	cout << "static函数countI()的地址：" << p.countI << endl;

	//验证nonstatic数据成员
	cout << "推测nonstatic数据成员baseI的地址：" << (int *)(&p) + 1 << endl;
	cout << "根据推测出的地址，输出该地址的值：" << *((int *)(&p) + 1) << endl;
	cout << "Base::getI():" << p.getI() << endl;

}

typedef void(*Fun)(void);

void TestDeriveFunction()
{
	TestDerive d(2000);
	//[0]
	cout << "[0]Base::vptr";
	cout << "\t地址：" << (int *)(&d) << endl;
	//vprt[0]
	cout << "  [0]";
	Fun fun1 = (Fun)*((int *)*((int *)(&d)));
	fun1();
	cout << "\t地址:\t" << *((int *)*((int *)(&d))) << endl;

	//vprt[1]析构函数无法通过地址调用，故手动输出
	cout << "  [1]" << "Derive::~Derive" << endl;

	//vprt[2]
	cout << "  [2]";
	Fun fun2 = (Fun)*((int *)*((int *)(&d)) + 2);
	fun2();
	cout << "\t地址:\t" << *((int *)*((int *)(&d)) + 2) << endl;
	//[1]
	cout << "[2]Base::baseI=" << *(int*)((int *)(&d) + 1);
	cout << "\t地址：" << (int *)(&d) + 1;
	cout << endl;
	//[2]
	cout << "[2]Derive::DeriveI=" << *(int*)((int *)(&d) + 2);
	cout << "\t地址：" << (int *)(&d) + 2;
	cout << endl;
}

void TestMultiDeriveFunction()
{
	TestDriveMultiBase d(3000);
	//[0]
	cout << "[0]Base::vptr";
	cout << "\t地址：" << (int *)(&d) << endl;

	//vprt[0]
	cout << "  [0]";
	Fun fun1 = (Fun)*((int *)*((int *)(&d)));
	fun1();
	cout << "\t地址:\t" << *((int *)*((int *)(&d))) << endl;

	//vprt[1]析构函数无法通过地址调用，故手动输出
	cout << "  [1]" << "Derive::~Derive" << endl;

	//vprt[2]
	cout << "  [2]";
	Fun fun2 = (Fun)*((int *)*((int *)(&d)) + 2);
	fun2();
	cout << "\t地址:\t" << *((int *)*((int *)(&d)) + 2) << endl;


	//[1]
	cout << "[1]Base::baseI=" << *(int*)((int *)(&d) + 1);
	cout << "\t地址：" << (int *)(&d) + 1;
	cout << endl;


	//[2]
	cout << "[2]Base2::vptr";
	cout << "\t地址：" << (int *)(&d) + 2 << endl;

	//vprt[0]
	cout << "  [0]";
	Fun fun4 = (Fun)*((int *)*((int *)(&d) + 2));
	fun4();
	cout << "\t地址:\t" << *((int *)*((int *)(&d))) << endl;

	//vprt[1]析构函数无法通过地址调用，故手动输出
	cout << "  [1]" << "Drive_multyBase::~Derive" << endl;

	//[3]
	cout << "[3]Base_2::base2I=" << *(int*)((int *)(&d) + 3);
	cout << "\t地址：" << (int *)(&d) + 3;
	cout << endl;

	//[4]
	cout << "[4]Drive_multyBase::Drive_multyBaseI=" << *(int*)((int *)(&d) + 4);
	cout << "\t地址：" << (int *)(&d) + 4;
	cout << endl;
}

void TestDiamondDeriveFunction()
{
	test_derive::D d(3000);
	//d[0]
	cout << "[0]B1::vptr";
	cout << "\t地址：" << (int *)(&d) << endl;

	//B1 vprt[0]
	cout << "  [0]";
	Fun fun1 = (Fun)*((int *)*((int *)(&d)));
	fun1();
	cout << "\t地址:\t" << *((int *)*((int *)(&d))) << endl;

	//B1 vprt[1]
	cout << "  [1]";
	Fun fun2 = (Fun)*((int *)*((int *)(&d)) + 1);
	fun2();
	cout << "\t地址:\t" << *((int *)*((int *)(&d)) + 1) << endl;

	//B1 vprt[2]
	cout << "  [2]";
	Fun fun3 = (Fun)*((int *)*((int *)(&d)) + 2);
	fun3();
	cout << "\t地址:\t" << *((int *)*((int *)(&d)) + 2) << endl;

	//B1 vprt[3]
	cout << "  [3]";
	Fun fun4 = (Fun)*((int *)*((int *)(&d)) + 3);
	fun4();
	cout << "\t地址:\t" << *((int *)*((int *)(&d)) + 3) << endl;

	//B1 vprt[4]
	cout << "  [4]";
	Fun fun5 = (Fun)*((int *)*((int *)(&d)) + 4);
	fun5();
	cout << "\t地址:\t" << *((int *)*((int *)(&d)) + 4) << endl;

	//d[1]
	cout << "[1]B::ib=" << *(int*)((int *)(&d) + 1);
	cout << "\t地址：" << (int *)(&d) + 1;
	cout << endl;

	//d[2]
	cout << "[2]B1::ib1=" << *(int*)((int *)(&d) + 2);
	cout << "\t地址：" << (int *)(&d) + 2;
	cout << endl;

	//d[3]
	cout << "[3]B2::vptr";
	cout << "\t地址：" << (int *)(&d) + 3 << endl;

	//B2 vprt[0]
	cout << "  [0]";
	Fun B2fun1 = (Fun)*((int *)*((int *)(&d) + 3));
	B2fun1();
	cout << "\t地址:\t" << *((int *)*((int *)(&d))) << endl;

	//B2 vprt[1]
	cout << "  [1]";
	Fun B2fun2 = (Fun)*((int *)*((int *)(&d) + 3) + 1);
	B2fun2();
	cout << "\t地址:\t" << *((int *)*((int *)(&d) + 3) + 1) << endl;

	//B2 vprt[2]
	cout << "  [2]";
	Fun B2fun3 = (Fun)*((int *)*((int *)(&d) + 3) + 2);
	B2fun3();
	cout << "\t地址:\t" << *((int *)*((int *)(&d) + 3) + 2) << endl;

	//B2 vprt[3]
	cout << "  [3]";
	Fun B2fun4 = (Fun)*((int *)*((int *)(&d) + 3) + 3);
	B2fun4();
	cout << "\t地址:\t" << *((int *)*((int *)(&d) + 3) + 3) << endl;

	//d[4]
	cout << "[4]B::ib=" << *(int*)((int *)(&d) + 4);
	cout << "\t地址：" << (int *)(&d) + 4;
	cout << endl;

	//d[5]
	cout << "[4]B2::ib2=" << *(int*)((int *)(&d) + 5);
	cout << "\t地址：" << (int *)(&d) + 5;
	cout << endl;

	//d[6]
	cout << "[5]D::id=" << *(int*)((int *)(&d) + 6);
	cout << "\t地址：" << (int *)(&d) + 6;
	cout << endl;

	cout << endl;
}

void TestVirtualDeriveFunction()
{

	test_derive::VB1 a;
	cout << "B1对象内存大小为：" << sizeof(a) << endl;

	//取得B1的虚函数表
	cout << "[0]B1::vptr";
	cout << "\t地址：" << (int *)(&a) << endl;

	//输出虚表B1::vptr中的函数
	for (int i = 0; i < 2; ++i)
	{
		cout << "  [" << i << "]";
		Fun fun1 = (Fun)*((int *)*(int *)(&a) + i);
		fun1();
		cout << "\t地址：\t" << *((int *)*(int *)(&a) + i) << endl;
	}

	//[1]
	cout << "[1]vbptr ";
	cout << "\t地址：" << (int *)(&a) + 1 << endl;  //虚表指针的地址
													//输出虚基类指针条目所指的内容
	for (int i = 0; i < 2; i++)
	{
		cout << "  [" << i << "]";

		cout << *(int *)((int *)*((int *)(&a) + 1) + i);

		cout << endl;
	}


	//[2]
	cout << "[2]B1::ib1=" << *(int*)((int *)(&a) + 2);
	cout << "\t地址：" << (int *)(&a) + 2;
	cout << endl;

	//[3]
	cout << "[3]值=" << *(int*)((int *)(&a) + 3);
	cout << "\t\t地址：" << (int *)(&a) + 3;
	cout << endl;

	//[4]
	cout << "[4]B::vptr";
	cout << "\t地址：" << (int *)(&a) + 3 << endl;

	//输出B::vptr中的虚函数
	for (int i = 0; i < 2; ++i)
	{
		cout << "  [" << i << "]";
		Fun fun1 = (Fun)*((int *)*((int *)(&a) + 4) + i);
		fun1();
		cout << "\t地址:\t" << *((int *)*((int *)(&a) + 4) + i) << endl;
	}

	//[5]
	cout << "[5]B::ib=" << *(int*)((int *)(&a) + 5);
	cout << "\t地址: " << (int *)(&a) + 5;
	cout << endl;
}

void TestVirtualDiamondDeriveFunction()
{
	test_derive::VD d;
	cout << "D对象内存大小为：" << sizeof(d) << endl;

	//取得B1的虚函数表
	cout << "[0]B1::vptr";
	cout << "\t地址：" << (int *)(&d) << endl;

	//输出虚表B1::vptr中的函数
	for (int i = 0; i < 3; ++i)
	{
		cout << "  [" << i << "]";
		Fun fun1 = (Fun)*((int *)*(int *)(&d) + i);
		fun1();
		cout << "\t地址：\t" << *((int *)*(int *)(&d) + i) << endl;
	}

	//[1]
	cout << "[1]B1::vbptr ";
	cout << "\t地址：" << (int *)(&d) + 1 << endl;  //虚表指针的地址
													//输出虚基类指针条目所指的内容
	for (int i = 0; i < 2; i++)
	{
		cout << "  [" << i << "]";

		cout << *(int *)((int *)*((int *)(&d) + 1) + i);

		cout << endl;
	}


	//[2]
	cout << "[2]B1::ib1=" << *(int*)((int *)(&d) + 2);
	cout << "\t地址：" << (int *)(&d) + 2;
	cout << endl;

	//[3]
	cout << "[3]B2::vptr";
	cout << "\t地址：" << (int *)(&d) + 3 << endl;

	//输出B2::vptr中的虚函数
	for (int i = 0; i < 2; ++i)
	{
		cout << "  [" << i << "]";
		Fun fun1 = (Fun)*((int *)*((int *)(&d) + 3) + i);
		fun1();
		cout << "\t地址:\t" << *((int *)*((int *)(&d) + 3) + i) << endl;
	}

	//[4]
	cout << "[4]B2::vbptr ";
	cout << "\t地址：" << (int *)(&d) + 4 << endl;  //虚表指针的地址
													//输出虚基类指针条目所指的内容
	for (int i = 0; i < 2; i++)
	{
		cout << "  [" << i << "]";

		cout << *(int *)((int *)*((int *)(&d) + 4) + i);

		cout << endl;
	}

	//[5]
	cout << "[5]B2::ib2=" << *(int*)((int *)(&d) + 5);
	cout << "\t地址: " << (int *)(&d) + 5;
	cout << endl;

	//[6]
	cout << "[6]D::id=" << *(int*)((int *)(&d) + 6);
	cout << "\t地址: " << (int *)(&d) + 6;
	cout << endl;

	//[7]
	cout << "[7]值=" << *(int*)((int *)(&d) + 7);
	cout << "\t\t地址：" << (int *)(&d) + 7;
	cout << endl;

	//间接父类
	//[8]
	cout << "[8]B::vptr";
	cout << "\t地址：" << (int *)(&d) + 8 << endl;

	//输出B::vptr中的虚函数
	for (int i = 0; i < 2; ++i)
	{
		cout << "  [" << i << "]";
		Fun fun1 = (Fun)*((int *)*((int *)(&d) + 8) + i);
		fun1();
		cout << "\t地址:\t" << *((int *)*((int *)(&d) + 8) + i) << endl;
	}

	//[9]
	cout << "[9]B::id=" << *(int*)((int *)(&d) + 9);
	cout << "\t地址: " << (int *)(&d) + 9;
	cout << endl;
}