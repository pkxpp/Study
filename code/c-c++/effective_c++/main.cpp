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
	
	NamedObject2<int> s(oldDog, 36);
	//05.h:28: error: non-static reference member ‘std::string& NamedObject2<int>::nameValue’, can't use default assignment operator
	p = s;
	return 0;
}
