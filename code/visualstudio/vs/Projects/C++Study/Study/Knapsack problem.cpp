//////////////////////////////////////////////////////////////////////////
// page@2014/07/31 背包问题-动态规划
//////////////////////////////////////////////////////////////////////////
#include <iostream>

using namespace std;
typedef struct _tagGoods
{
	int nValue;
	int nWeight;
}Goods; 

int Max(int a, int b)
{
	return ((a > b) ? a : b);
}

int PrintPack(int c[100][100], Goods *A, int nItem, int nSize)
{
	int *Ret = new int[nItem];
	for (int i = 0; i < nItem; i++)
		Ret[i] = 0;
	for (int i= nItem; i > 0 ; i--)
	{
		if(c[i][nSize] > c[i-1][nSize])
		{
			Ret[i-1] = 1;
			nSize -= A[i-1].nWeight;
		}
	}
	cout << "PrintPack: " << endl;
	for (int j = 0; j < nItem; j++)
	{
		cout << Ret[j] << ", ";
	}
	cout << endl;

	delete [] Ret;
	return true;
}

int Pack(Goods *A, int nItem, int nSize)
{
	//
	int MN[100][100];
	int nMax = 0;
	for(int i = 0; i < 100; i++)
		for(int j = 0; j < 100; j++)
			MN[i][j] = 0;
	for (int i=1; i <= nItem; i++)
	{
		for (int j = 1; j <= nSize; j++)
		{
			if (j < A[i-1].nWeight)
			{
				MN[i][j] = MN[i-1][j];
			}
			else
			{
				MN[i][j] = Max(MN[i-1][j], MN[i-1][j-A[i-1].nWeight]+A[i-1].nValue);
			}
		}
	}

	cout << "Result: " << endl;
	for (int i = 0; i < nItem; i++)
	{
		for (int j =0; j < nSize; j++ )
			cout << MN[i][j] << ", ";
		cout << endl;
	}
	cout << "End..." << endl;

	PrintPack(MN, A, nItem, nSize);
	return MN[nItem][nSize];
}

int TestKnapsack()
{
	const int GOODS_NUM= 5;
	Goods A[GOODS_NUM];
	//int arrValues[GOODS_NUM] = {2, 11, 15, 20, 32};
	//int arrWeight[GOODS_NUM] = {2, 11, 15, 20, 32};
	//例子
	//int arrValues[GOODS_NUM] = {1, 4, 3, 6, 8};
	//int arrWeight[GOODS_NUM] = {2, 3, 4, 5, 6};
	// 我的需求:子集和>x的最小值
	int arrNeed[GOODS_NUM] = {2, 5, 11, 15, 20};
	int arrValues[GOODS_NUM] = {2, 5, 11, 15, 20};

	int nTarget = 12;
	int nSum = 0;
	for (int i = 0; i < GOODS_NUM; i++)
	{
		nSum += arrNeed[i];
	}
	nTarget = nSum - nTarget;
	cout << "nSum: " << nSum << ", nTarget: " << nTarget << endl;

	int arrWeight[GOODS_NUM] = {2, 5, 11, 15, 20};
	for (int i = 0; i < GOODS_NUM; i++)
	{
		A[i].nWeight = arrWeight[i];
		A[i].nValue = arrValues[i];
	}
	
	int nRet = Pack(A, GOODS_NUM, nTarget);
	cout << "nRet: " << nRet << endl;

	getchar();
	return true;
}