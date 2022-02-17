#pragma once

#include <iostream>
#include<memory>	// error C2039: 'shared_ptr' : is not a member of 'std'

using namespace std;

class A {
public:
	//A() {};
	A(int a) : num(a){
		cout << "A(int a) = " << a << endl;
	}

	A(A& a) : num(a.num) {
		cout << "A(A& a)" << endl;
	}
	//A(const A& a) : num(a.num) {}
	//A(const int& a) : num(a) {}

	// classical implementation
	A& operator=(A const & rhs) { 
		cout << "operator=(A const & rhs)" << endl;
		return *this; 
	}
	A& operator=(A&& rhs);
private:
	int num;
};

A& A::operator=(A&& rhs)
{
	cout << "operator=(A&& rhs)" << endl;
	// Move semantics: exchange content between this and rhs
	return *this;
}

//////////////////////////////////////////////////////////////////////////

A foobar()
{
	cout << "foobar()" << endl;
	int a = 0;
	return A(a);
}

namespace NS1
{
	//foo can be called on l - values, but not on r - values.If you implement

	void foo(A&)
	{
		cout << "foo(A&)" << endl;
	}
}

namespace NS2
{
	//foo can be called on l - values, but not on r - values.If you implement

	void foo(const A&)
	{
		cout << "foo(A&)" << endl;
	}

	void foo(A&&)
	{
		cout << "foo(X&&)" << endl;
	}
}

namespace NS3
{
	//foo can be called on l - values, but not on r - values.If you implement

	void foo(A&)
	{
		cout << "foo(A&)" << endl;
	}

	void foo(A&&)
	{
		cout << "foo(X&&)" << endl;
	}
}
//////////////////////////////////////////////////////////////////////////

template <class T>
std::shared_ptr<T>
factory()   // no argument version
{
	cout << "factory()" << endl;
	return std::shared_ptr<T>(new T);
}

//template <class T, class A1>
//std::shared_ptr<T>
//factory(const A1& a1)   // one argument version
//{
//	return std::shared_ptr<T>(new T(a1));
//}

//template <class T, class A1>
//std::shared_ptr<T>
//factory(A1& a1)
//{
//	return std::shared_ptr<T>(new T(a1));
//}

template <class T, class A1>
std::shared_ptr<T>
factory(A1&& a1)
{
	cout << "factory(A1&& a1)" << endl;
	return std::shared_ptr<T>(new T(std::forward<A1>(a1)));
}

void TestRvalueReferences()
{
	//int a = 5;
	//A sA(a);
	//NS1::foo(sA);
	//NS1::foo(foobar());

	//NS2::foo(foobar());
	//////////////////////////////////////////////////////////////////////////
	A* q = new A(5);
	// if not factory(const A1& a1)
	// error C2664: 'std::shared_ptr<_Ty> factory<A,int>(A1 &)' : cannot convert parameter 1 from 'int' to 'int &'
	std::shared_ptr<A> p = factory<A>(6);

	p = factory<A>(*q);

	
	//p = factory<A>(a);
	//int& b = a;
	//p = factory<A>(b);

	
}