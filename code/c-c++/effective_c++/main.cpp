#include <iostream>
#include "05.h"


// main
int main()
{
	// NamedObject1
	//NamedObject1<int> no1("Smallest Prime Number", 2);
	
	// NamedObject2
	std::string newDog("Persephone");
	std::string oldDog("Satch");

	NamedObject2<int> p(newDog, 2);
	return 0;
}
