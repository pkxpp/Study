#include "inherited_study.h"



TestInheritedA::TestInheritedA()
{
}


TestInheritedA::~TestInheritedA()
{
}

//////////////////////////////////////////////////////////////////////////

TestInheritedB::TestInheritedB()
{
}


TestInheritedB::~TestInheritedB()
{
}

//////////////////////////////////////////////////////////////////////////
void TestInherited()
{
	cout << "Test Inherited ..." << endl;
	
	TestInheritedB b;
	b.FuncInheritedBase();
}