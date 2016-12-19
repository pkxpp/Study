//////////////////////////////////////////////////////////////////////////
// page@2014/07/31 背包问题-动态规划
//////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <time.h>
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

int PrintPack(int **c, Goods *A, int nItem, int nSize)
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
	//int (*MN)[1024];

	int **MN = new int*[nItem+1];//定义一维动态数组
	for (int i = 0; i<nItem + 1; i++)//用循环令一维数组变成二维
	{
		MN[i] = new int[nSize + 1];
	}
	int nMax = 0;
	for (int i = 0; i < nItem; i++)
		for (int j = 0; j < nSize; j++)
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

	/*cout << "Result: " << endl;
	for (int i = 0; i <= nItem; i++)
	{
		for (int j =0; j <= nSize; j++ )
			cout << MN[i][j] << ", ";
		cout << endl;
	}
	cout << "End..." << endl;*/

	PrintPack(MN, A, nItem, nSize);
	int nRet = MN[nItem][nSize];

	for (int i = 0; i<nItem + 1; i++)
	{
		delete [] MN[i];
	}
	//delete [] MN;
	return nRet;
}

int main()
{
	const int GOODS_NUM= 2;
	Goods A[GOODS_NUM];
	//int arrValues[GOODS_NUM] = {2, 11, 15, 20, 32};
	//int arrWeight[GOODS_NUM] = {2, 11, 15, 20, 32};
	//例子
	//int arrValues[GOODS_NUM] = {1, 4, 3, 6, 8};
	//int arrWeight[GOODS_NUM] = {2, 3, 4, 5, 6};
	// 我的需求:子集和>x的最小值
	//int arrNeed[GOODS_NUM] = {2, 5, 11, 15, 20};
	//int arrValues[GOODS_NUM] = {2, 5, 11, 15, 20};
	//int arrNeed[GOODS_NUM] = { 2, 11, 15, 20, 25, 29 };
	int arrValues[GOODS_NUM] = { 45, 50};
	int arrWeight[GOODS_NUM] = {45, 50 };

	int nTarget = 44;
	int nSum = 0;
	for (int i = 0; i < GOODS_NUM; i++)
	{
		nSum += arrWeight[i];
	}
	nTarget = nSum - nTarget;
	cout << "nSum: " << nSum << ", nTarget: " << nTarget << endl;

	
	for (int i = 0; i < GOODS_NUM; i++)
	{
		A[i].nWeight = arrWeight[i];
		A[i].nValue = arrValues[i];
	}
	
	int nRet = Pack(A, GOODS_NUM, nTarget);
	cout << "nRet: " << nRet << endl;

	time_t tNow = time(NULL);
	char szTime[256] = { 0 };
	cout << ctime_s(szTime, 256, &tNow) << endl;
	cout << szTime << endl;

	//压力测试
	const int nCircl = 10;
	const int C_ITMES = 10;
	const int C_SIZE = 100;
	srand((unsigned int)time(NULL));

	time_t tStart = time(NULL);
	for (int i = 0; i < nCircl; i++)
	{
		int nItemsR = rand() % C_ITMES + 1;
		Goods *pA = new Goods[nItemsR];
		cout << "元数据: ";
		for (int j = 0; j < nItemsR; j++)
		{
			int nSizeR = rand() % C_SIZE + 1;
			pA[j].nValue = nSizeR;
			pA[j].nWeight = nSizeR;
			cout << nSizeR << ",";
		}
		
		int nSum = 0;
		for (int k = 0; k < nItemsR; k++)
		{
			nSum += pA[k].nWeight;
		}

		int nTargetR = rand() % nSum + 1;
		cout << ", 目标：" << nTargetR << endl;
		nTargetR = nSum - nTargetR;

		nRet = Pack(pA, nItemsR, nTargetR);
		//cout << "nRet: " << nRet << endl;
	}
	time_t tEnd = time(NULL);
	cout << "耗时：" << difftime(tEnd, tStart) << "秒" << endl;

	getchar();
	return true;
}