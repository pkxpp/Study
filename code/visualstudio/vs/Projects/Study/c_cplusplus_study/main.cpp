#include <iostream>
#include "ClassLayout.h"
#include "ClassFragments.h"

using namespace std;
//////////////////////////////////////////////////////////////////////////
// content
// 1.
// 2. Class Layout
// 3.Class Fragments

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
int main(){
	//TestDecimal();

	//2. Class Layout
	//Test1();
	//TestClassLayout2();

	// 3. Class Fagments
	// TestClassFragments();

	// 4. Binary Tree
	//TestBinaryTree();


	getchar();
	return 0;
}