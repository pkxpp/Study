#include <iostream>
#include "TestBase.h"
#include "ObjectMemoryStudy.h"

#include <windows.h>
#include <d3d11.h>

using namespace std;

int main()
{
	//TestObjectModel();

	ObjectMemoryStudy om;
	om.TestSingleInherit();

	getchar();
	return 0;
}

int WINAPI WinMain(
	__in HINSTANCE hInstance,
	__in_opt HINSTANCE hPrevInstance,
	__in LPSTR lpCmdLine,
	__in int nShowCmd
)
{
	int nResult = false;

	do
	{
		ObjectMemoryStudy om;
		om.TestSingleInherit();

		nResult = true;
	} while (0);

	return 0;
}