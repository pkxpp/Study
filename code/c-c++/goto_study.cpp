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
	
	/*int a = 0;
	cout << "Input a number: " << endl;
	cin >> a;

	if (a < 10)
	{
		cout << "number less than 10" << endl;
		goto Exit;
		int b = 0;
	}
	else
	{
		cout << "number is: " << a  << endl;
	}
	int b;
	b = 6;
Exit:
	//b = a;
	cout << "number is: " << a + b << endl;

	getchar();
	getchar();*/
	goto Exit;
#ifdef 0		// failed
	int a = 1;
#elif 1			// work ok
	{
		int i =  1;
	}
#else
	int a;		// work ok
	a = 1;
#endif
	B classB;
	
Exit:
	//cout << "a = " << a << endl;
	classB.testB();
	return 0;
}
