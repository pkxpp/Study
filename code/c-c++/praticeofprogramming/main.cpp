#include <iostream>
#include "mytree.h"

void TestTree()
{
	CTree t;
	for(int i = 0;i < 5; i++)
	{
		int nValue = rand();
		char szName[32] = {0};
		sprintf(szName, "pktree%d", nValue);
		cout << "szName: " << szName << ", "
			<< "nValue: " << nValue << endl;
		tagTreeNode *newp = t.NewNode(szName, nValue);
		t.Insert(newp);
	}
	
	cout << "Result: " << endl;
	t.PrintNode();
}

int main()
{
	CTree t();
		

	return 0;	
}
