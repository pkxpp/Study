#include <iostream>
#include "05.h"
//#include "29.h"	// exist errors
#include "33.h"


// main
int main()
{
	// NamedObject1
	//NamedObject1<int> no1("Smallest Prime Number", 2);
	
	// NamedObject2
	/*
	std::string newDog("Persephone");
	std::string oldDog("Satch");

	NamedObject2<int> p(newDog, 2);
	
	NamedObject2<int> s(oldDog, 36);
	//05.h:28: error: non-static reference member ‘std::string& NamedObject2<int>::nameValue’, can't use default assignment operator
	p = s;*/

	/*********************************************/
	// 29
	/*Point coord1(0, 0);
	Point coord2(100, 100);
	const Rectangle rec(coord1, coord2);
	
	rec.upperLeft().setX(50);*/


	/*********************************************/
	// 33.class public
	Derived d;
	int x;
	d.mf1();
	//d.mf1(x); // error: no match
	d.mf2();
	d.mf3();
	//d.mf3(x); // error: not match...

	return 0;
}
