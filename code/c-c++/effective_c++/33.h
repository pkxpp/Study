#include <iostream>
using namespace std;

class Base{
private:
	int x;
public:
	virtual void mf1() = 0;
	virtual void mf1(int);
	virtual void mf2();
	void mf3();
	void mf3(double);
};

class Derived: public Base{
public:
	virtual void mf1();
	void mf3();
	void mf4();
};
