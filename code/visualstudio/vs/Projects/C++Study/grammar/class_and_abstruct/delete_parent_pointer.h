
#pragma once
#include <iostream>
#include <new>
using namespace std;

namespace DeleteParentPointer
{
	class A
	{
	public:
		A() {};
		~A() {}; // Ooops must use virtual ~A()
	private:
		int a = 1;
	};


	class B : public A
	{
	public:
		B() {};
		~B() {};
	private:
		int b = 2;
	};

	class VA
	{
	public:
		~VA()
		{
			cout << "~VA()..." << endl;
		}
		virtual void virFunc() = 0;
	};

	class VA1
	{
	public:
		~VA1()
		{
			cout << "~VA1()..." << endl;
		}
		virtual void virFunc1() = 0;
	};

	class VB : public VA, public VA1
	{
	public:
		~VB() 
		{
			cout << "~VB()..." << endl;
		}
		virtual void virFunc() override {}
		virtual void virFunc1() override {}

		//void operator delete(VB* p, std::destroying_delete_t) {
		//	std::cout << "In Bar::operator delete(Bar *, std::destroying_delete_t)\n";
		//	p->~Bar();
		//	::operator delete(p);
		//}
	};
}

inline void TestDeleteParentPointer()
{
	using namespace DeleteParentPointer;
	A* pB = new B();
	delete pB;

	VA* pInst1 = new VB();
	delete pInst1;					// ok

	VA1* pInst2 = new VB();
	//VB* pVB = dynamic_cast<VB*>(pInst2); 
	delete pInst2;					// crash
}
