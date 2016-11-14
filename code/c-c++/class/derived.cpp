#include <iostream>

using namespace std;

class Base{
public:
	int a;
	int b;
	virtual void fBase(){cout << "Base class function fBase()" << endl;}
	void fBase(int a){cout << "Base class function fBase(" << a << ")." << endl;}
};

class Derived : public Base{
public:
	using Base::fBase;
	int b;
	int c;
	void fDerived() {cout << "Derived class function fDerived()" << endl;}
	void fBase() { cout << "Derived class function fBase()" << endl;}
};

/*
class Base1{
private: 
	int a;
	int b;
};

class Derived1 : private Base1{
	int b;
	int c;
};
*/

int main()
{
	// 1. size
	//cout << "sizeof Derived: " << sizeof(Derived) << endl;		// 16
	//cout << "sizeof Base: " << sizeof(Base) << endl; 		// 8
	//cout << "sizeof Derived1: " << sizeof(Derived1) << endl;	// 16
	//cout << "sizeof Base1: " << sizeof(Base1) << endl; 		// 8

	// 2. cover
	/*Derived d;
	cout << d.c << endl;
	cout << d.b << endl;
	cout << d.Base::b << endl;
	cout << "----------------" << endl;
	d.fDerived();
	d.fBase();
	d.Base::fBase();*/

	// 3. overwrite
	Derived d;
	d.fBase();
	d.fBase(2); //error: no matching function for call to 'Derived::fBase(int)'
	d.Base::fBase(3);	

	getchar();
	return 0;
}
