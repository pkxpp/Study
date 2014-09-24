#include <iostream>
#include "mytree.h"
#include <time.h>
#include <math.h>

void TestTreeOrder(tagTreeNode* pTree, void*){
	
}

void TestTree()
{
	CTree t;
	const int cnSum = 1000000;
	clock_t tStart;
	clock_t tEnd;
	std::cout << "Test: " << cnSum << std::endl;
	tStart = clock();
	for(int i = 0;i < cnSum; i++)
	{
		int nValue = rand();
		char szName[32] = {0};
		sprintf(szName, "pktree%d", i);
		//cout << "szName: " << szName << ", "
		//	<< "nValue: " << nValue << endl;
		tagTreeNode *newp = t.NewNode(szName, nValue);
		//cout << "newp: " << newp << endl;
		t.Insert(newp);
	}
	tEnd = clock();
	std::cout << "cost time(s): " << (double)(tEnd-tStart)/CLOCKS_PER_SEC << std::endl;
	//
	//cout << "Result: " << endl;
	//t.PrintNode();
	// 递归查找
	int nFind = 0;
	int nNotFind = 0;
	char szName[32] = {0};
	srand((unsigned)time(NULL));	// 随机种子
	tStart = clock();
	for (int i = 0; i < cnSum; i++){
		int nRand = rand()%(10*cnSum);
		sprintf(szName, "pktree%d", nRand);
		tagTreeNode *pNode = t.LookUp(szName);
		if (pNode){
			nFind++;
			//std::cout << nRand << ", "
			//	<< pNode->name << ", " 
			//	<< pNode->value << std::endl;
		}
		else
			nNotFind++;
	}
	tEnd = clock();
	std::cout << "递归：" << std::endl
		<< "Find: " << nFind << std::endl
		<< "NotFind: " << nNotFind << std::endl
		<< "cost time(s): " << (double)(tEnd-tStart)/CLOCKS_PER_SEC << std::endl;
	
	// 循环查找
	nFind = 0;
	nNotFind = 0;
	tStart = clock();
	for (int i = 0; i < cnSum; i++){
		int nRand = rand()%(10*cnSum);
		sprintf(szName, "pktree%d", nRand);
		tagTreeNode *pNode = t.NrLookUp(szName);
		if (pNode){
			nFind++;
			//std::cout << nRand << ", "
			//	<< pNode->name << ", " 
			//	<< pNode->value << std::endl;
		}
		else
			nNotFind++;
	}
	tEnd = clock();
	std::cout << "循环：" << std::endl
		<< "Find: " << nFind << std::endl
		<< "NotFind: " << nNotFind << std::endl
		<< "cost time(s): " << (double)(tEnd-tStart)/CLOCKS_PER_SEC << std::endl;

	// 中序排序
	tStart = clock();
	t.ApplyInOrder();
	tEnd = clock();
	std::cout << "中序排序：" << std::endl
		<< "cost time(s): " << (double)(tEnd-tStart)/CLOCKS_PER_SEC << std::endl;

}

int main()
{
	//CTree t();
	TestTree();	

	return 0;	
}
