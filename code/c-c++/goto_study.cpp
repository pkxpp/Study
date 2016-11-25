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
#if 0			// failed
	int a = 1;
#elif 0			// work ok
	{
		int i =  1;
	}
#else
	int a;		// work ok
	a = 1;
#endif
	//A classA;	// failed
	B classB;
Exit:
	cout << "a = " << a << endl;
	classB.testB();
	return 0;
}
