/*
|| How to print the contents of a simple STL list. Whew!
*/
#include <iostream>
#include <string>
#include <string>
#include <list>

using namespace std;

int main(void)
{
	list<string> Milkshakes;
	list<string>::iterator MilkshakeIterator;//为Milkshakes实例容器定义一个iterator指针：MilkshakeIterator

	Milkshakes.push_back("Chocolate");
	Milkshakes.push_back("Strawberry");
	Milkshakes.push_front("Lime");
	Milkshakes.push_front("Vanilla");

	// print the milkshakes打印
	Milkshakes.push_front("The Milkshake Menu");
	Milkshakes.push_back("*** Thats the end ***");
	for (MilkshakeIterator = Milkshakes.begin(); MilkshakeIterator != Milkshakes.end(); ++MilkshakeIterator)
	{
		// dereference the iterator to get the element
		cout << *MilkshakeIterator << endl; //输出
	}

	getchar();

	return 0;
}