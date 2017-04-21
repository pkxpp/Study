#include <iostream>
#include "ClassLayout.h"
using namespace std;
//////////////////////////////////////////////////////////////////////////
// content
// 1.
// 2. Class Layout

void TestDecimal(){
    cout << "INT_MIN: " << INT_MIN << endl;
    cout << "INT_MAX: " << INT_MAX << endl;
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
	cout << "*(&d + 2) = "<< hex << *((int*)(&d1) + 2) << endl;
}

//////////////////////////////////////////////////////////////////////////
int main(){
	//TestDecimal();

	//2. Class Layout
	Test1();

	getchar();
	return 0;
}