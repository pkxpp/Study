#include "ObjectMemoryStudy.h"


void ObjectMemoryStudy::TestSingleInherit()
{
	typedef void(*Fun)(void);

	GrandChild gc;

	int** pVtab = (int**)&gc;

	cout << "[0] GrandChild::_vptr->" << endl;

	for (int i = 0; (Fun)pVtab[0][i] != NULL; i++) {

		Fun pFun = (Fun)pVtab[0][i];

		cout << " [" << i << "] ";

		pFun();

	}

	cout << "[1] Parent.iparent = " << (int)pVtab[1] << endl;

	cout << "[2] Child.ichild = " << (int)pVtab[2] << endl;

	cout << "[3] GrandChild.igrandchild = " << (int)pVtab[3] << endl;
}