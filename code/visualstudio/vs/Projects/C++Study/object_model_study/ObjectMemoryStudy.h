#pragma once
#include <iostream>

using namespace std;

class ObjectMemoryStudy
{
public:
	void TestSingleInherit();
};

class Parent {

public:

	int iparent;

	Parent() :iparent(10) {}

	virtual void f() { cout << " Parent::f()" << endl; }

	virtual void g() { cout << " Parent::g()" << endl; }

	virtual void h() { cout << " Parent::h()" << endl; }

};

class Child : public Parent {

public:

	int ichild;

	Child() :ichild(100) {}

	virtual void f() { cout << "Child::f()" << endl; }

	virtual void g_child() { cout << "Child::g_child()" << endl; }

	virtual void h_child() { cout << "Child::h_child()" << endl; }

};

class GrandChild : public Child {

public:

	int igrandchild;

	GrandChild() :igrandchild(1000) {}

	virtual void f() { cout << "GrandChild::f()" << endl; }

	virtual void g_child() { cout << "GrandChild::g_child()" << endl; }

	virtual void h_grandchild() { cout << "GrandChild::h_grandchild()" << endl; }

};

