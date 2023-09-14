#include <iostream>
#include <vector>
#include <functional>
#include "ClassLayout.h"
#include "ClassFragments.h"
//#include "6-7-1.h"
#include "namespace_interfaceprinciple.h"
#include "template.h"


using namespace std;
//////////////////////////////////////////////////////////////////////////
// content
// 1.
// 2. Class Layout
// 3.Class Fragments
// 4.
// 5. heap alogrithm test
// 7. templates

void TestDecimal(){
	/*cout << "INT_MIN: " << INT_MIN << endl;
	cout << "INT_MAX: " << INT_MAX << endl;*/
    int nNum = 2147483648;
    if (nNum >= 2147483648 && nNum <= 2147483647){
        cout << "11111111" << endl;
    }
    else
        cout << "2222222222" << endl;
}

//////////////////////////////////////////////////////////////////////////
// 2 class object Layout
void Test1(){
	cout << "sizeof class: " << endl;
	cout << "sizeof(SelfStudy::Base) = " << sizeof(SelfStudy::Base) << endl;
	cout << "sizeof(SelfStudy::Derive) = " << sizeof(SelfStudy::Derive) << endl;
	cout << "sizeof(SelfStudy::Base1) = " << sizeof(SelfStudy::Base1) << endl;
	cout << "sizeof(SelfStudy::Base2) = " << sizeof(SelfStudy::Base2) << endl;
	cout << "sizeof(SelfStudy::Base3) = " << sizeof(SelfStudy::Base3) << endl;
	cout << "sizeof(SelfStudy::Derive1) = " << sizeof(SelfStudy::Derive1) << endl;

	SelfStudy::Base b;
	b.b = 10;
	Fun pFun = NULL;

	cout << "&b = " << &b << endl;
	cout << "vtable[0] = " << (int*)*((int*)(&b)) << endl;
	cout << "vtable[1] = " << (int*)*((int*)(&b)) + 1 << endl;
	cout << "vtable[2] = " << (int*)*((int*)(&b)) + 2 << endl;
	cout << "&b.b = " << &b.b << endl;
	cout << "*(&b + 1) = " << *((int*)(&b) + 1) << endl;

	//Base::f()
	pFun = (Fun)*((int*)*((int*)(&b)));		
	pFun();
	// Base::g()
	pFun = (Fun)*((int*)*((int*)(&b))+1);
	pFun();
	// Base::h()
	pFun = (Fun)*((int*)*((int*)(&b))+2);
	pFun();

	SelfStudy::Derive d;
	d.b = 10;
	d.d = 100;
	for (int i = 0; i < 6; ++i)
	{
		cout << "SelfStudy::Derive vtable[" << i << "] = " << (int*)*((int*)(&d)) + i << endl;
		( (Fun)*((int*)*((int*)(&d)) + i ) )();
		/*pFun = (Fun)*((int*)*((int*)(&b))+i);
		pFun();*/
	}
	cout << "*(&d + 1) = " << *((int*)(&d) + 1) << endl;
	cout << "*(&d + 2) = " << *((int*)(&d) + 2) << endl;

	SelfStudy::Derive1 d1;
	d1.b1 = 10;
	d1.b2 = 11;
	d1.b3 = 12;
	d1.d = 100;
	for (int i = 0; i < 6; ++i)
	{
		cout << "SelfStudy::Derive1 vtable1[" << i << "] = " << (int*)*((int*)(&d1)) + i << endl;
		( (Fun)*((int*)*((int*)(&d1)) + i ) )();
	}
	int nOffSet = sizeof(SelfStudy::Base1)/sizeof(int);
	for (int i = 0; i < 3; ++i)
	{
		cout << "SelfStudy::Derive1 vtable2[" << i << "] = " << (int*)*((int*)(&d1) + nOffSet) + i << endl;
		( (Fun)*((int*)*((int*)(&d1) + nOffSet) + i ) )();
	}
	nOffSet = (sizeof(SelfStudy::Base1) + sizeof(SelfStudy::Base2))/sizeof(int);
	for (int i = 0; i < 3; ++i)
	{
		cout << "SelfStudy::Derive1 vtable2[" << i << "] = " << (int*)*((int*)(&d1) + nOffSet) + i << endl;
		( (Fun)*((int*)*((int*)(&d1) + nOffSet) + i ) )();
	}
	cout << "*(&d + 1) = " << *((int*)(&d1) + 1) << endl;
	//cout << "*(&d + 2) = "<< hex << *((int*)(&d1) + 2) << endl;
}

void TestClassLayout2()
{
	int** pVtab = NULL;
	Fun pFun = NULL;

	SelfVirtual::B1 bb1;

	pVtab = (int**)&bb1;
	cout << "[0] B1::_vptr->" << endl;
	pFun = (Fun)pVtab[0][0];
	cout << "     [0] ";
	pFun(); //B1::f1();
	cout << "     [1] ";
	pFun = (Fun)pVtab[0][1];
	pFun(); //B1::bf1();
	cout << "     [2] ";
	cout << pVtab[0][2] << endl;

	cout << "[1] = 0x";
	cout << (int*)*((int*)(&bb1)+1) <<endl; //B1::ib1
	cout << "[2] B1::ib1 = ";
	cout << (int)*((int*)(&bb1)+2) <<endl; //B1::ib1
	cout << "[3] B1::cb1 = ";
	cout << (char)*((int*)(&bb1)+3) << endl; //B1::cb1

	cout << "[4] = 0x";
	cout << (int*)*((int*)(&bb1)+4) << endl; //NULL

	cout << "[5] B::_vptr->" << endl;
	pFun = (Fun)pVtab[5][0];
	cout << "     [0] ";
	pFun(); //B1::f();
	pFun = (Fun)pVtab[5][1];
	cout << "     [1] ";
	pFun(); //B::Bf();
	cout << "     [2] ";
	cout << "0x" << (Fun)pVtab[5][2] << endl;

	cout << "[6] B::ib = ";
	cout << (int)*((int*)(&bb1)+6) <<endl; //B::ib
	cout << "[7] B::cb = ";
}
//////////////////////////////////////////////////////////////////////////
// 3.Class Fragments
void TestClassFragments(){
	printf("&Point3d::x = %p\n", &ClassFragments::Point3d::x);
	printf("&Point3d::y = %p\n", &ClassFragments::Point3d::y);
	printf("&Point3d::z = %p\n", &ClassFragments::Point3d::z);

	// multi 
	printf("&Base1::val1 = %p\n", &ClassFragments::Base1::val1);
	printf("&Base2::val2 = %p\n", &ClassFragments::Base2::val2);
	printf("&Derived::val1 = %p\n", &ClassFragments::Derived::val1);
	printf("&Derived::val2 = %p\n", &ClassFragments::Derived::val2);
}

//////////////////////////////////////////////////////////////////////////
// 5.
//int even_by_two::_x = 0;
//
//void TestHeapAlogrithm(){
//	int ia[] = { 0, 1, 2, 3, 4, 5, 6, 6, 6, 7, 8 };
//	vector<int> iv(ia, ia + sizeof(ia) / sizeof(int));
//
//	for_each(iv.begin(), iv.end(), display<int>());
//	cout << endl;
//
//	vector<int> iv2(ia + 6, ia + 8);
//	generate(iv2.begin(), iv2.end(), even_by_two());
//	for_each(iv2.begin(), iv2.end(), display<int>());
//	cout << endl;
//
//	generate_n(iv.begin(), 3, even_by_two());
//	for_each(iv.begin(), iv.end(), display<int>());
//	cout << endl;
//
//	remove(iv.begin(), iv.end(), 6);
//	for_each(iv.begin(), iv.end(), display<int>());
//	cout << endl;
//
//	vector<int> iv3(12);
//	remove_copy(iv.begin(), iv.end(), iv3.begin(), 6);
//	for_each(iv3.begin(), iv3.end(), display<int>());
//	cout << endl;
//
//	remove_if(iv.begin(), iv.end(), bind2nd(less<int>(), 6));
//	for_each(iv.begin(), iv.end(), display<int>());
//	cout << endl;
//	cout << "sizeof(iv) = " << iv.size() << endl;
//
//	remove_copy_if(iv.begin(), iv.end(), iv3.begin(), bind2nd(less<int>(), 7));
//	for_each(iv3.begin(), iv3.end(), display<int>());
//	cout << endl;
//}

struct BigInt {
	int64_t h;
	int64_t l;
};
int64_t Fibonacci2(int64_t n)
{
	static int nDepth = 1;
	++nDepth;
	std::function<int64_t(int64_t, int64_t, int64_t)> iter = [&](int64_t n, int64_t prev, int64_t next)
	{
		if (n == 0)
			return prev;
		return iter(n - 1, next, prev + next);
	};
	return iter(n, 0, 1);
};

//////////////////////////////////////////////////////////////////////////
int main() {
	//TestDecimal();

	//2. Class Layout
	//Test1();
	//TestClassLayout2();

	// 3. Class Fagments
	// TestClassFragments();

	// 4. Binary Tree
	//TestBinaryTree();

	// 5. heap alogrithm
	//TestHeapAlogrithm();

	// 6.
	//TestNamespaceAndInterfacePrinciple();

	// 7.
	TestTemplate();

	static int nDepth = 1;
	std::function<int(int)> Fibonacci = [&](int n)
	{
		if (n <= 1)
			return n;
		else
			return Fibonacci(n - 1) + Fibonacci(n - 2);
	};

	static int nDepth1 = 1;
	std::function<int64_t(int64_t, int64_t, int64_t)> Fibonacci1 = [&](int64_t n, int64_t prev, int64_t next)
	{
		++nDepth1;
		if (n == 0)
			return prev;
		return Fibonacci1(n - 1, next, prev + next);
	};

	std::cout << "[0] =" << Fibonacci(0) << std::endl;
	std::cout << "[1] =" << Fibonacci(1) << std::endl;
	std::cout << "[2] =" << Fibonacci(2) << std::endl;
	std::cout << "[3] =" << Fibonacci(3) << std::endl;
	std::cout << "[4] =" << Fibonacci(4) << std::endl;
	std::cout << "[5] =" << Fibonacci(5) << std::endl;
	std::cout << "[6] =" << Fibonacci(6) << std::endl;
	std::cout << "[7] =" << Fibonacci(7) << std::endl;
	std::cout << "[8] =" << Fibonacci(8) << std::endl;
	std::cout << "[9] =" << Fibonacci(9) << std::endl;
	std::cout << "[10] =" << Fibonacci(10) << ", " << Fibonacci1(10, 0, 1) << std::endl;
	std::cout << "[20] =" << Fibonacci(20) << ", " << Fibonacci1(20, 0, 1) << std::endl;
	std::cout << "[23] =" << Fibonacci(23) << ", " << Fibonacci1(23, 0, 1) << std::endl;
	std::cout << "[24] =" << Fibonacci(24) << ", " << Fibonacci1(24, 0, 1) << std::endl;
	std::cout << "[32] =" << Fibonacci(32) << ", " << Fibonacci1(32, 0, 1) << std::endl;
	//std::cout << "[2024] =" << Fibonacci1(2024, 0, 1) << std::endl;
	std::cout << "[32] =" << Fibonacci2(32) << std::endl;

	getchar();
	return 0;
}