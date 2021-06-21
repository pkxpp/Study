#include <iostream>
using namespace std;

class A{
public:
	A(){}
	~A(){}
	void testA(){
		cout << "A::test." << endl;
	}
};

class B{
public:
	void testB(){
		cout << "B::test." << endl;
	}
};

int main()
{
	goto Exit;
	int a = 1;
	//A classA;		// failed:
	B classB;		// success:
	classB.testB();
Exit:
	cout << "a = " << a << endl;
	classB.testB();
	getchar();
	getchar();
	return 0;
}