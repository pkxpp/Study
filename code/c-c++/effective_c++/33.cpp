#include "33.h"

///////////////////////////////////////
//class Base
void Base::mf1(int){
	cout << ("Base::mf1(int)") << endl;
}

void Base::mf2(){
	cout << "Base::mf2()" << endl;
}

void Base::mf3(){
	cout << "Base::mf3()" << endl;
}

void Base::mf3(double){
	cout << "Base::mf3(double)" << endl;
}

/////////////////////////////////////////
//class Derived
void Derived::mf1(){
	cout << "Derived::mf1()" << endl;
}

void Derived::mf3(){
	cout << "Derived::mf3()" << endl;
}

void Derived::mf4(){
	cout << "Derived::mf4()" << endl;
}
